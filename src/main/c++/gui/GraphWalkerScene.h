#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QtGui>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include "VertexItem.h"

class GraphWalkerScene : public QGraphicsScene {
    Q_OBJECT

  public:

    GraphWalkerScene ( QObject* parent = 0 );

    void loadGraph ( const QFileInfo& file_name );

  public slots:
    void hierarchicalLayout();

  private:
    void populateGraphFromScene();
    void populateSceneFromGraph();
    VertexItem* getNode ( ogdf::node source );

    ogdf::GraphAttributes GA;
    ogdf::Graph G;
};

#endif
