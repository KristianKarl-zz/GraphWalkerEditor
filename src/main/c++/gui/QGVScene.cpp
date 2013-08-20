#include "QGVScene.h"
#include "GraphWalker.h"

#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>

QGVScene::QGVScene(QObject* parent) : QGraphicsScene(parent), line(0) {
  qDebug() << Q_FUNC_INFO;
  init();
}

QGVScene::~QGVScene() {
  qDebug() << Q_FUNC_INFO;
  clear();
}

void QGVScene::init() {
  _context = gvContext();
  _graph = agopen(QString("GraphWalker").toLocal8Bit().data(), Agdirected, NULL);
  //setGraphAttribute("fontname", QFont().family());
  //setGraphAttribute("label", "DEMO");

  setGraphAttribute("splines", "true");
  setGraphAttribute("remincross", "true");
  setGraphAttribute("overlap", "false");
  setGraphAttribute("rankdir", "TB");
  //setGraphAttribute("concentrate", "true"); //Error !
  setGraphAttribute("nodesep", "2.0");

  setNodeAttribute("shape", "ellipse");
  setNodeAttribute("style", "filled");
  setNodeAttribute("fillcolor", "white");
  //setEdgeAttribute("labelfloat", "true");
//   setNodeAttribute("height", "1.2");
  setEdgeAttribute("minlen", "2");
}

bool QGVScene::loadGraph(const QFileInfo& file_name) {
  qDebug() << Q_FUNC_INFO;

  if (QString::compare(file_name.suffix(), "graphml", Qt::CaseInsensitive) == 0) {
    GraphWalker gw;

    clear();

    if (!gw.readGraphml(this, file_name.absoluteFilePath())) {
      qWarning() << Q_FUNC_INFO << "Could not read:" << file_name.absoluteFilePath();
      return false;
    }

  }
  else {
    qWarning() << Q_FUNC_INFO << "Graph file format" << file_name.suffix() << "is not supported.";
    return false;
  }

  qDebug() << Q_FUNC_INFO << "Loaded graph with" << _nodes.count() << "nodes, and" << _edges.count() << "edges";
  return true;
}

