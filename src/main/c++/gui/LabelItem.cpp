#include "LabelItem.h"
#include "GraphWalkerScene.h"

#include <QtGui>

LabelItem::LabelItem ( const QString& str, QGraphicsItem* parent, QGraphicsScene* scene )
  : QGraphicsTextItem ( parent, scene ) {
  setPlainText ( str );
  setFlag ( QGraphicsItem::ItemIsMovable, false );
  setFlag ( QGraphicsItem::ItemIsSelectable, false );
}

QVariant LabelItem::itemChange ( GraphicsItemChange change, const QVariant& value ) {
  if ( change == QGraphicsItem::ItemSelectedHasChanged ) {
    emit selectedChange ( this );
  }

  return value;
}

void LabelItem::keyPressEvent ( QKeyEvent* event ) {
  if ( event->key() != Qt::Key_Space && event->key() != Qt::Key_Tab && event->key() != Qt::Key_Return )
    QGraphicsTextItem::keyPressEvent ( event );
}

void LabelItem::focusOutEvent ( QFocusEvent* event ) {
  setTextInteractionFlags ( Qt::NoTextInteraction );
  setFlag ( QGraphicsItem::ItemIsSelectable, false );
  scene()->update();
  emit lostFocus ( this );
  QGraphicsTextItem::focusOutEvent ( event );
}

void LabelItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event ) {
  if ( event->button() != Qt::LeftButton )
    return;

  setFlag ( QGraphicsItem::ItemIsSelectable, true );

  if ( textInteractionFlags() == Qt::NoTextInteraction ) {
    setTextInteractionFlags ( Qt::TextEditorInteraction );
  }

  QGraphicsTextItem::mouseDoubleClickEvent ( event );
}
