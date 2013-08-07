#include <QtGui>
#include <boost/graph/graph_concepts.hpp>
#include <ogdf/fileformats/GraphIO.h>

#include "GraphWalkerScene.h"
#include "EdgeItem.h"
#include "Layout.h"

GraphWalkerScene::GraphWalkerScene ( QObject* parent )
  : QGraphicsScene ( parent ) {
  GA = ogdf::GraphAttributes ( G,
       ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics |
       ogdf::GraphAttributes::nodeLabel | ogdf::GraphAttributes::edgeStyle |
       ogdf::GraphAttributes::nodeStyle | ogdf::GraphAttributes::nodeTemplate |
       ogdf::GraphAttributes::edgeLabel );

  myMode = MoveItem;
  line = 0;
  myItemColor = Qt::white;
  myTextColor = Qt::black;
  myLineColor = Qt::black;

  populateGraphFromScene();
}

void GraphWalkerScene::populateGraphFromScene() {
  qDebug() << Q_FUNC_INFO;
  G.clear();

  foreach ( QGraphicsItem * item, items() ) {
    VertexItem* vertex = dynamic_cast<VertexItem*> ( item );

    if ( vertex ) {
      ogdf::node v = G.newNode();
      vertex->set_ogdf_node ( v );

      GA.x ( v ) = vertex->pos().x();
      GA.y ( v ) = vertex->pos().y();
      GA.width ( v ) = vertex->boundingRect().width();
      GA.height ( v ) = vertex->boundingRect().height();
      GA.label ( v ) = vertex->getLabel().toStdString();
    }
  }

  foreach ( QGraphicsItem * item, items() ) {
    EdgeItem* edge = dynamic_cast<EdgeItem*> ( item );

    if ( edge ) {
      ogdf::edge e = G.newEdge ( edge->startItem()->get_ogdf_node(), edge->endItem()->get_ogdf_node() );
      GA.label ( e ) = edge->getLabel().toStdString();
    }
  }
}

void GraphWalkerScene::populateSceneFromGraph() {
  qDebug() << Q_FUNC_INFO;
  clear();

  ogdf::node n;
  forall_nodes ( n, G ) {
    VertexItem* item = new VertexItem ( n );
    item->setPos ( GA.x ( n ), GA.y ( n ) );
    item->setLabel ( GA.label ( n ).c_str() );
    addItem ( item );
  }

  ogdf::edge e;
  forall_edges ( e, G ) {
    EdgeItem* item = new EdgeItem ( getNode ( e->source() ), getNode ( e->target() ) );
    item->setLabel ( GA.label ( e ).c_str() );

    const ogdf::DPolyline& pl = GA.bends ( e );
    qDebug() << Q_FUNC_INFO << "Edge" << GA.label ( e ).c_str() << "Number of bends" << pl.size();
    ogdf::ListConstIterator<ogdf::DPoint> it;

    for ( it = pl.begin(); it.valid(); ++it ) {
      item->getBends().append ( QPointF ( ( *it ).m_x, ( *it ).m_y ) );
    }

    addItem ( item );
  }

  qDebug() << Q_FUNC_INFO << GA.boundingBox().width() << GA.boundingBox().height();

  setSceneRect ( QRectF ( 0,
                 0,
                 GA.boundingBox().width(),
                 GA.boundingBox().height() ) );
  update();
}

