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

void GraphWalkerWidget::drawBackground(QPainter* painter, const QRectF& rect) {
  Q_UNUSED(rect);

  // Shadow
  QRectF sceneRect = this->sceneRect();
  QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
  QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);

//   if (rightShadow.intersects(rect) || rightShadow.contains(rect))
//     painter->fillRect(rightShadow, Qt::darkGray);
//
//   if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
//     painter->fillRect(bottomShadow, Qt::darkGray);

  // Fill
//   QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
//   gradient.setColorAt(0, Qt::white);
//   gradient.setColorAt(1, Qt::lightGray);
//   painter->fillRect(rect.intersect(sceneRect), gradient);
//   painter->setBrush(Qt::NoBrush);
//   painter->drawRect(sceneRect);

//   // Text
//   QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
//                   sceneRect.width() - 4, sceneRect.height() - 4);
//   QString message(tr("Click and drag the nodes around, and zoom with the mouse "
//                      "wheel or the '+' and '-' keys"));
//
//   QFont font = painter->font();
//   font.setBold(true);
//   font.setPointSize(14);
//   painter->setFont(font);
//   painter->setPen(Qt::lightGray);
//   painter->drawText(textRect.translated(2, 2), message);
//   painter->setPen(Qt::black);
//   painter->drawText(textRect, message);
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

