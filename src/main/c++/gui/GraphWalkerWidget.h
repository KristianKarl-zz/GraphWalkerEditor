#ifndef GRAPHWALKERWIDGET_H
#define GRAPHWALKERWIDGET_H

#include <QGraphicsView>

class GraphWalkerScene;

class GraphWalkerWidget : public QGraphicsView {
    Q_OBJECT

  public:
    GraphWalkerWidget ( GraphWalkerScene* );

  private:
    GraphWalkerScene* scene;
};

#endif // GRAPHWALKERWIDGET_H
