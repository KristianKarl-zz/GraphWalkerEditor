#include "GraphicItem.h"

GraphicItem::GraphicItem ()
  : keywords ( 0 ) {
  label = new LabelItem ( "" );
}

GraphicItem::GraphicItem ( const QString& name )
  : keywords ( 0 ) {
  label = new LabelItem ( name );
}

void GraphicItem::addKeyWord ( const GraphWalker::Keywords& keyword ) {
  keywords |= keyword;
}

void GraphicItem::setKeyWord ( const GraphWalker::Keywords& keyword ) {
  keywords = keyword;
}

QString GraphicItem::getLabel() const {
  return label->toPlainText();
}

void GraphicItem::setLabel ( const QString& str ) {
  label->setPlainText ( str );
  label->setPos(label->parentItem()->boundingRect().topLeft());
}
