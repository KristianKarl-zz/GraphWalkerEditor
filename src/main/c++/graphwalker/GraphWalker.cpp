#include "GraphWalker.h"

#include <tinyxml.h>
#include <QtCore/QDebug>
#include <boost/graph/graph_concepts.hpp>

class TiXmlGeometry : public TiXmlVisitor {
  private:
    QString str;
    TiXmlElement* pElement;
  public:
    TiXmlGeometry(QString str) {
      this->str = str;
      pElement = 0;
    }
    virtual bool VisitExit(const TiXmlElement& e) {
      if (str.compare(e.Value()) == 0) {
        pElement = const_cast<TiXmlElement*>(&e);
        return false;
      }

      return true;
    }
    TiXmlElement* element() {
      return pElement;
    }
};

bool GraphWalker::readGraphml(ogdf::GraphAttributes* ga, ogdf::Graph* g, const QString& file) {
  qDebug() << Q_FUNC_INFO << "Start reading file:" << file;
  qDebug() << g->numberOfNodes() << g->numberOfEdges();
  GA = ga;
  G = g;
  g->clear();

  TiXmlDocument doc(file.toStdString());

  if (!doc.LoadFile()) {
    qWarning() << Q_FUNC_INFO << "Failed to load file: " << file;
    return false;
  }

  parseGraphml(TiXmlHandle(&doc));
  qDebug() << Q_FUNC_INFO << "File reading is done! Loaded graph with" << g->numberOfNodes() << "nodes, and" << g->numberOfEdges() << "edges";
  return true;
}

bool GraphWalker::writeGraphml(ogdf::GraphAttributes* GA, const QString& file) {
  return false;
}

void GraphWalker::parseGraphml(TiXmlHandle hDoc) {
  TiXmlHandle hRoot(0);
  TiXmlElement* pElem = hDoc.FirstChildElement().Element();

  // should always have a valid root but handle gracefully if it does
  if (!pElem) {
    qWarning() << Q_FUNC_INFO << "No root handle to XML doc";
    return;
  }

  // save this for later
  hRoot = TiXmlHandle(pElem).FirstChild("graph");

  // Parse all nodes
  parseNodes(hRoot.FirstChild("node").Element());
  // Parse all edges
  parseEdges(hRoot.FirstChild("edge").Element());
}

void GraphWalker::parseNodes(TiXmlElement* pElem) {
  for (; pElem; pElem = pElem->NextSiblingElement("node")) {
    const char* id = pElem->Attribute("id");

    if (!id) {
      qWarning() << Q_FUNC_INFO << "No id found for node. That was unexpected!";
      return;
    }

    qDebug() << Q_FUNC_INFO << "Node:" << id;
    nodeList[id] = G->newNode();

    TiXmlGeometry geometry("y:Geometry");
    pElem->Accept(&geometry);

    if (geometry.element()) {
      TiXmlElement* element = geometry.element();
      GA->x(nodeList[id]) = atof(element->Attribute("x"));
      GA->y(nodeList[id]) = atof(element->Attribute("y"));
      GA->width(nodeList[id]) = atof(element->Attribute("width"));
      GA->height(nodeList[id]) = atof(element->Attribute("height"));
    }

    TiXmlGeometry label("y:NodeLabel");
    pElem->Accept(&label);

    if (label.element()) {
      TiXmlElement* element = label.element();
      qDebug() << Q_FUNC_INFO << "Node:" << element->GetText();
      GA->label(nodeList[id]) = element->GetText();
    }
  }
}

void GraphWalker::parseEdges(TiXmlElement* pElem) {
  for (; pElem; pElem = pElem->NextSiblingElement("edge")) {
    const char* id = pElem->Attribute("id");
    const char* src = pElem->Attribute("source");
    const char* dst = pElem->Attribute("target");

    if (!src || !dst) {
      qWarning() << Q_FUNC_INFO << "No source and target id's found for edge. That was unexpected!";
      return;
    }

    qDebug() << Q_FUNC_INFO << "Edge:" << id << "from:" << src << "to:" << dst;
    ogdf::edge e = G->newEdge(nodeList[src], nodeList[dst]);

    TiXmlGeometry label("y:EdgeLabel");
    pElem->Accept(&label);

    if (label.element()) {
      TiXmlElement* element = label.element();
      qDebug() << Q_FUNC_INFO << "Edge:" << element->GetText();
      GA->label(e) = element->GetText();
    }
  }
}




