// #include <QtGui>
// #include <boost/exception/all.hpp>
// #include <math.h>
//
// #include "EdgeItem.h"
// #include "VertexItem.h"
//
// static const double Pi = 3.14159265358979323846264338327950288419717;
// static double TwoPi = 2.0 * Pi;
//
// EdgeItem::EdgeItem(VertexItem* startItem, VertexItem* endItem)
//   : arrowSize(10) {
//   qDebug() << Q_FUNC_INFO;
//
//   setAcceptedMouseButtons(0);
//
//   if (startItem == NULL || endItem == NULL) {
//     throw;
//   }
//
//   srcVertex = startItem;
//   dstVertex = endItem;
//   srcVertex->addEdgeItem(this);
//   dstVertex->addEdgeItem(this);
//   adjust();
//
//   setZValue(-1);
//   label->setParentItem(this);
// }
//
// void EdgeItem::adjust() {
//   if (!srcVertex || !dstVertex)
//     return;
//
//   QLineF line(mapFromItem(srcVertex, 0, 0), mapFromItem(dstVertex, 0, 0));
//   qreal length = line.length();
//
//   prepareGeometryChange();
//
//   if (length > qreal(20.)) {
//     QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
//     srcPoint = line.p1() + edgeOffset;
//     dstPoint = line.p2() - edgeOffset;
//
//   }
//   else {
//     srcPoint = dstPoint = line.p1();
//   }
//
//   label->setPos(shape().pointAtPercent(0.5));
// }
//
// QPainterPath EdgeItem::shape() const {
//   QPainterPath path;
//
//   if (!srcVertex || !dstVertex)
//     return path;
//
//   path.moveTo(srcPoint);
//
//   foreach (QPointF p, bends) {
//     path.lineTo(p);
//   }
//
//   path.lineTo(dstPoint);
//   return path;
// }
//
// QRectF EdgeItem::boundingRect() const {
//   if (!srcVertex || !dstVertex)
//     return QRectF();
//
//   // Not perfect, it does not always cover the arrow heads.
//   return shape().boundingRect();
// }
//
// void EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
//   if (!srcVertex || !dstVertex)
//     return;
//
//   QLineF line(srcPoint, dstPoint);
//
//   if (qFuzzyCompare(line.length(), qreal(0.)))
//     return;
//
//   // Draw the line itself
//   painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//
//
//   QVector<QPointF> polyPoints;
//   polyPoints << srcPoint;
//
//   foreach (QPointF p, bends) {
//     polyPoints << p;
//   }
//
//   polyPoints << dstPoint;
//   painter->drawPolyline(polyPoints);
//
//   // Last line segment
//   QLineF lastSegment(polyPoints[polyPoints.size() - 2], polyPoints[polyPoints.size() - 1]);
//
//   // Find intersection point
//   QPolygonF endPolygon = dstVertex->shape().toFillPolygon();
//   QPointF p1 = endPolygon.first() + mapFromItem(dstVertex, 0, 0);
//   QPointF p2;
//   QPointF intersectPoint;
//   QLineF polyLine;
//
//   for (int i = 1; i < endPolygon.count(); ++i) {
//     p2 = endPolygon.at(i) + mapFromItem(dstVertex, 0, 0);
//     polyLine = QLineF(p1, p2);
//     QLineF::IntersectType intersectType = polyLine.intersect(lastSegment, &intersectPoint);
//
//     if (intersectType == QLineF::BoundedIntersection)
//       break;
//
//     p1 = p2;
//   }
//
//   // Draw the arrows
//   double angle = ::acos(lastSegment.dx() / lastSegment.length());
//
//   if (lastSegment.dy() >= 0)
//     angle = TwoPi - angle;
//
//   QPointF dstVertexArrowP1 = intersectPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
//       cos(angle - Pi / 3) * arrowSize);
//   QPointF dstVertexArrowP2 = intersectPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
//       cos(angle - Pi + Pi / 3) * arrowSize);
//
//   painter->setBrush(Qt::black);
//   painter->drawPolygon(QPolygonF() << intersectPoint << dstVertexArrowP1 << dstVertexArrowP2);
//
//   painter->setPen(QPen(Qt::red, 1));
//   painter->drawPoint(intersectPoint);
// }
