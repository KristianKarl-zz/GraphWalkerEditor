#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "GraphicItem.h"
#include "VertexItem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class LabelItem;

class EdgeItem : public GraphicItem, public QGraphicsLineItem {
  public:
    EdgeItem ( VertexItem* startItem, VertexItem* endItem,
               QGraphicsItem* parent = 0, QGraphicsScene* scene = 0 );

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor ( const QColor& color ) {
      myColor = color;
    }
    VertexItem* startItem() const {
      return myStartItem;
    }
    VertexItem* endItem() const {
      return myEndItem;
    }

    void updatePosition();

  protected:
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event );
    void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );
    void paint ( QPainter* painter, const QStyleOptionGraphicsItem* option,
                 QWidget* widget = 0 );

  private:
    VertexItem* myStartItem;
    VertexItem* myEndItem;
    QPolygonF arrowHead;
};

#endif
