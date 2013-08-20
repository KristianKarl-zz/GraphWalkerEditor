#include "LabelItem.h"
#include "GraphWalkerScene.h"

#include <QtGui>

LabelItem::LabelItem(const QString& str, QGraphicsItem* parent, QGraphicsScene* scene)
  : QGraphicsTextItem(parent, scene) {
  setPlainText(str);
  setFlag(QGraphicsItem::ItemIsMovable, false);
  setFlag(QGraphicsItem::ItemIsSelectable, false);
  setZValue(2);
}

QVariant LabelItem::itemChange(GraphicsItemChange change, const QVariant& value) {
  if (change == QGraphicsItem::ItemSelectedHasChanged) {
    emit selectedChange(this);
  }

  return value;
}

void LabelItem::keyPressEvent(QKeyEvent* event) {
  qDebug() << Q_FUNC_INFO << toPlainText();

  if (event->key() != Qt::Key_Space && event->key() != Qt::Key_Tab && event->key() != Qt::Key_Return) {
    QGraphicsTextItem::keyPressEvent(event);
    emit labelIsUpdated(this);
  }

  if (event->key() == Qt::Key_Return) {
    clearFocus();
  }
}

void LabelItem::focusOutEvent(QFocusEvent* event) {
  qDebug() << Q_FUNC_INFO << toPlainText();
  setTextInteractionFlags(Qt::NoTextInteraction);
  setFlag(QGraphicsItem::ItemIsSelectable, false);
  scene()->update();

  // Clear selection
  QTextCursor c = textCursor();
  c.clearSelection();
  setTextCursor(c);

  emit lostFocus(this);
}

void LabelItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
  qDebug() << Q_FUNC_INFO << toPlainText();

  if (event->button() != Qt::LeftButton)
    return;

  setFlag(QGraphicsItem::ItemIsSelectable, true);

  if (textInteractionFlags() == Qt::NoTextInteraction) {
    setTextInteractionFlags(Qt::TextEditorInteraction);
  }

  setFocus();

  // Select all
  QTextCursor c = textCursor();
  c.select(QTextCursor::Document);
  setTextCursor(c);
}


QString LabelItem::getLabel() const {
  return toPlainText();
}

void LabelItem::setLabel(const QString& str) {
  setPlainText(str);
  setPos(parentItem()->boundingRect().topLeft());
}

