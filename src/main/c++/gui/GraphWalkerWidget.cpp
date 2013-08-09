#include "GraphWalkerWidget.h"
#include "GraphWalkerScene.h"

#include <QtGui>

#include <math.h>

GraphWalkerWidget::GraphWalkerWidget(QWidget* parent)
  : QGraphicsView(parent) {

  scene = new GraphWalkerScene(this);
  scene->setItemIndexMethod(GraphWalkerScene::NoIndex);
  scene->setSceneRect(-200, -200, 400, 400);
  setScene(scene);
  setCacheMode(CacheBackground);
  setViewportUpdateMode(BoundingRectViewportUpdate);
  setRenderHint(QPainter::Antialiasing);
  setTransformationAnchor(AnchorUnderMouse);
  scale(qreal(0.8), qreal(0.8));
  setMinimumSize(400, 400);
  setWindowTitle(tr("GraphWalker Editor"));
}

void GraphWalkerWidget::loadGraph(const QFileInfo& graph_file) {
  scene->loadGraph(graph_file);
}

void GraphWalkerWidget::wheelEvent(QWheelEvent* event) {
  scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphWalkerWidget::scaleView(qreal scaleFactor) {
  qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();

  if (factor < 0.07 || factor > 100)
    return;

  scale(scaleFactor, scaleFactor);
}

void GraphWalkerWidget::zoomIn() {
  scaleView(qreal(1.2));
}

void GraphWalkerWidget::zoomOut() {
  scaleView(1 / qreal(1.2));
}

