#include "GraphWalkerWidget.h"
#include "GraphWalkerScene.h"

GraphWalkerWidget::GraphWalkerWidget ( GraphWalkerScene* s ) : scene ( s ) {
  setScene ( scene );
  setCacheMode ( CacheBackground );
  setViewportUpdateMode ( BoundingRectViewportUpdate );
  setRenderHint ( QPainter::Antialiasing );
  setTransformationAnchor ( AnchorUnderMouse );
}