void GraphWalkerScene::loadGraph ( const QFileInfo& file_name ) {

  if ( QString::compare ( file_name.suffix(), "gml", Qt::CaseInsensitive ) == 0 ) {
    if ( !ogdf::GraphIO::readGML ( GA, G, file_name.absoluteFilePath().toStdString() ) ) {
      qWarning() << Q_FUNC_INFO << "Could not read:" << file_name.absoluteFilePath();
      return;
    }
  }
  else  if ( QString::compare ( file_name.suffix(), "graphml", Qt::CaseInsensitive ) == 0 ) {
    GraphWalker gw;

    if ( !gw.readGraphml ( &GA, &G, file_name.absoluteFilePath() ) ) {
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

void GraphWalkerScene::newGraph() {
  G.clear();
  clear();
  VertexItem* item = new VertexItem ( G.newNode() );
  item->setLabel ( "Start" );
  item->setKeyWord ( GraphWalker::START_NODE );
  addItem ( item );
}


VertexItem* GraphWalkerScene::getNode ( ogdf::node source ) {
  foreach ( QGraphicsItem * item, items() ) {
    VertexItem* v = dynamic_cast<VertexItem*> ( item );

    if ( v && v->get_ogdf_node() == source ) {
      return v;
    }
  }

  return NULL;
}


void GraphWalkerScene::setLineColor ( const QColor& color ) {
  myLineColor = color;
}

void GraphWalkerScene::setItemColor ( const QColor& color ) {
  myItemColor = color;
}

void GraphWalkerScene::setMode ( Mode mode ) {
  myMode = mode;
}

void GraphWalkerScene::mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent ) {
  if ( mouseEvent->button() != Qt::LeftButton ) {
    QGraphicsScene::mousePressEvent ( mouseEvent );
    return;
  }

  VertexItem* item = 0;
  ogdf::node v = 0;

  switch ( myMode ) {
    case InsertItem:
      v = G.newNode();
      item = new VertexItem ( v );
      item->setPos ( mouseEvent->scenePos() );
      item->setLabel ( "<VertexLabel>" );
      addItem ( item );
      emit itemInserted ( item );
      break;

    case InsertLine:
      line = new QGraphicsLineItem ( QLineF ( mouseEvent->scenePos(),
          mouseEvent->scenePos() ) );
      line->setPen ( QPen ( myLineColor, 2 ) );
      addItem ( line );
      break;

    default
        :
      ;
  }

  QGraphicsScene::mousePressEvent ( mouseEvent );
}

void GraphWalkerScene::mouseMoveEvent ( QGraphicsSceneMouseEvent* mouseEvent ) {
  if ( myMode == InsertLine && line != 0 ) {
    QLineF newLine ( line->line().p1(), mouseEvent->scenePos() );
    line->setLine ( newLine );
  }
  else if ( myMode == MoveItem ) {
    QGraphicsScene::mouseMoveEvent ( mouseEvent );
  }
}

void GraphWalkerScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent* mouseEvent ) {
  if ( line != 0 && myMode == InsertLine ) {
    QList<QGraphicsItem*> startItems = items ( line->line().p1(), Qt::IntersectsItemShape, Qt::AscendingOrder );

    if ( startItems.count() && startItems.first() == line )
      startItems.removeFirst();

    QList<QGraphicsItem*> endItems = items ( line->line().p2(), Qt::IntersectsItemShape, Qt::AscendingOrder );

    if ( endItems.count() && endItems.first() == line )
      endItems.removeFirst();

    removeItem ( line );
    delete line;

    if ( startItems.count() > 0 && endItems.count() > 0 &&
         dynamic_cast<VertexItem*> ( startItems.first() ) &&
         dynamic_cast<VertexItem*> ( endItems.first() ) &&
         startItems.first() != endItems.first() ) {
      VertexItem* startItem = qgraphicsitem_cast<VertexItem*> ( startItems.first() );
      VertexItem* endItem = qgraphicsitem_cast<VertexItem*> ( endItems.first() );

      if ( endItem->getKeyWords() & GraphWalker::START_NODE ||
           ( startItem->getKeyWords() & GraphWalker::START_NODE && startItem->getEdges().count() > 0 ) ) {
        QMessageBox::information ( 0, "No can do!", "The Start node can only have 1 out-edge, and no in-edges." );
      }
      else {
        EdgeItem* arrow = new EdgeItem ( startItem, endItem );
        G.newEdge ( startItem->get_ogdf_node(), endItem->get_ogdf_node() );

        arrow->setColor ( myLineColor );
        startItem->addEdgeItem ( arrow );
        endItem->addEdgeItem ( arrow );
        arrow->setZValue ( -1000.0 );
        addItem ( arrow );
        arrow->updatePosition();
      }
    }
  }

  line = 0;
  QGraphicsScene::mouseReleaseEvent ( mouseEvent );
  update();
}

void GraphWalkerScene::hierarchicalLayout() {
  populateGraphFromScene();
  Layout::hierarchical ( GA );
  populateSceneFromGraph();
}

