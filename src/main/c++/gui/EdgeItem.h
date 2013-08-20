// #ifndef ARROW_H
// #define ARROW_H
//
// #include <QtGui/QGraphicsItem>
//
// #include "GraphicItem.h"
//
// class VertexItem;
//
// class EdgeItem : public GraphicItem, public QGraphicsItem {
//   public:
//     EdgeItem(VertexItem* startItem, VertexItem* endItem);
//
//     VertexItem* startItem() const {
//       return srcVertex;
//     }
//     VertexItem* endItem() const {
//       return dstVertex;
//     }
//     void adjust();
//
//     enum { Type = UserType + 2 };
//     int type() const {
//       return Type;
//     }
//
//     QList<QPointF>& getBends() {
//       return bends;
//     }
//
//   protected:
//     QRectF boundingRect() const;
//     QPainterPath shape() const;
//     void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
//
//   private:
//     VertexItem* srcVertex;
//     VertexItem* dstVertex;
//
//     QPointF srcPoint;
//     QPointF dstPoint;
//     qreal arrowSize;
//     QList<QPointF> bends;
// };
//
// #endif
