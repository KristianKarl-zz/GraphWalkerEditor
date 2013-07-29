#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QtGui>

#include "GraphWalker.h"

class EdgeItem;
class LabelItem;

class VertexItem : public QObject, public QGraphicsPolygonItem {
    Q_OBJECT

  public:
    enum { Type = UserType + 15 };

    VertexItem ( QGraphicsItem* parent = 0, QGraphicsScene* scene = 0 );
    VertexItem ( const QString& label,  QGraphicsItem* parent = 0, QGraphicsScene* scene = 0 );

    void removeEdgeItem ( EdgeItem* edge );
    void removeEdgeItems();
    void addEdgeItem ( EdgeItem* edge );
    QList<EdgeItem*> getEdges() {
      return edges;
    }

    void setKeyWord ( const GrapwWalker::Keywords& );
    void addKeyWord ( const GrapwWalker::Keywords& );
    int getKeyWords () {
      return keywords;
    }
    int type() const {
      return Type;
    }

    /**
     * Used by the edges to calculate where the arrow heads
     * should end.
     */
    QPolygonF polygon() const {
      return myPolygon;
    }

    QRectF boundingRect () const;

  public slots:
    void toggleBlocked();
    void toggleSwitchModel();

  protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* );
    void contextMenuEvent ( QPoint );
    QVariant itemChange ( GraphicsItemChange change, const QVariant& value );
    void paint ( QPainter* painter, const QStyleOptionGraphicsItem* option,
                 QWidget* widget = 0 );
    QPainterPath shape () const;

  private:
    void init ( const QString& );
    QAction* blockAction;
    QAction* switchModelAction;

    QPolygonF myPolygon;
    LabelItem* label;
    int keywords;
    QList<EdgeItem*> edges;
    bool blocked;
    bool switchModel;
};

#endif
