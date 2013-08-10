#include <QtGui>
#include <ogdf/fileformats/GraphIO.h>

#include "GraphWalkerScene.h"
#include "EdgeItem.h"
#include "Layout.h"

GraphWalkerScene::GraphWalkerScene(QObject* parent)
  : QGraphicsScene(parent) {
  qDebug() << Q_FUNC_INFO;
  GA = ogdf::GraphAttributes(G,
       ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics |
       ogdf::GraphAttributes::nodeLabel | ogdf::GraphAttributes::edgeStyle |
       ogdf::GraphAttributes::nodeStyle | ogdf::GraphAttributes::nodeTemplate |
       ogdf::GraphAttributes::edgeLabel);
}

void GraphWalkerScene::populateGraphFromScene() {
  qDebug() << Q_FUNC_INFO;
  G.clear();

  foreach (QGraphicsItem * item, items()) {
    VertexItem* vertex = dynamic_cast<VertexItem*>(item);

    if (vertex) {
      ogdf::node v = G.newNode();
      vertex->set_ogdf_node(v);

      GA.x(v) = vertex->pos().x();
      GA.y(v) = vertex->pos().y();
      GA.width(v) = vertex->boundingRect().width();
      GA.height(v) = vertex->boundingRect().height();
      GA.label(v) = vertex->getLabel().toStdString();
    }
  }

  foreach (QGraphicsItem * item, items()) {
    EdgeItem* edge = dynamic_cast<EdgeItem*>(item);

    if (edge) {
      ogdf::edge e = G.newEdge(edge->startItem()->get_ogdf_node(), edge->endItem()->get_ogdf_node());
      GA.label(e) = edge->getLabel().toStdString();
    }
  }
}

void GraphWalkerScene::populateSceneFromGraph() {
  qDebug() << Q_FUNC_INFO;
  clear();

  ogdf::node n;
  forall_nodes(n, G) {
    VertexItem* item = new VertexItem(n);
    item->setPos(GA.x(n), GA.y(n));
    item->setLabel(GA.label(n).c_str());

    if (item->getLabel().compare("START", Qt::CaseInsensitive)==0) {
      item->setKeyWord(GraphWalker::START_NODE);
    }

    addItem(item);
  }

  ogdf::edge e;
  forall_edges(e, G) {
    EdgeItem* item = new EdgeItem(getNode(e->source()), getNode(e->target()));
    item->setLabel(GA.label(e).c_str());

    const ogdf::DPolyline& pl = GA.bends(e);
    qDebug() << Q_FUNC_INFO << "Edge" << GA.label(e).c_str() << "Number of bends" << pl.size();
    ogdf::ListConstIterator<ogdf::DPoint> it;

    for (it = pl.begin(); it.valid(); ++it) {
      item->getBends().append(QPointF((*it).m_x, (*it).m_y));
    }

    addItem(item);
  }

  qDebug() << Q_FUNC_INFO << GA.boundingBox().width() << GA.boundingBox().height();

  setSceneRect(QRectF(0,
               0,
               GA.boundingBox().width(),
               GA.boundingBox().height()));
  update();
}

void GraphWalkerScene::loadGraph(const QFileInfo& file_name) {
  qDebug() << Q_FUNC_INFO;

  if (QString::compare(file_name.suffix(), "gml", Qt::CaseInsensitive) == 0) {
    if (!ogdf::GraphIO::readGML(GA, G, file_name.absoluteFilePath().toStdString())) {
      qWarning() << Q_FUNC_INFO << "Could not read:" << file_name.absoluteFilePath();
      return;
    }

  }
  else if (QString::compare(file_name.suffix(), "graphml", Qt::CaseInsensitive) == 0) {
    GraphWalker gw;

    qDebug() << G.numberOfNodes() << G.numberOfEdges();

    if (!gw.readGraphml(&GA, &G, file_name.absoluteFilePath())) {
      qWarning() << Q_FUNC_INFO << "Could not read:" << file_name.absoluteFilePath();
      return;
    }

  }
  else {
    qWarning() << Q_FUNC_INFO << "Graph file format" << file_name.suffix() << "is not supported.";
    return;
  }

  qDebug() << Q_FUNC_INFO << "Loaded graph with" << G.numberOfNodes() << "nodes, and" << G.numberOfEdges() << "edges";

  populateSceneFromGraph();
}

VertexItem* GraphWalkerScene::getNode(ogdf::node source) {
  foreach (QGraphicsItem * item, items()) {
    VertexItem* v = dynamic_cast<VertexItem*>(item);

    if (v && v->get_ogdf_node() == source) {
      return v;
    }
  }

  qWarning() << Q_FUNC_INFO << "No graphical vertex found with ogdf::node" << source;
  return NULL;
}

void GraphWalkerScene::hierarchicalLayout() {
  populateGraphFromScene();
  Layout::hierarchical(GA);
  populateSceneFromGraph();
}

void GraphWalkerScene::energyBasedLayout() {
  populateGraphFromScene();
  Layout::energyBased(GA);
  populateSceneFromGraph();
}

void GraphWalkerScene::orthogonalLayout() {
  populateGraphFromScene();
  Layout::orthogonal(GA);
  populateSceneFromGraph();
}


