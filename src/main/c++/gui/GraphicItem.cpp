#include "GraphicItem.h"

GraphicItem::GraphicItem ()
  : keywords ( 0 ) {
  label = new LabelItem ( "" );
}

GraphicItem::GraphicItem ( const QString& name )
  : keywords ( 0 ) {
  label = new LabelItem ( name );
}

void GraphicItem::addKeyWord ( const GrapwWalker::Keywords& keyword ) {
  keywords |= keyword;
}

void GraphicItem::setKeyWord ( const GrapwWalker::Keywords& keyword ) {
  keywords = keyword;
}

QString GraphicItem::getLabel() {
  return label->toPlainText();
}

void GraphicItem::setLabel ( const QString& str ) {
  label->setPlainText ( str );
}
