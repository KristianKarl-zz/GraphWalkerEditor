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

  if ( event->key() == Qt::Key_Return ) {
    clearFocus();
  }
}

void LabelItem::focusOutEvent ( QFocusEvent* event ) {
  setTextInteractionFlags ( Qt::NoTextInteraction );
  setFlag ( QGraphicsItem::ItemIsSelectable, false );
  scene()->update();

  // Clear selection
  QTextCursor c = textCursor();
  c.clearSelection();
  setTextCursor ( c );

  emit lostFocus ( this );
  qDebug() << toPlainText() << " lost focus";
}

void LabelItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event ) {
  if ( event->button() != Qt::LeftButton )
    return;

  setFlag ( QGraphicsItem::ItemIsSelectable, true );

  if ( textInteractionFlags() == Qt::NoTextInteraction ) {
    setTextInteractionFlags ( Qt::TextEditorInteraction );
  }

  setFocus();

  // Select all
  QTextCursor c = textCursor();
  c.select ( QTextCursor::Document );
  setTextCursor ( c );
}
