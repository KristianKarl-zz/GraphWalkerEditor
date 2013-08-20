#include <QGVNode.h>
#include <QGVScene.h>
#include <QtGui>
#include <QDebug>


QGVNode::QGVNode(Agnode_t* node, QGVScene* scene) : _node(node), _scene(scene) {
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  //setCacheMode(DeviceCoordinateCache);
  setZValue(1);
  label->setParentItem(this);
  connect(label, SIGNAL(labelIsUpdated(LabelItem*)), this, SLOT(labelIsUpdated(LabelItem*)));
}

QGVNode::~QGVNode() {
  _scene->removeItem(this);
}

QString QGVNode::getLabel() const {
  return getAttribute("label");
}

void QGVNode::setLabel(const QString& str) {
  GraphicItem::setLabel(str);

  if (str.compare("Start", Qt::CaseInsensitive) == 0) {
    setKeyWord(GraphWalker::START_NODE);
  }

  setAttribute("label", str);
  QPainterPath p;
  p.addEllipse(label->boundingRect());
  _path = p;
}

QRectF QGVNode::boundingRect() const {
  return _path.boundingRect();
}

void QGVNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  painter->save();

  painter->setPen(Qt::NoPen);
  painter->drawPolygon(shape().toFillPolygon());

  QColor color;

  if (option->state & QStyle::State_Sunken) {

    if (getKeyWords() & GraphWalker::START_NODE) {
      color = QColor(Qt::green).lighter(70);
    }
    else {
      color = QColor(Qt::yellow).lighter(70);
    }
  }
  else {
    if (getKeyWords() & GraphWalker::START_NODE) {
      color = Qt::green;
    }
    else {
      color = Qt::yellow;
    }
  }

  painter->setBrush(color);
  painter->setPen(QPen(Qt::black, 0));
  painter->drawPolygon(shape().toFillPolygon());
  painter->restore();
}

void QGVNode::setAttribute(const QString& name, const QString& value) {
  agsafeset(_node, name.toLocal8Bit().data(), value.toLocal8Bit().data(), "");
}

QString QGVNode::getAttribute(const QString& name) const {
  char* value = agget(_node, name.toLocal8Bit().data());

  if (value)
    return value;

  return QString();
}

void QGVNode::setIcon(const QImage& icon) {
  _icon = icon;
}

QPainterPath QGVNode::shape() const {
  return _path;
}

void QGVNode::labelIsUpdated(LabelItem*) {
  setLabel(label->getLabel());

  foreach (QGVEdge * edge, edges)
    edge->adjust();
}

void QGVNode::updateLayout() {
  qDebug() << Q_FUNC_INFO << QGVNode::getLabel();
  prepareGeometryChange();

  qreal width = ND_width(_node) * DotDefaultDPI;
  qreal height = ND_height(_node) * DotDefaultDPI;
  qDebug() << Q_FUNC_INFO << "width;" << width;
  qDebug() << Q_FUNC_INFO << "height:" << height;

  qDebug() << Q_FUNC_INFO << "attribute pos" << getAttribute("pos");
  //Node Position (center)
  qreal gheight = QGVCore::graphHeight(_scene->_graph);
  setPos(QGVCore::centerToOrigin(QGVCore::toPoint(ND_coord(_node), gheight), width, height));
  qDebug() << Q_FUNC_INFO << "pos:" << pos();

  //Node on top
  setZValue(1);

  //Node path
  _path = QGVCore::toPath(ND_shape(_node)->name, (polygon_t*)ND_shape_info(_node), width, height);
  _pen.setWidth(1);

  _brush.setStyle(QGVCore::toBrushStyle(getAttribute("style")));
  _brush.setColor(QGVCore::toColor(getAttribute("fillcolor")));
  _pen.setColor(QGVCore::toColor(getAttribute("color")));

  label->setPos(8, 5);

  setToolTip(getDescription());
}

QVariant QGVNode::itemChange(GraphicsItemChange change, const QVariant& value) {
  switch (change) {
    case ItemPositionHasChanged:

      foreach (QGVEdge * edge, edges)
        edge->adjust();

      break;

    default
        :
      break;
  };

  return QGraphicsItem::itemChange(change, value);
}

QPointF QGVNode::getCenter() {
  return QPointF(pos().x() + boundingRect().width() / 2, pos().y() + boundingRect().height() / 2);
}

void QGVNode::removeEdgeItem(QGVEdge* edge) {
  qDebug() << Q_FUNC_INFO << QGVNode::getLabel();
  int index = edges.indexOf(edge);

  if (index != -1)
    edges.removeAt(index);
}

void QGVNode::removeEdgeItems() {
  qDebug() << Q_FUNC_INFO << QGVNode::getLabel();

  foreach (QGVEdge * edge, edges) {
    edge->srcNode()->removeEdgeItem(edge);
    edge->dstNode()->removeEdgeItem(edge);
    scene()->removeItem(edge);
    delete edge;
  }
}

void QGVNode::addEdgeItem(QGVEdge* edge) {
  qDebug() << Q_FUNC_INFO;
  edges.append(edge);
}

void QGVNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
  qDebug() << Q_FUNC_INFO;

  if (getKeyWords() & GraphWalker::START_NODE)
    return;

  label->mouseDoubleClickEvent(event);
  QGraphicsItem::mouseDoubleClickEvent(event);
}


// void QGVNode::mousePressEvent(QGraphicsSceneMouseEvent* event) {
//   qDebug() << Q_FUNC_INFO;
//    update();
//   QGraphicsItem::mousePressEvent(event);
// }
//
// void QGVNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
//   qDebug() << Q_FUNC_INFO;
//    update();
//   QGraphicsItem::mouseReleaseEvent(event);
// }


