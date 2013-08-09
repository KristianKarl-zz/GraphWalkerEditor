#ifndef LAYOUT_H
#define LAYOUT_H

#include <ogdf/basic/GraphAttributes.h>

class Layout {
  public:
    static void hierarchical(ogdf::GraphAttributes&);
    static void energyBased(ogdf::GraphAttributes&);
    static void orthogonal(ogdf::GraphAttributes&);
};

#endif // LAYOUT_H
