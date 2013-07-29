#ifndef LABELITEM_H
#define LABELITEM_H

#include <QtGui>

class LabelItem : public QGraphicsTextItem {
    Q_OBJECT

  public:
    enum { Type = UserType + 3 };

    LabelItem ( const QString& str, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0 );

    int type() const {
      return Type;
    }

    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent* event );

  signals:
    void lostFocus ( LabelItem* item );
    void selectedChange ( QGraphicsItem* item );

  protected:
    QVariant itemChange ( GraphicsItemChange change, const QVariant& value );
    void focusOutEvent ( QFocusEvent* event );
    void keyPressEvent ( QKeyEvent* event );
};

#endif // LABELITEM_H
