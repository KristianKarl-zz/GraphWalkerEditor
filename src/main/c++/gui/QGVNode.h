#ifndef QGVNODE_H
#define QGVNODE_H

#include <QGVCore.h>
#include <QGraphicsItem>
#include <QPen>

#include "GraphicItem.h"

class QGVEdge;
class QGVScene;

/**
 * @brief Node item
 *
 */
class QGVNode : public GraphicItem, public QGraphicsItem {
    Q_OBJECT
  public:
    ~QGVNode();

    QString getLabel() const;
    void setLabel(const QString& label);

    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    void setAttribute(const QString& label, const QString& value);
    QString getAttribute(const QString& name) const;

    void setIcon(const QImage& icon);

    QPointF getCenter();

    enum { Type = UserType + 2 };
    int type() const {
      return Type;
    }

    void removeEdgeItem(QGVEdge* edge);
    void removeEdgeItems();
    void addEdgeItem(QGVEdge* edge);
    QList<QGVEdge*> getEdges() {
      return edges;
    }

  public slots:
    void labelIsUpdated(LabelItem*);

  protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
//     void mousePressEvent(QGraphicsSceneMouseEvent* event);
//     void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

  private:
    friend class QGVScene;
    void updateLayout();
    QGVNode(Agnode_t* node, QGVScene* scene);

    QPainterPath makeShape(Agnode_t* node) const;
    QPolygonF makeShapeHelper(Agnode_t* node) const;

    Agnode_t* _node;
    QPainterPath _path;
    QPen _pen;
    QBrush _brush;
    QImage _icon;

    QGVScene* _scene;

    QList<QGVEdge*> edges;
};


#endif // QGVNODE_H
