#include <QtCore/QDebug>

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

#include "Layout.h"

using namespace ogdf;

void Layout::hierarchical ( ogdf::GraphAttributes& GA ) {
  qDebug() << Q_FUNC_INFO << "Start performing hierarchical layout operation";
  SugiyamaLayout SL;
  SL.setRanking ( new OptimalRanking );
  SL.setCrossMin ( new MedianHeuristic );

  OptimalHierarchyLayout* ohl = new OptimalHierarchyLayout;
  ohl->layerDistance ( 30.0 );
  ohl->nodeDistance ( 25.0 );
  ohl->weightBalancing ( 0.8 );
  SL.setLayout ( ohl );

  SL.call ( GA );
  qDebug() << Q_FUNC_INFO << "Hierarchical layout operation done!";
}
