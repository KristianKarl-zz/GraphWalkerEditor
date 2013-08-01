#include <QtGui>
#include <ogdf/fileformats/GraphIO.h>

#include "GraphWalkerScene.h"
#include "EdgeItem.h"
#include "Layout.h"

GraphWalkerScene::GraphWalkerScene ( QObject* parent )
  : QGraphicsScene ( parent ) {
  graphAttributes = ogdf::GraphAttributes ( graph,
      ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics |
      ogdf::GraphAttributes::nodeLabel | ogdf::GraphAttributes::edgeStyle |
      ogdf::GraphAttributes::nodeStyle | ogdf::GraphAttributes::nodeTemplate |
      ogdf::GraphAttributes::edgeLabel );

  myMode = InsertItem;
  line = 0;
  myItemColor = Qt::white;
  myTextColor = Qt::black;
  myLineColor = Qt::black;

  loadGraph();
}

void GraphWalkerScene::loadGraph() {
  graph.clear();

  foreach ( QGraphicsItem * item, items() ) {
    VertexItem* vertex = dynamic_cast<VertexItem*> ( item );

    if ( vertex ) {
      ogdf::node v = graph.newNode();
      vertex->set_ogdf_node ( v );

      graphAttributes.x ( v ) = vertex->pos().x();
      graphAttributes.y ( v ) = vertex->pos().y();
      graphAttributes.width ( v ) = vertex->boundingRect().width();
      graphAttributes.height ( v ) = vertex->boundingRect().height();
      graphAttributes.label ( v ) = vertex->getLabel().toStdString();
    }
  }

  foreach ( QGraphicsItem * item, items() ) {
    EdgeItem* edge = dynamic_cast<EdgeItem*> ( item );

    if ( edge ) {
      ogdf::edge e = graph.newEdge ( edge->startItem()->get_ogdf_node(), edge->endItem()->get_ogdf_node() );
      graphAttributes.label ( e ) = edge->getLabel().toStdString();
    }
  }
}

void GraphWalkerScene::loadScene() {
  clear();
  ogdf::node n;
  forall_nodes ( n, graph ) {
    VertexItem* item = new VertexItem ( n );
    item->setPos ( graphAttributes.x ( n ), graphAttributes.y ( n ) );
    item->setLabel ( graphAttributes.label ( n ).c_str() );
    if ( item->getLabel().compare ( "Start", Qt::CaseInsensitive ) == 0 ) {
      item->setKeyWord(GrapwWalker::START_NODE);
    }
    addItem ( item );
  }

  ogdf::edge e;
  forall_edges ( e, graph ) {
    VertexItem* source = getNode ( e->source() );
    VertexItem* target = getNode ( e->target() );

    addItem ( new EdgeItem ( source, target ) );
  }

  setSceneRect ( QRectF ( 0,
                 0,
                 graphAttributes.boundingBox().width(),
                 graphAttributes.boundingBox().height() ) );
  update();
}

void GraphWalkerScene::loadGraph ( const QFileInfo& file_name ) {
  if ( ! ogdf::GraphIO::readGML ( graphAttributes, graph, file_name.absoluteFilePath().toStdString() ) ) {
    qWarning() << "Could not read: " << file_name.absoluteFilePath();
    return;
  }

  loadScene();
}

void GraphWalkerScene::newGraph() {
  graph.clear();
  clear();
  VertexItem* item = new VertexItem (graph.newNode());
  item->setLabel("Start");
  item->setKeyWord(GrapwWalker::START_NODE);
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

  if ( isItemChange ( EdgeItem::Type ) ) {
    EdgeItem* item = qgraphicsitem_cast<EdgeItem*> ( selectedItems().first() );
    item->setColor ( myLineColor );
    update();
  }
}

void GraphWalkerScene::setItemColor ( const QColor& color ) {
  myItemColor = color;

  if ( isItemChange ( VertexItem::Type ) ) {
    VertexItem* item =
      qgraphicsitem_cast<VertexItem*> ( selectedItems().first() );
    item->setBrush ( myItemColor );
  }
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
    v = graph.newNode();
    item = new VertexItem ( v );
    item->setPos ( mouseEvent->scenePos() );
    item->setLabel ( "<VertexLabel>" );

    graphAttributes.x ( v ) = item->pos().x();
    graphAttributes.y ( v ) = item->pos().y();
    //graphAttributes.label ( item->getLabel().toStdString().c_str() );

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
  else
    if ( myMode == MoveItem ) {
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
         startItems.first()->type() == VertexItem::Type &&
         endItems.first()->type() == VertexItem::Type &&
         startItems.first() != endItems.first() ) {
      VertexItem* startItem = qgraphicsitem_cast<VertexItem*> ( startItems.first() );
      VertexItem* endItem = qgraphicsitem_cast<VertexItem*> ( endItems.first() );

      if ( endItem->getKeyWords() & GrapwWalker::START_NODE ||
           ( startItem->getKeyWords() & GrapwWalker::START_NODE && startItem->getEdges().count() > 0 ) ) {
        QMessageBox::information ( 0, "No can do!", "The Start node can only have 1 out-edge, and no in-edges." );
      }
      else {
        EdgeItem* arrow = new EdgeItem ( startItem, endItem );
        graph.newEdge ( startItem->get_ogdf_node(), endItem->get_ogdf_node() );

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

bool GraphWalkerScene::isItemChange ( int type ) {
  foreach ( QGraphicsItem * item, selectedItems() ) {
    if ( item->type() == type )
      return true;
  }

  return false;
}

void GraphWalkerScene::hierarchicalLayout() {
  loadGraph();
  Layout::hierarchical ( graphAttributes );
  loadScene();
}

