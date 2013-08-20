#include <QGVEdge.h>
#include <QGVScene.h>
#include <QDebug>
#include <QPainter>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

QGVEdge::QGVEdge(Agedge_t* edge, QGVNode* src, QGVNode* dst, QGVScene* scene)
  : _edge(edge), _src(src), _dst(dst), _scene(scene), arrowSize(10) {
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  label->setParentItem(this);
  connect(label, SIGNAL(labelIsUpdated(LabelItem*)), this, SLOT(labelIsUpdated(LabelItem*)));
}

QGVEdge::~QGVEdge() {
  _scene->removeItem(this);
}

void QGVEdge::labelIsUpdated(LabelItem*) {
  setLabel(label->getLabel());
}


QString QGVEdge::getLabel() const {
  return getAttribute("xlabel");
}

void QGVEdge::setLabel(const QString& str) {
  GraphicItem::setLabel(str);
  setAttribute("xlabel", str);
}


QRectF QGVEdge::boundingRect() const {
  return _path.boundingRect() | _head_arrow.boundingRect() | _label_rect;
}

QPainterPath QGVEdge::shape() const {
  QPainterPathStroker ps;
  ps.setCapStyle(_pen.capStyle());
  ps.setWidth(_pen.widthF() + 10);
  ps.setJoinStyle(_pen.joinStyle());
  ps.setMiterLimit(_pen.miterLimit());
  return ps.createStroke(_path);
}

void QGVEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  painter->save();

  if (isSelected()) {
    QPen tpen(_pen);
    tpen.setColor(_pen.color().darker(120));
    tpen.setStyle(Qt::DotLine);
    painter->setPen(tpen);
  }
  else
    painter->setPen(_pen);


  painter->drawPath(_path);
  //painter->drawText(_label_rect, Qt::AlignCenter, _label);
  painter->setBrush(QBrush(_pen.color(), Qt::SolidPattern));
  painter->drawPolygon(_head_arrow);
  painter->restore();
}

void QGVEdge::setAttribute(const QString& name, const QString& value) {
  agsafeset(_edge, name.toLocal8Bit().data(), value.toLocal8Bit().data(), "");
}

QString QGVEdge::getAttribute(const QString& name) const {
  char* value = agget(_edge, name.toLocal8Bit().data());

  if (value)
    return value;

  return QString();
}

void QGVEdge::updateLayout() {
  //qDebug() << Q_FUNC_INFO << QGVEdge::label();
  prepareGeometryChange();

  qreal gheight = QGVCore::graphHeight(_scene->_graph);

  const splines* spl = ED_spl(_edge);
  _path = QGVCore::toPath(spl, gheight);

  //Edge arrows
  if ((spl->list != 0) && (spl->list->size % 3 == 1)) {
    if (spl->list->eflag) {
      QLineF line(QGVCore::toPoint(spl->list->list[spl->list->size - 1], gheight), QGVCore::toPoint(spl->list->ep, gheight));
      // Set the arrow
      double angle = ::acos(line.dx() / line.length());

      if (line.dy() >= 0)
        angle = TwoPi - angle;

      _head_arrow = toArrow(angle, line.p2());
    }
  }

  _pen.setWidth(1);
  _pen.setColor(QGVCore::toColor(getAttribute("color")));
  _pen.setStyle(QGVCore::toPenStyle(getAttribute("style")));

  //Edge label
  textlabel_t* xlabel = ED_xlabel(_edge);

  if (xlabel) {
    _label = xlabel->text;
    _label_rect.setSize(QSize(xlabel->dimen.x + 40, xlabel->dimen.y));
    _label_rect.moveCenter(QGVCore::toPoint(xlabel->pos, QGVCore::graphHeight(_scene->_graph)));
    label->setPos(_label_rect.center());
  }

  setToolTip(getDescription());
}

void QGVEdge::setToolTip(const QString& toolTip) {
  label->setToolTip(toolTip);
  QGraphicsItem::setToolTip(toolTip);
}

void QGVEdge::adjust() {
  qDebug() << Q_FUNC_INFO << QGVEdge::getLabel();

//   if (_path.isEmpty())
//     return;

  if (!_src || !_dst)
    return;

  QLineF line(_src->getCenter(), _dst->getCenter());
  QPainterPath path;
  path.moveTo(line.p1());

  if (_src == _dst) {
    path.cubicTo(scene()->sceneRect().topLeft(), scene()->sceneRect().bottomRight(), _dst->getCenter());
  }
  else {
    QLineF l = line.normalVector();
    l.setLength(l.length() / 3);
    path.cubicTo(l.p2(), line.p2(), line.p2());
  }

  path.lineTo(line.p2());
  _label_rect.moveCenter(path.pointAtPercent(0.5));

  // Find intersection point
  QPolygonF endPolygon = _dst->shape().toFillPolygon();
  QPointF p1 = endPolygon.first() + mapFromItem(_dst, 0, 0);
  QPointF p2;
  QPointF intersectPoint;
  QLineF segment;

  QList<QPolygonF> polygonList = path.toSubpathPolygons();
  QPointF p21;
  QPointF p22;
  QLineF pathSegment;

  for (int i = 1; i < endPolygon.count(); ++i) {
    p2 = endPolygon.at(i) + mapFromItem(_dst, 0, 0);
    segment = QLineF(p1, p2);

    QLineF::IntersectType intersectType;
    p21 = polygonList.first().first();

    for (int j = 1; j < polygonList.first().count(); ++j) {
      p22 = polygonList.first().at(j);
      pathSegment = QLineF(p21, p22);

      intersectType = segment.intersect(pathSegment, &intersectPoint);

      if (intersectType == QLineF::BoundedIntersection)
        break;

      p21 = p22;
    }

    if (intersectType == QLineF::BoundedIntersection)
      break;

    p1 = p2;
  }

  // Set the arrow
  double angle = ::acos(pathSegment.dx() / pathSegment.length());

  if (pathSegment.dy() >= 0)
    angle = TwoPi - angle;

  label->setPos(path.pointAtPercent(0.5));

  prepareGeometryChange();
  _path = path;
  _head_arrow = toArrow(angle, intersectPoint);
}

QPolygonF QGVEdge::toArrow(const qreal angle, const QPointF& point) const {
  QPointF dstVertexArrowP1 = point + QPointF(sin(angle - Pi / 3) * arrowSize,
      cos(angle - Pi / 3) * arrowSize);
  QPointF dstVertexArrowP2 = point + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
      cos(angle - Pi + Pi / 3) * arrowSize);

  return QPolygonF() << point << dstVertexArrowP1 << dstVertexArrowP2;
}







