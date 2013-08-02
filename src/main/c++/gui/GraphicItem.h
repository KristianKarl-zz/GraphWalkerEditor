#ifndef GRAPHICITEM_H
#define GRAPHICITEM_H

#include <QtGui>

#include "GraphWalker.h"
#include "LabelItem.h"

class GraphicItem : public QObject  {
  public:

    GraphicItem ();
    GraphicItem ( const QString& label );

    void setLabel ( const QString& );
    QString getLabel();

    void setKeyWord ( const GrapwWalker::Keywords& );
    void addKeyWord ( const GrapwWalker::Keywords& );
    int getKeyWords () {
      return keywords;
    }

  protected:
    LabelItem* label;
    int keywords;
    QColor myColor;
};

#endif // GRAPHICITEM_H
