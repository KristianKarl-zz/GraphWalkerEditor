#ifndef GRAPHWALKER_H
#define GRAPHWALKER_H

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Graph_d.h>

#include <QtCore/QString>
#include <QtCore/QHash>

class TiXmlHandle;
class TiXmlElement;

class GraphWalker {
  public:
    enum Keywords { REQTAG = 1,
        START_NODE = 2,
        STOP_NODE = 4,
        BLOCKED = 8,
        SWITCH_MODEL = 16
                  };

    bool readGraphml ( ogdf::GraphAttributes* GA, ogdf::Graph* G, const QString& );
    bool writeGraphml ( ogdf::GraphAttributes* GA, const QString& );

  private:

    void parseGraphml ( TiXmlHandle );
    void parseNodes ( TiXmlElement* );
    void parseEdges ( TiXmlElement* );

    ogdf::GraphAttributes* GA;
    ogdf::Graph* G;

    QHash<QString, ogdf::node> nodeList;
};

#endif
