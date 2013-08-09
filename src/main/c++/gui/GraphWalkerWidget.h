#ifndef GRAPHWALKERWIDGET_H
#define GRAPHWALKERWIDGET_H

#include <QtGui/QGraphicsView>
#include <QtCore/QFileInfo>

class GraphWalkerScene;

class GraphWalkerWidget : public QGraphicsView {

    Q_OBJECT

  public:
    GraphWalkerWidget(QWidget* parent = 0);

    void loadGraph(const QFileInfo& graph_file);

    GraphWalkerScene* getScene() {
      return scene;
    }

  public slots:
    void zoomIn();
    void zoomOut();

  protected:
    void wheelEvent(QWheelEvent* event);
    void scaleView(qreal scaleFactor);

    GraphWalkerScene* scene;
};

#endif // GRAPHWALKERWIDGET_H
