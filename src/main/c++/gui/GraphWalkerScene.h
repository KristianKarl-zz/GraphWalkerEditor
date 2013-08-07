#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QtGui>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include "VertexItem.h"

class GraphWalkerScene : public QGraphicsScene {
    Q_OBJECT

  public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    GraphWalkerScene ( QObject* parent = 0 );
    QFont font() const {
      return myFont;
    }
    QColor textColor() const {
      return myTextColor;
    }
    QColor itemColor() const {
      return myItemColor;
    }
    QColor lineColor() const {
      return myLineColor;
    }
    void setLineColor ( const QColor& color );
    void setTextColor ( const QColor& color );
    void setItemColor ( const QColor& color );
    void setFont ( const QFont& font );

    void loadGraph ( const QFileInfo& file_name );

  public slots:
    void setMode ( Mode mode );
    void hierarchicalLayout();
    void newGraph();

  signals:
    void itemInserted ( VertexItem* item );
    void itemSelected ( QGraphicsItem* item );

  protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent* mouseEvent );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent* mouseEvent );

  private:
    void populateGraphFromScene();
    void populateSceneFromGraph();
    VertexItem* getNode ( ogdf::node source );

    ogdf::GraphAttributes GA;
    ogdf::Graph G;

    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem* line;
    QFont myFont;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
};

#endif
