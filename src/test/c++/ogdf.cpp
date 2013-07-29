#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE gwe

#include <boost/test/unit_test.hpp>
#include <ogdf/basic/Graph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/FastPlanarSubgraph.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>

#define ROOT_PATH "../src/test/resources/"
#define INPUT_FILE(x) ROOT_PATH x

BOOST_AUTO_TEST_SUITE ( IO_Suite )

BOOST_AUTO_TEST_CASE ( Generate_an_acyclic_random_graph ) {
  using namespace ogdf;
  Graph G;
  randomSimpleGraph ( G, 10, 20 );
  DfsAcyclicSubgraph DAS;
  DAS.callAndReverse ( G );
  GraphIO::writeGML ( G, INPUT_FILE ( "acyclic_random_graph.gml" ) );
}

BOOST_AUTO_TEST_CASE ( Hierarchical_Layout_unix_history_layout ) {
  using namespace ogdf;
  Graph G;
  GraphAttributes GA ( G,
      GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
      GraphAttributes::nodeLabel | GraphAttributes::edgeStyle |
      GraphAttributes::nodeStyle | GraphAttributes::nodeTemplate );

  if ( ! GraphIO::readGML ( GA, G, INPUT_FILE ( "unix-history.gml" ) ) ) {
    BOOST_REQUIRE_MESSAGE ( false, "Could not load unix-history.gml" );
  }

  SugiyamaLayout SL;
  SL.setRanking ( new OptimalRanking );
  SL.setCrossMin ( new MedianHeuristic );

  OptimalHierarchyLayout* ohl = new OptimalHierarchyLayout;
  ohl->layerDistance ( 30.0 );
  ohl->nodeDistance ( 25.0 );
  ohl->weightBalancing ( 0.8 );
  SL.setLayout ( ohl );

  SL.call ( GA );
  GraphIO::writeGML ( GA, INPUT_FILE ( "unix-history-layout.gml" ) );
}


BOOST_AUTO_TEST_CASE ( Hierarchical_Layout_Login ) {
  using namespace ogdf;
  Graph G;
  GraphAttributes GA ( G,
      GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
      GraphAttributes::nodeLabel | GraphAttributes::edgeStyle |
      GraphAttributes::nodeStyle | GraphAttributes::nodeTemplate );

  if ( ! GraphIO::readGML ( GA, G, INPUT_FILE ( "Login.gml" ) ) ) {
    BOOST_REQUIRE_MESSAGE ( false, "Could not load Login.gml" );
  }

  SugiyamaLayout SL;
  SL.setRanking ( new OptimalRanking );
  SL.setCrossMin ( new MedianHeuristic );

  OptimalHierarchyLayout* ohl = new OptimalHierarchyLayout;
  ohl->layerDistance ( 30.0 );
  ohl->nodeDistance ( 25.0 );
  ohl->weightBalancing ( 0.8 );
  SL.setLayout ( ohl );

  SL.call ( GA );
  GraphIO::writeGML ( GA, INPUT_FILE ( "Login-layout.gml" ) );
}


BOOST_AUTO_TEST_SUITE_END()

