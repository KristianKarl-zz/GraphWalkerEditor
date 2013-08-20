#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

// #include <QGraphicsItem>
//
// #include "GraphicItem.h"
//
// class EdgeItem;

// class VertexItem : public GraphicItem, public QGraphicsItem {
//   public:
//     VertexItem();
//
//     void removeEdgeItem(EdgeItem* edge);
//     void removeEdgeItems();
//     void addEdgeItem(EdgeItem* edge);
//     QList<EdgeItem*> getEdges() {
//       return edges;
//     }
//
//     enum { Type = UserType + 1 };
//     int type() const {
//       return Type;
//     }
//
//     ogdf::node get_ogdf_node();
//     void set_ogdf_node(ogdf::node n) {
//       ogdf_node = n;
//     }
//
//     QRectF boundingRect() const;
//     QPainterPath shape() const;
//     void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
//
//   protected:
//     QVariant itemChange(GraphicsItemChange change, const QVariant& value);
//
//     void mousePressEvent(QGraphicsSceneMouseEvent* event);
//     void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
//     void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
//
//   private:
//     ogdf::node ogdf_node;
//     QList<EdgeItem*> edges;
//     bool switchModel;
// };

#endif
