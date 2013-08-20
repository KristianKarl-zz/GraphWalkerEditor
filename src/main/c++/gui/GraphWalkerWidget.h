#ifndef GRAPHWALKERWIDGET_H
#define GRAPHWALKERWIDGET_H

#include <QtGui/QGraphicsView>
#include <QtCore/QFileInfo>

class QGVScene;

class GraphWalkerWidget : public QGraphicsView {

    Q_OBJECT

  public:
    GraphWalkerWidget(QWidget* parent = 0);

    QGVScene* getScene() {
      return scene;
    }

  public slots:
    void zoomIn();
    void zoomOut();

  protected:
    void wheelEvent(QWheelEvent* event);
    void scaleView(qreal scaleFactor);

    QGVScene* scene;
};

#endif // GRAPHWALKERWIDGET_H
