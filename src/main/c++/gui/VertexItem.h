#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QtGui>

#include <ogdf/basic/Graph.h>

#include "GraphicItem.h"

class EdgeItem;
class LabelItem;

class VertexItem : public GraphicItem, public QGraphicsPolygonItem {
    Q_OBJECT

  public:
    VertexItem ( ogdf::node n, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0 );
    VertexItem ( const QString& label,  QGraphicsItem* parent = 0, QGraphicsScene* scene = 0 );

    void removeEdgeItem ( EdgeItem* edge );
    void removeEdgeItems();
    void addEdgeItem ( EdgeItem* edge );
    QList<EdgeItem*> getEdges() {
      return edges;
    }

    ogdf::node get_ogdf_node();
    void set_ogdf_node ( ogdf::node n ) {
      ogdf_node = n;
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
    void hoverEnterEvent ( QGraphicsSceneHoverEvent* event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent* event );

    void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* );
    void contextMenuEvent ( QPoint );
    QVariant itemChange ( GraphicsItemChange change, const QVariant& value );
    void paint ( QPainter* painter, const QStyleOptionGraphicsItem* option,
                 QWidget* widget = 0 );
    QPainterPath shape () const;

  private:
    void init ();
    QAction* blockAction;
    QAction* switchModelAction;

    ogdf::node ogdf_node;
    QPolygonF myPolygon;
    QList<EdgeItem*> edges;
    bool blocked;
    bool switchModel;
};

#endif
