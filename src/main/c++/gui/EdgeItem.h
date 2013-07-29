#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

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

class EdgeItem : public QGraphicsLineItem {
  public:
    enum { Type = UserType + 4 };

    EdgeItem ( VertexItem* startItem, VertexItem* endItem,
               QGraphicsItem* parent = 0, QGraphicsScene* scene = 0 );

    int type() const {
      return Type;
    }
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
    void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );
    void paint ( QPainter* painter, const QStyleOptionGraphicsItem* option,
                 QWidget* widget = 0 );

  private:
    VertexItem* myStartItem;
    VertexItem* myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
    LabelItem* label;
};

#endif
