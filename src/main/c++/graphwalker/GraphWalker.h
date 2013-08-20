#ifndef GRAPHWALKER_H
#define GRAPHWALKER_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QHash>

class TiXmlHandle;
class TiXmlElement;
class QGVScene;
class QGVNode;

class GraphWalker {
  public:
    enum Keywords { REQTAG = 1,
        START_NODE = 2,
        STOP_NODE = 4,
        BLOCKED = 8,
        SWITCH_MODEL = 16
                  };

    bool readGraphml(QGVScene*, const QString&);
    bool writeGraphml(QGVScene*, const QString&);

  private:

    void parseGraphml(TiXmlHandle);
    void parseNodes(TiXmlElement*);
    void parseEdges(TiXmlElement*);

    QGVScene* scene;
    QHash<QString, QGVNode*> nodeList;
};

#endif


