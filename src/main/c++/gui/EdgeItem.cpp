#include <QtGui>

#include <math.h>

#include "EdgeItem.h"

const qreal Pi = 3.14;

EdgeItem::EdgeItem ( VertexItem* startItem, VertexItem* endItem,
    QGraphicsItem* parent, QGraphicsScene* scene )
  : GraphicItem(), QGraphicsLineItem ( parent, scene ) {
  setFlag ( QGraphicsItem::ItemIsSelectable, true );
  myStartItem = startItem;
  myEndItem = endItem;
  setZValue ( -1000.0 );
  label->setParentItem ( this );
}

QRectF EdgeItem::boundingRect() const {
  qreal extra = ( pen().width() + 20 ) / 2.0;

  QRectF rect ( line().p1(), QSizeF ( line().p2().x() - line().p1().x(),
                line().p2().y() - line().p1().y() ) );
  rect.normalized().adjusted ( -extra, -extra, extra, extra );

  label->setPos ( rect.center() );

  return rect;
}

QPainterPath EdgeItem::shape() const {
  QPainterPath path = QGraphicsLineItem::shape();
  path.addPolygon ( arrowHead );
  return path;
}

void EdgeItem::updatePosition() {
  QLineF line ( mapFromItem ( myStartItem, 0, 0 ), mapFromItem ( myEndItem, 0, 0 ) );
  setLine ( line );
}

void EdgeItem::paint ( QPainter* painter, const QStyleOptionGraphicsItem*,
    QWidget* ) {
  if ( myStartItem->collidesWithItem ( myEndItem ) )
    return;

  if ( myEndItem->polygon().isEmpty() )
    return;

  QPen myPen = pen();
  myPen.setColor ( myColor );
  qreal arrowSize = 20;
  painter->setPen ( myPen );
  painter->setBrush ( myColor );

  QLineF centerLine ( myStartItem->pos(), myEndItem->pos() );
  QPolygonF endPolygon = myEndItem->polygon();
  QPointF p1 = endPolygon.first() + myEndItem->pos();
  QPointF p2;
  QPointF intersectPoint;
  QLineF polyLine;

  for ( int i = 1; i < endPolygon.count(); ++i ) {
    p2 = endPolygon.at ( i ) + myEndItem->pos();
    polyLine = QLineF ( p1, p2 );
    QLineF::IntersectType intersectType =
      polyLine.intersect ( centerLine, &intersectPoint );

    if ( intersectType == QLineF::BoundedIntersection )
      break;

    p1 = p2;
  }

  setLine ( QLineF ( intersectPoint, myStartItem->pos() ) );

  double angle = ::acos ( line().dx() / line().length() );

  if ( line().dy() >= 0 )
    angle = ( Pi * 3 ) - angle;

  QPointF arrowP1 = line().p1() + QPointF ( sin ( angle + Pi / 3 ) * arrowSize,
      cos ( angle + Pi / 3 ) * arrowSize );
  QPointF arrowP2 = line().p1() + QPointF ( sin ( angle + Pi - Pi / 3 ) * arrowSize,
      cos ( angle + Pi - Pi / 3 ) * arrowSize );

  arrowHead.clear();
  arrowHead << line().p1() << arrowP1 << arrowP2;

  if ( isSelected() ) {
    setPen ( QPen ( myColor, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
  }
  else {
    setPen ( QPen ( myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
  }

  QVector<QPointF> polyPoints;
  polyPoints << myStartItem->pos();
  foreach( QPointF p, bends ) {
    polyPoints << p;
  }
  polyPoints << myEndItem->pos();
  painter->drawPolyline( polyPoints );
  painter->drawPolygon ( arrowHead );
}

void EdgeItem::mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent ) {
  qDebug() << Q_FUNC_INFO << getLabel();

  if ( mouseEvent->button() == Qt::RightButton ) {
    return;
  }
  else if ( mouseEvent->button() == Qt::LeftButton ) {
    setSelected ( true );
  }

  update();
}

void EdgeItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event ) {
  qDebug() << Q_FUNC_INFO << getLabel();

  if ( event->button() != Qt::LeftButton )
    return;

  label->mouseDoubleClickEvent ( event );
}

