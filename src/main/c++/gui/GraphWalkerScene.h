#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QtGui>

#include "VertexItem.h"

class GraphWalkerScene : public QGraphicsScene {
    Q_OBJECT

  public:

    GraphWalkerScene(QObject* parent = 0);

    void loadGraph(const QFileInfo& file_name);

  public slots:
    void hierarchicalLayout();
    void energyBasedLayout();
    void orthogonalLayout();

  private:
    void populateGraphFromScene();
    void populateSceneFromGraph();
    //VertexItem* getNode ( QGVNode* source );

//     QList<QGVNode*> _nodes;
//     QList<QGVEdge*> _edges;
};

#endif
