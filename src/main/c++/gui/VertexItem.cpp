#include <QtGui>

#include "VertexItem.h"
#include "EdgeItem.h"
#include "GraphWalkerWidget.h"

VertexItem::VertexItem(ogdf::node n)
  : ogdf_node(n) {
  qDebug() << Q_FUNC_INFO;

  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  setCacheMode(DeviceCoordinateCache);
  setZValue(-1);
  label->setParentItem(this);
}

void VertexItem::removeEdgeItem(EdgeItem* edge) {
  qDebug() << Q_FUNC_INFO;
  int index = edges.indexOf(edge);

  if (index != -1)
    edges.removeAt(index);
}

void VertexItem::removeEdgeItems() {
  qDebug() << Q_FUNC_INFO;

  foreach (EdgeItem * edge, edges) {
    edge->startItem()->removeEdgeItem(edge);
    edge->endItem()->removeEdgeItem(edge);
    scene()->removeItem(edge);
    delete edge;
  }
}

void VertexItem::addEdgeItem(EdgeItem* edge) {
  qDebug() << Q_FUNC_INFO;
  edges.append(edge);
}

ogdf::node VertexItem::get_ogdf_node() {
  return ogdf_node;
}

QRectF VertexItem::boundingRect() const {
  QRectF rect = label->boundingRect();
  rect.moveTo(-rect.width()/2, -rect.height()/2);
  return rect;
}

QPainterPath VertexItem::shape() const {
  QPainterPath path;
  path.addRoundedRect(boundingRect(), 10, 10);
  return path;
}

void VertexItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) {
  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::darkGray);
  painter->drawPolygon(shape().toFillPolygon());

  QRadialGradient gradient(-3, -3, 100);

  if (option->state & QStyle::State_Sunken) {
    gradient.setCenter(3, 3);
    gradient.setFocalPoint(3, 3);

    if (getKeyWords() & GraphWalker::START_NODE) {
      gradient.setColorAt(1, QColor(Qt::green).light(120));
      gradient.setColorAt(0, QColor(Qt::darkGreen).light(120));
    }
    else {
      gradient.setColorAt(1, QColor(Qt::yellow).light(120));
      gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    }
  }
  else {
    if (getKeyWords() & GraphWalker::START_NODE) {
      gradient.setColorAt(0, Qt::green);
      gradient.setColorAt(1, Qt::darkGreen);
    }
    else {
      gradient.setColorAt(0, Qt::yellow);
      gradient.setColorAt(1, Qt::darkYellow);
    }
  }

  painter->setBrush(gradient);
  painter->setPen(QPen(Qt::black, 0));
  painter->drawPolygon(shape().toFillPolygon());
}

QVariant VertexItem::itemChange(GraphicsItemChange change, const QVariant& value) {
  qDebug() << Q_FUNC_INFO;

  switch (change) {
    case ItemPositionHasChanged:
      foreach (EdgeItem * edge, edges)
        edge->adjust();

      break;

    default
        :
      break;
  };

  return QGraphicsItem::itemChange(change, value);
}

void VertexItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  qDebug() << Q_FUNC_INFO;
  update();
  QGraphicsItem::mousePressEvent(event);
}

void VertexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  qDebug() << Q_FUNC_INFO;
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}

void VertexItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
  qDebug() << Q_FUNC_INFO;

  if (getKeyWords() & GraphWalker::START_NODE)
    return;

  label->mouseDoubleClickEvent(event);
}

