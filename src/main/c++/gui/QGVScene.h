#ifndef QGVSCENE_H
#define QGVSCENE_H

#include<QtCore/QFileInfo>

#include <QGVCore.h>
#include <QGraphicsScene>
#include <boost/graph/graph_concepts.hpp>
#include <QGVNode.h>
#include <QGVEdge.h>


/**
 * @brief GraphViz interactive scene
 *
 */
class QGVScene : public QGraphicsScene {
    Q_OBJECT
  public:

    explicit QGVScene(QObject* parent = 0);
    ~QGVScene();

    bool loadGraph(const QFileInfo& file_name);

    void setGraphAttribute(const QString& name, const QString& value);
    void setNodeAttribute(const QString& name, const QString& value);
    void setEdgeAttribute(const QString& name, const QString& value);

    QGVNode* addNode(const QString& label);
    QGVEdge* addEdge(QGVNode* source, QGVNode* target, const QString& label = QString());

    void setRootNode(QGVNode* node);

    void applyLayout();
    void clear();


  signals:
    void nodeContextMenu(QGVNode* node);
    void nodeDoubleClick(QGVNode* node);

    void edgeContextMenu(QGVEdge* edge);
    void edgeDoubleClick(QGVEdge* edge);

    void graphContextMenuEvent();

  public slots:
    void autoLayout();

  protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* contextMenuEvent);
//     virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);

  private:
    void init();

    friend class QGVNode;
    friend class QGVEdge;

    GVC_t* _context;
    Agraph_t* _graph;
    //QFont _font;

    QList<QGVNode*> _nodes;
    QList<QGVEdge*> _edges;

    QGraphicsLineItem* line;
};

#endif // QGVSCENE_H
