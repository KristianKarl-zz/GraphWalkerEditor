#ifndef QGVEDGE_H
#define QGVEDGE_H

#include <QGVCore.h>
#include <QGraphicsItem>
#include <QPen>

#include "GraphicItem.h"

class QGVNode;
class QGVScene;

/**
 * @brief Edge item
 *
 */
class QGVEdge : public GraphicItem, public QGraphicsItem {
    Q_OBJECT
  public:
    ~QGVEdge();

    QRectF boundingRect() const;
    QPainterPath shape() const;

    QString getLabel() const;
    void setLabel(const QString& label);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

    void setAttribute(const QString& name, const QString& value);
    QString getAttribute(const QString& name) const;

    void setToolTip(const QString& toolTip);

    void updateLayout();
    void adjust();

    enum { Type = UserType + 3 };
    int type() const {
      return Type;
    }

    QGVNode* srcNode() const {
      return _src;
    }
    QGVNode* dstNode() const {
      return _dst;
    }

  public slots:
    void labelIsUpdated(LabelItem*);

  private:
    QGVEdge(Agedge_t* edge, QGVNode* src, QGVNode* dst, QGVScene* scene);

    QPolygonF toArrow(const qreal, const QPointF&) const;

    friend class QGVScene;
    friend class QGVNode;

    Agedge_t* _edge;
    QGVNode* _src;
    QGVNode* _dst;
    QGVScene* _scene;

    QPainterPath _path;
    QPen _pen;
    QPolygonF _head_arrow;
    qreal arrowSize;

    QString _label;
    QRectF _label_rect;
};

#endif // QGVEDGE_H
