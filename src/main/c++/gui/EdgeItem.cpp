#include <QtGui>
#include <boost/exception/all.hpp> 
#include <math.h>

#include "EdgeItem.h"
#include "VertexItem.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

EdgeItem::EdgeItem(VertexItem* startItem, VertexItem* endItem)
  : arrowSize(10) {

  setAcceptedMouseButtons(0);

  if (startItem == NULL || endItem == NULL) {
    throw;
  }

  srcVertex = startItem;
  dstVertex = endItem;
  srcVertex->addEdgeItem(this);
  dstVertex->addEdgeItem(this);
  adjust();
}

void EdgeItem::adjust() {
  if (!srcVertex || !dstVertex)
    return;

  QLineF line(mapFromItem(srcVertex, 0, 0), mapFromItem(dstVertex, 0, 0));
  qreal length = line.length();

  prepareGeometryChange();

  if (length > qreal(20.)) {
    QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
    srcPoint = line.p1() + edgeOffset;
    dstPoint = line.p2() - edgeOffset;

  } else {
    srcPoint = dstPoint = line.p1();
  }
}

QRectF EdgeItem::boundingRect() const {
  if (!srcVertex || !dstVertex)
    return QRectF();

  qreal penWidth = 1;
  qreal extra = (penWidth + arrowSize) / 2.0;

  return QRectF(srcPoint, QSizeF(dstPoint.x() - srcPoint.x(),
                                 dstPoint.y() - srcPoint.y()))
         .normalized()
         .adjusted(-extra, -extra, extra, extra);
}

void EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
  if (!srcVertex || !dstVertex)
    return;

  QLineF line(srcPoint, dstPoint);

  if (qFuzzyCompare(line.length(), qreal(0.)))
    return;

  // Draw the line itself
  painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->drawLine(line);

  // Draw the arrows
  double angle = ::acos(line.dx() / line.length());

  if (line.dy() >= 0)
    angle = TwoPi - angle;

  QPointF srcVertexArrowP1 = srcPoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                             cos(angle + Pi / 3) * arrowSize);
  QPointF srcVertexArrowP2 = srcPoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                             cos(angle + Pi - Pi / 3) * arrowSize);
  QPointF dstVertexArrowP1 = dstPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                             cos(angle - Pi / 3) * arrowSize);
  QPointF dstVertexArrowP2 = dstPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                             cos(angle - Pi + Pi / 3) * arrowSize);

  painter->setBrush(Qt::black);
  painter->drawPolygon(QPolygonF() << line.p1() << srcVertexArrowP1 << srcVertexArrowP2);
  painter->drawPolygon(QPolygonF() << line.p2() << dstVertexArrowP1 << dstVertexArrowP2);
}
