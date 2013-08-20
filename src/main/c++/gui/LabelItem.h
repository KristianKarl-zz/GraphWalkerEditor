#ifndef LABELITEM_H
#define LABELITEM_H

#include <QtGui>

class LabelItem : public QGraphicsTextItem {
    Q_OBJECT

  public:
    LabelItem(const QString& str, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

    void setLabel(const QString&);
    QString getLabel() const;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

  signals:
    void lostFocus(LabelItem* item);
    void selectedChange(QGraphicsItem* item);
    void labelIsUpdated(LabelItem* item);

  protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    void focusOutEvent(QFocusEvent* event);
    void keyPressEvent(QKeyEvent* event);
};

#endif // LABELITEM_H