void QGVScene::setGraphAttribute(const QString& name, const QString& value) {
  agattr(_graph, AGRAPH, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVScene::setNodeAttribute(const QString& name, const QString& value) {
  agattr(_graph, AGNODE, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVScene::setEdgeAttribute(const QString& name, const QString& value) {
  agattr(_graph, AGEDGE, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

QGVNode* QGVScene::addNode(const QString& label) {
  qDebug() << Q_FUNC_INFO;
  Agnode_t* node = agnode(_graph, NULL, TRUE);

  if (node == NULL) {
    qWarning() << "Invalid node :" << label;
    return 0;
  }

  QGVNode* item = new QGVNode(node, this);
  item->setLabel(label);
  addItem(item);
  _nodes.append(item);
  return item;
}

QGVEdge* QGVScene::addEdge(QGVNode* source, QGVNode* target, const QString& label) {
  qDebug() << Q_FUNC_INFO;
  Agedge_t* edge = agedge(_graph, source->_node, target->_node, NULL, TRUE);

  if (edge == NULL) {
    qWarning() << "Invalid egde :" << label;
    return 0;
  }

  QGVEdge* item = new QGVEdge(edge, source, target, this);
  item->setLabel(label);
  addItem(item);
  _edges.append(item);

  source->addEdgeItem(item);
  target->addEdgeItem(item);
  return item;
}

void QGVScene::setRootNode(QGVNode* node) {
  qDebug() << Q_FUNC_INFO;
  Q_ASSERT(_nodes.contains(node));
  agset(_graph, "root", node->getLabel().toLocal8Bit().data());
}

void QGVScene::applyLayout() {
  qDebug() << Q_FUNC_INFO << "Start performing layout using 'dot'";

  if (gvLayout(_context, _graph, "dot") != 0) {
    qCritical() << "Layout render error" << agerrors() << QString::fromLocal8Bit(aglasterr());
    return;
  }

  //gvRenderFilename(_context, _graph, "png", "debug.png");
  qDebug() << Q_FUNC_INFO << "Dot layout done.";

  //Update items layout
  foreach (QGVNode * node, _nodes)
    node->updateLayout();

  foreach (QGVEdge * edge, _edges)
    edge->updateLayout();

  setSceneRect(QRectF(0, 0, QGVCore::graphWidth(_graph), QGVCore::graphHeight(_graph)));
  gvFreeLayout(_context, _graph);
  update();
  qDebug() << Q_FUNC_INFO << "Done with applying layout";
}

void QGVScene::clear() {
  qDebug() << Q_FUNC_INFO;
  _nodes.clear();
  _edges.clear();
  QGraphicsScene::clear();

  agclose(_graph);
  gvFreeContext(_context);
  init();
}

void QGVScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* contextMenuEvent) {
  qDebug() << Q_FUNC_INFO;
  QGraphicsItem* item = itemAt(contextMenuEvent->scenePos());

  if (item) {
    item->setSelected(true);

    if (item->type() == QGVNode::Type)
      emit nodeContextMenu(qgraphicsitem_cast<QGVNode*>(item));
    else if (item->type() == QGVEdge::Type)
      emit edgeContextMenu(qgraphicsitem_cast<QGVEdge*>(item));
    else
      emit graphContextMenuEvent();
  }

  QGraphicsScene::contextMenuEvent(contextMenuEvent);
}

// void QGVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) {
//   qDebug() << Q_FUNC_INFO;
//   QGraphicsItem* item = itemAt(mouseEvent->scenePos());
//
//   if (item) {
//     if (item->type() == QGVNode::Type)
//       emit nodeDoubleClick(qgraphicsitem_cast<QGVNode*>(item));
//     else if (item->type() == QGVEdge::Type)
//       emit edgeDoubleClick(qgraphicsitem_cast<QGVEdge*>(item));
//   }
//
//   QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
// }

void QGVScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) {
  qDebug() << Q_FUNC_INFO;
  QGraphicsItem* item = itemAt(mouseEvent->scenePos());

  if (mouseEvent->button() == Qt::LeftButton &&
      QApplication::keyboardModifiers() == Qt::ControlModifier) {

    if (item) {
      line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
      addItem(line);
    }
    else {
      QGVNode* node = addNode("v_NewVertex");
      node->setPos(mouseEvent->scenePos());
    }
  }

  QGraphicsScene::mousePressEvent(mouseEvent);
}

void QGVScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) {
  if (line != 0) {
    QLineF newLine(line->line().p1(), mouseEvent->scenePos());
    line->setLine(newLine);
  }
  else {
    QGraphicsScene::mouseMoveEvent(mouseEvent);
  }
}

void QGVScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) {
  if (line != 0) {
    QList<QGraphicsItem*> startItems = items(line->line().p1());
    qDebug() << Q_FUNC_INFO << "Start items" << startItems.count();
    QGVNode* src = 0;

    foreach (QGraphicsItem * item, startItems) {
      if (dynamic_cast<QGVNode*>(item)) {
        src = dynamic_cast<QGVNode*>(item);
        break;
      }
    }

    QList<QGraphicsItem*> endItems = items(line->line().p2());
    qDebug() << Q_FUNC_INFO << "End items" << endItems.count();
    QGVNode* dst = 0;

    foreach (QGraphicsItem * item, endItems) {
      if (dynamic_cast<QGVNode*>(item)) {
        dst = dynamic_cast<QGVNode*>(item);
        break;
      }
    }

    removeItem(line);
    delete line;
    line = 0;

    if (src && dst) {
      QGVEdge* edge = addEdge(src, dst, "e_EdgeLabel");
      edge->adjust();
    }
  }

  QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void QGVScene::autoLayout() {
  applyLayout();
}
