#include <QtGui>

#include "VertexItem.h"
#include "EdgeItem.h"
#include "GraphWalkerWidget.h"

VertexItem::VertexItem(ogdf::node n)
  : ogdf_node(n) {

  setFlag(ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);
  setCacheMode(DeviceCoordinateCache);
  setZValue(-1);
}

void VertexItem::removeEdgeItem(EdgeItem* edge) {
  int index = edges.indexOf(edge);

  if (index != -1)
    edges.removeAt(index);
}

void VertexItem::removeEdgeItems() {
  foreach (EdgeItem * edge, edges) {
    edge->startItem()->removeEdgeItem(edge);
    edge->endItem()->removeEdgeItem(edge);
    scene()->removeItem(edge);
    delete edge;
  }
}

void VertexItem::addEdgeItem(EdgeItem* edge) {
  edges.append(edge);
}

ogdf::node VertexItem::get_ogdf_node() {
  return ogdf_node;
}

QRectF VertexItem::boundingRect() const {
  qreal adjust = 2;
  return QRectF(-10 - adjust, -10 - adjust,
                23 + adjust, 23 + adjust);
}

QPainterPath VertexItem::shape() const {
  QPainterPath path;
  path.addEllipse(-10, -10, 20, 20);
  return path;
}

void VertexItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) {
  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::darkGray);
  painter->drawEllipse(-7, -7, 20, 20);

  QRadialGradient gradient(-3, -3, 10);

  if (option->state & QStyle::State_Sunken) {
    gradient.setCenter(3, 3);
    gradient.setFocalPoint(3, 3);
    gradient.setColorAt(1, QColor(Qt::yellow).light(120));
    gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));

  } else {
    gradient.setColorAt(0, Qt::yellow);
    gradient.setColorAt(1, Qt::darkYellow);
  }

  painter->setBrush(gradient);

  painter->setPen(QPen(Qt::black, 0));
  painter->drawEllipse(-10, -10, 20, 20);
}

QVariant VertexItem::itemChange(GraphicsItemChange change, const QVariant& value) {
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
  update();
  QGraphicsItem::mousePressEvent(event);
}

void VertexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}
