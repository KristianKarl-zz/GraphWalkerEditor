#include <QtGui>
#include <QMenu>
#include <QAction>

#include "VertexItem.h"
#include "EdgeItem.h"
#include "LabelItem.h"

VertexItem::VertexItem ( QGraphicsItem* parent, QGraphicsScene* scene )
  : QGraphicsPolygonItem ( parent, scene ), keywords ( 0 ), blocked ( false ), switchModel ( false ) {
  setKeyWord ( GrapwWalker::START_NODE );
  init ( "Start" );
}

VertexItem::VertexItem ( const QString& name, QGraphicsItem* parent, QGraphicsScene* scene )
  : QGraphicsPolygonItem ( parent, scene ), keywords ( 0 ), blocked ( false ), switchModel ( false ) {
  init ( name );
}

void VertexItem::init ( const QString& name ) {
  setFlag ( QGraphicsItem::ItemIsMovable, true );
  setFlag ( QGraphicsItem::ItemIsSelectable, true );
  setFlag ( QGraphicsItem::ItemSendsGeometryChanges, true );

  label = new LabelItem ( name, this );

  blockAction = new QAction ( "&Blocked", this );
  blockAction->setStatusTip ( "This will exclude this vertex from the graph." );
  blockAction->setCheckable ( true );
  blockAction->setChecked ( blocked );
  connect ( blockAction, SIGNAL ( triggered() ), this, SLOT ( toggleBlocked() ) );

  switchModelAction = new QAction ( "&Switch model", this );
  switchModelAction->setStatusTip ( "This will enable SWITCH_MODEL" );
  switchModelAction->setCheckable ( true );
  switchModelAction->setChecked ( switchModel );
  connect ( switchModelAction, SIGNAL ( triggered() ), this, SLOT ( toggleSwitchModel() ) );
}

QRectF VertexItem::boundingRect() const {
  QFontMetrics fontMetrics ( label->font() );
  QString text = label->toPlainText();

  if ( blocked && QString ( "BLOCKED" ).length() > text.length() ) {
    text = QString ( "BLOCKED" );
  }

  if ( switchModel && QString ( "SWITCH_MODEL" ).length() > text.length() ) {
    text = QString ( "SWITCH_MODEL" );
  }

  QRectF boundingRect = fontMetrics.boundingRect ( text );
  label->setPos ( -boundingRect.width() / 2, -boundingRect.height() + 4 );
  QRectF rect = QRectF ( -boundingRect.width() / 2, -boundingRect.height() / 2, boundingRect.width(), boundingRect.height() ).adjusted ( -2, -2, 10, 2 );

  if ( blocked ) {
    rect.adjust ( 0, 0, 0, boundingRect.height() );
  }

  if ( switchModel ) {
    rect.adjust ( 0, 0, 0, boundingRect.height() );
  }

  return rect;
}

QPainterPath VertexItem::shape() const {
  QPainterPath path;
  path.addRoundedRect ( boundingRect(), 10, 10 );
  return path;
}

void VertexItem::paint ( QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* ) {
  if ( getKeyWords() & GrapwWalker::START_NODE ) {
    painter->setBrush ( Qt::green );
  }
  else {
    painter->setBrush ( Qt::white );
  }

  if ( isSelected() ) {
    painter->setPen ( QPen ( Qt::black, 2 ) );
  }
  else {
    painter->setPen ( QPen ( Qt::black, 1 ) );
  }

  myPolygon = shape().toFillPolygon();
  painter->drawPolygon ( myPolygon );

  QPointF textPixelOffset = QPoint ( -boundingRect().width() / 2 + 6, 18 );

  if ( blocked ) {
    painter->setPen ( QPen ( Qt::black, 1 ) );
    painter->drawText ( textPixelOffset, "BLOCKED" );
    textPixelOffset += QPointF ( 0, 16 );
  }

  if ( switchModel ) {
    painter->setPen ( QPen ( Qt::black, 1 ) );
    painter->drawText ( textPixelOffset, "SWITCH_MODEL" );
  }
}


void VertexItem::removeEdgeItem ( EdgeItem* edge ) {
  int index = edges.indexOf ( edge );

  if ( index != -1 )
    edges.removeAt ( index );
}

void VertexItem::removeEdgeItems() {
  foreach ( EdgeItem * edge, edges ) {
    edge->startItem()->removeEdgeItem ( edge );
    edge->endItem()->removeEdgeItem ( edge );
    scene()->removeItem ( edge );
    delete edge;
  }
}

void VertexItem::addEdgeItem ( EdgeItem* edge ) {
  edges.append ( edge );
}

void VertexItem::contextMenuEvent ( QPoint pt ) {
  scene()->clearSelection();
  setSelected ( true );

  QMenu menu;
  menu.addAction ( blockAction );
  menu.addAction ( switchModelAction );
  menu.exec ( pt );
}

QVariant VertexItem::itemChange ( GraphicsItemChange change, const QVariant& value ) {
  if ( change == QGraphicsItem::ItemPositionChange ) {
    foreach ( EdgeItem * edge, edges ) {
      edge->updatePosition();
    }
  }

  return value;
}

void VertexItem::addKeyWord ( const GrapwWalker::Keywords& keyword ) {
  keywords |= keyword;
}

void VertexItem::setKeyWord ( const GrapwWalker::Keywords& keyword ) {
  keywords = keyword;
}

void VertexItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* mouseEvent ) {
  if ( getKeyWords() & GrapwWalker::START_NODE )
    return;

  label->mouseDoubleClickEvent ( mouseEvent );
}

void VertexItem::mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent ) {
  if ( mouseEvent->button() != Qt::RightButton ) {
    return;
  }

  if ( getKeyWords() & GrapwWalker::START_NODE ) {
    return;
  }

  contextMenuEvent ( QCursor::pos() );
}

void VertexItem::toggleBlocked() {
  blocked = ! blocked;
  scene()->update();
}

void VertexItem::toggleSwitchModel() {
  switchModel = !switchModel;
  scene()->update();
}
