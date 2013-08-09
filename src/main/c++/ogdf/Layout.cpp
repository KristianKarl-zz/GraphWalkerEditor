#include "Layout.h"

#include <QtCore/QDebug>

#include <ogdf/orthogonal/OrthoLayout.h>

#include <ogdf/cluster/ClusterOrthoLayout.h>

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

#include <ogdf/energybased/FMMMLayout.h>

#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/FastPlanarSubgraph.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
 
void Layout::hierarchical(ogdf::GraphAttributes& GA) {
  qDebug() << Q_FUNC_INFO << "Start performing hierarchical layout operation";
  ogdf::SugiyamaLayout SL;
  SL.setRanking(new ogdf::OptimalRanking);
  SL.setCrossMin(new ogdf::MedianHeuristic);

  ogdf::OptimalHierarchyLayout* ohl = new ogdf::OptimalHierarchyLayout;
  ohl->layerDistance(30.0);
  ohl->nodeDistance(25.0);
  ohl->weightBalancing(0.8);
  SL.setLayout(ohl);

  SL.call(GA);
  qDebug() << Q_FUNC_INFO << "Hierarchical layout operation done!";
}

void Layout::energyBased(ogdf::GraphAttributes& GA) {
  ogdf::FMMMLayout fmmm;

  fmmm.useHighLevelOptions(true);
  fmmm.unitEdgeLength(15.0);
  fmmm.newInitialPlacement(true);
  fmmm.qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);

  fmmm.call(GA);
}

void Layout::orthogonal(ogdf::GraphAttributes& GA) {
  ogdf::PlanarizationLayout pl;
  ogdf::OrthoLayout *ol = new ogdf::OrthoLayout;
  pl.setPlanarLayouter(ol);
  pl.call(GA);
}
