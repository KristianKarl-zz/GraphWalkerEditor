#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QtGui>

#include <ogdf/basic/Graph.h>

#include "GraphWalker.h"

class EdgeItem;
class LabelItem;

class VertexItem : public QObject, public QGraphicsPolygonItem {
    Q_OBJECT

  public:
    enum { Type = UserType + 15 };

    VertexItem ( ogdf::node n, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0 );
    VertexItem ( const QString& label,  QGraphicsItem* parent = 0, QGraphicsScene* scene = 0 );

    void removeEdgeItem ( EdgeItem* edge );
    void removeEdgeItems();
    void addEdgeItem ( EdgeItem* edge );
    QList<EdgeItem*> getEdges() {
      return edges;
    }

    ogdf::node get_ogdf_node();
    void set_ogdf_node(ogdf::node n) {
      ogdf_node = n;
    }

    void setLabel ( const QString& );
    QString getLabel();

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
    void init ( const QString& );
    QAction* blockAction;
    QAction* switchModelAction;

    ogdf::node ogdf_node;
    QPolygonF myPolygon;
    LabelItem* label;
    int keywords;
    QList<EdgeItem*> edges;
    bool blocked;
    bool switchModel;
};

#endif
