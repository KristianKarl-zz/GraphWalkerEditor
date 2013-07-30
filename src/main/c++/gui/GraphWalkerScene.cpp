#include <QtGui>
#include <ogdf/fileformats/GraphIO.h>

#include "GraphWalkerScene.h"
#include "EdgeItem.h"

GraphWalkerScene::GraphWalkerScene ( QObject* parent )
  : QGraphicsScene ( parent ) {
  //myMode = MoveItem;
  myMode = InsertItem;
  line = 0;
  myItemColor = Qt::white;
  myTextColor = Qt::black;
  myLineColor = Qt::black;

  loadGraph();
}

void GraphWalkerScene::loadGraph() {
//   VertexItem* item = new VertexItem ();
//   addItem ( item );
//   item->setPos ( width() / 2, height() / 2 );
}

void GraphWalkerScene::loadGraph ( const QFileInfo& file_name ) {
  ogdf::Graph G;
  ogdf::GraphAttributes GA ( G,
      ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics |
      ogdf::GraphAttributes::nodeLabel | ogdf::GraphAttributes::edgeStyle |
      ogdf::GraphAttributes::nodeStyle | ogdf::GraphAttributes::nodeTemplate |
      ogdf::GraphAttributes::edgeLabel );

  if ( ! ogdf::GraphIO::readGML ( GA, G, file_name.absoluteFilePath().toStdString() ) ) {
    qWarning() << "Could not read: " << file_name.absoluteFilePath();
    return;
  }

  graphAttributes = GA;
  graph = G;

  ogdf::node n;
  forall_nodes ( n, graph ) {
    VertexItem* item = new VertexItem ( n );
    item->setPos ( graphAttributes.x ( n ), graphAttributes.y ( n ) );
    item->setLabel ( graphAttributes.label ( n ).c_str() );
    addItem ( item );
  }

  ogdf::edge e;
  forall_edges ( e, graph ) {
    VertexItem* source = getNode ( e->source() );
    VertexItem* target = getNode ( e->target() );

    addItem ( new EdgeItem ( source, target ) );
  }

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
    EdgeItem* item =
      qgraphicsitem_cast<EdgeItem*> ( selectedItems().first() );
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

  VertexItem* item;

  switch ( myMode ) {
  case InsertItem:
    item = new VertexItem ( "<VertexLabel>" );
    addItem ( item );
    item->setPos ( mouseEvent->scenePos() );
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
