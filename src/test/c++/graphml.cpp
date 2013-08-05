#define BOOST_TEST_MODULE gwe

#include <boost/test/unit_test.hpp>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

#include "GraphWalker.h"

#define ROOT_PATH "../src/test/resources/"
#define INPUT_FILE(x) ROOT_PATH x

BOOST_AUTO_TEST_SUITE ( Graphml_IO_Suite )

BOOST_AUTO_TEST_CASE ( Hierarchical_login_graphml_layout ) {
  using namespace ogdf;
  Graph G;
  GraphAttributes GA ( G,
      GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
      GraphAttributes::nodeLabel | GraphAttributes::edgeStyle |
      GraphAttributes::nodeStyle | GraphAttributes::nodeTemplate |
      GraphAttributes::edgeLabel );

  GraphWalker parser;

  if ( ! parser.readGraphml ( &GA, &G, INPUT_FILE ( "Login.graphml" ) ) ) {
    BOOST_REQUIRE_MESSAGE ( false, "Could not load Login.graphml" );
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
  parser.writeGraphml ( &GA, INPUT_FILE ( "Login-layout.graphml" ) );
}

BOOST_AUTO_TEST_SUITE_END()

