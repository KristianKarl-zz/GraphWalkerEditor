#include <QtGui>
#include <QLabel>
#include <boost/graph/graph_concepts.hpp>

#include "MainWindow.h"
#include "VertexItem.h"
#include "EdgeItem.h"
#include "GraphWalkerWidget.h"
#include "GraphWalkerScene.h"

const int InsertTextButton = 10;

MainWindow::MainWindow() {
  qDebug() << Q_FUNC_INFO;
  initialize();
}

MainWindow::MainWindow(const QString& graph_file) {
  qDebug() << Q_FUNC_INFO;
  initialize();
  widget->loadGraph(graph_file);
}

void MainWindow::initialize() {
  qDebug() << Q_FUNC_INFO;

  widget = new GraphWalkerWidget;
  setCentralWidget(widget);

  createActions();
  createMenus();
}

void MainWindow::open() {
  qDebug() << Q_FUNC_INFO;
  QString fileName = QFileDialog::getOpenFileName(this, tr(
                       "Open GML file"), currentFile.absoluteFilePath(), tr("Graph files (*.gml *.graphml)"));

  if (!fileName.isEmpty()) {
    if (currentFile.fileName() == fileName) {
      if (QMessageBox::warning(this,
                               tr("GWE question"),
                               tr("Do you want to reload from file?"),
                               QMessageBox::No | QMessageBox::Yes) == QMessageBox::No) {
        return;
      }
    }

    currentFile = QFileInfo(fileName);
    widget->loadGraph(currentFile);
  }
}


void MainWindow::createActions() {
  qDebug() << Q_FUNC_INFO;
  openAct = new QAction(tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  hierarchicalLayoutAction = new QAction(tr("&Hierarchical layout"), this);
  hierarchicalLayoutAction->setStatusTip(tr("Peforms an automatic hierarchical layout of the graph"));
  connect(hierarchicalLayoutAction, SIGNAL(triggered()), widget, SLOT(hierarchicalLayout()));

  exitAction = new QAction(tr("E&xit"), this);
  exitAction->setShortcuts(QKeySequence::Quit);
  exitAction->setStatusTip(tr("Quit Scenediagram example"));
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenus() {
  qDebug() << Q_FUNC_INFO;
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

  layoutMenu = menuBar()->addMenu(tr("&Layout"));
  layoutMenu->addAction(hierarchicalLayoutAction);
}

