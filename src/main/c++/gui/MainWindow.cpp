#include <QtGui>
#include <QLabel>
#include <boost/graph/graph_concepts.hpp>

#include "MainWindow.h"
#include "VertexItem.h"
#include "GraphWalkerWidget.h"
#include "GraphWalkerScene.h"
#include "EdgeItem.h"

const int InsertTextButton = 10;

MainWindow::MainWindow() {
  initialize();
}

MainWindow::MainWindow ( const QString& graph_file ) {
  initialize();
  scene->loadGraph(graph_file);
}

void MainWindow::initialize() {
  createActions();
  createMenus();
  createToolbars();

  scene = new GraphWalkerScene ( this );
  scene->setSceneRect ( QRectF ( 0, 0, 5000, 5000 ) );

  //scene->setSceneRect ( -200, -200, 400, 400 );

  QHBoxLayout* layout = new QHBoxLayout;
  view = new GraphWalkerWidget ( scene );
  layout->addWidget ( view );

  QWidget* widget = new QWidget;
  widget->setLayout ( layout );

  setCentralWidget ( widget );
  setWindowTitle ( tr ( "GraphWalker Editor" ) );
  setUnifiedTitleAndToolBarOnMac ( true );

  connect ( scene, SIGNAL ( itemInserted ( VertexItem* ) ),
            this, SLOT ( itemInserted ( VertexItem* ) ) );
}

void MainWindow::newFile() {
  scene->clear();
}

void MainWindow::open() {
  QString fileName = QFileDialog::getOpenFileName ( this, tr (
                       "Open GML file" ), currentFile.absoluteFilePath(), tr ( "GML file (*.gml)" ) );

  if ( !fileName.isEmpty() ) {
    if ( currentFile.fileName() == fileName ) {
      if ( QMessageBox::warning ( this,
                                  tr ( "GWE question" ),
                                  tr ( "Do you want to reload from file?" ),
                                  QMessageBox::No | QMessageBox::Yes ) == QMessageBox::No ) {
        return;
      }
    }

    currentFile = QFileInfo ( fileName );
    scene->loadGraph(currentFile);
  }
}

void MainWindow::deleteItem() {
  foreach ( QGraphicsItem * item, scene->selectedItems() ) {
    if ( item->type() == EdgeItem::Type ) {
      scene->removeItem ( item );
      EdgeItem* arrow = qgraphicsitem_cast<EdgeItem*> ( item );
      arrow->startItem()->removeEdgeItem ( arrow );
      arrow->endItem()->removeEdgeItem ( arrow );
      delete item;
    }
  }

  foreach ( QGraphicsItem * item, scene->selectedItems() ) {
    if ( item->type() == VertexItem::Type ) {
      qgraphicsitem_cast<VertexItem*> ( item )->removeEdgeItems();
    }

    scene->removeItem ( item );
    delete item;
  }
}

void MainWindow::pointerGroupClicked ( int ) {
  scene->setMode ( GraphWalkerScene::Mode ( pointerTypeGroup->checkedId() ) );
}


void MainWindow::itemInserted ( VertexItem* item ) {
}

void MainWindow::sceneScaleChanged ( const QString& scale ) {
  double newScale = scale.left ( scale.indexOf ( tr ( "%" ) ) ).toDouble() / 100.0;
  QMatrix oldMatrix = view->matrix();
  view->resetMatrix();
  view->translate ( oldMatrix.dx(), oldMatrix.dy() );
  view->scale ( newScale, newScale );
}

void MainWindow::itemColorChanged() {
  fillAction = qobject_cast<QAction*> ( sender() );
  fillColorToolButton->setIcon ( createColorToolButtonIcon (
        ":/images/floodfill.png",
        qvariant_cast<QColor> ( fillAction->data() ) ) );
  fillButtonTriggered();
}

void MainWindow::lineColorChanged() {
  lineAction = qobject_cast<QAction*> ( sender() );
  lineColorToolButton->setIcon ( createColorToolButtonIcon (
        ":/images/linecolor.png",
        qvariant_cast<QColor> ( lineAction->data() ) ) );
  lineButtonTriggered();
}

void MainWindow::fillButtonTriggered() {
  scene->setItemColor ( qvariant_cast<QColor> ( fillAction->data() ) );
}

void MainWindow::lineButtonTriggered() {
  scene->setLineColor ( qvariant_cast<QColor> ( lineAction->data() ) );
}

void MainWindow::about() {
  QMessageBox::about ( this, tr ( "About Diagram Scene" ),
      tr ( "The <b>Diagram Scene</b> example shows "
           "use of the graphics framework." ) );
}


void MainWindow::createActions() {
  newAct = new QAction ( tr ( "&New" ), this );
  newAct->setShortcuts ( QKeySequence::New );
  newAct->setStatusTip ( tr ( "Create a new file" ) );
  connect ( newAct, SIGNAL ( triggered() ), this, SLOT ( newFile() ) );

  openAct = new QAction ( tr ( "&Open..." ), this );
  openAct->setShortcuts ( QKeySequence::Open );
  openAct->setStatusTip ( tr ( "Open an existing file" ) );
  connect ( openAct, SIGNAL ( triggered() ), this, SLOT ( open() ) );

  deleteAction = new QAction ( QIcon ( ":/images/delete.png" ),
      tr ( "&Delete" ), this );
  deleteAction->setShortcut ( tr ( "Delete" ) );
  deleteAction->setStatusTip ( tr ( "Delete item from diagram" ) );
  connect ( deleteAction, SIGNAL ( triggered() ),
            this, SLOT ( deleteItem() ) );

  exitAction = new QAction ( tr ( "E&xit" ), this );
  exitAction->setShortcuts ( QKeySequence::Quit );
  exitAction->setStatusTip ( tr ( "Quit Scenediagram example" ) );
  connect ( exitAction, SIGNAL ( triggered() ), this, SLOT ( close() ) );


  aboutAction = new QAction ( tr ( "A&bout" ), this );
  aboutAction->setShortcut ( tr ( "Ctrl+B" ) );
  connect ( aboutAction, SIGNAL ( triggered() ),
            this, SLOT ( about() ) );
}

void MainWindow::createMenus() {
  fileMenu = menuBar()->addMenu ( tr ( "&File" ) );
  fileMenu->addAction ( newAct );
  fileMenu->addAction ( openAct );
  fileMenu->addSeparator();
  fileMenu->addAction ( exitAction );

  aboutMenu = menuBar()->addMenu ( tr ( "&Help" ) );
  aboutMenu->addAction ( aboutAction );
}

void MainWindow::createToolbars() {
  editToolBar = addToolBar ( tr ( "Edit" ) );
  editToolBar->addAction ( deleteAction );

  fillColorToolButton = new QToolButton;
  fillColorToolButton->setPopupMode ( QToolButton::MenuButtonPopup );
  fillColorToolButton->setMenu ( createColorMenu ( SLOT ( itemColorChanged() ),
      Qt::white ) );
  fillAction = fillColorToolButton->menu()->defaultAction();
  fillColorToolButton->setIcon ( createColorToolButtonIcon (
        ":/images/floodfill.png", Qt::white ) );
  connect ( fillColorToolButton, SIGNAL ( clicked() ),
            this, SLOT ( fillButtonTriggered() ) );

  lineColorToolButton = new QToolButton;
  lineColorToolButton->setPopupMode ( QToolButton::MenuButtonPopup );
  lineColorToolButton->setMenu ( createColorMenu ( SLOT ( lineColorChanged() ),
      Qt::black ) );
  lineAction = lineColorToolButton->menu()->defaultAction();
  lineColorToolButton->setIcon ( createColorToolButtonIcon (
        ":/images/linecolor.png", Qt::black ) );
  connect ( lineColorToolButton, SIGNAL ( clicked() ),
            this, SLOT ( lineButtonTriggered() ) );

  colorToolBar = addToolBar ( tr ( "Color" ) );
  colorToolBar->addWidget ( fillColorToolButton );
  colorToolBar->addWidget ( lineColorToolButton );

  QToolButton* pointerButton = new QToolButton;
  pointerButton->setCheckable ( true );
  pointerButton->setChecked ( true );
  pointerButton->setIcon ( QIcon ( ":/images/pointer.png" ) );

  QToolButton* nodePointerButton = new QToolButton;
  nodePointerButton->setCheckable ( true );
  nodePointerButton->setIcon ( QIcon ( ":/images/nodepointer.png" ) );

  QToolButton* linePointerButton = new QToolButton;
  linePointerButton->setCheckable ( true );
  linePointerButton->setIcon ( QIcon ( ":/images/linepointer.png" ) );

  pointerTypeGroup = new QButtonGroup ( this );
  pointerTypeGroup->addButton ( pointerButton, int ( GraphWalkerScene::MoveItem ) );
  pointerTypeGroup->addButton ( nodePointerButton, int ( GraphWalkerScene::InsertItem ) );
  pointerTypeGroup->addButton ( linePointerButton, int ( GraphWalkerScene::InsertLine ) );
  connect ( pointerTypeGroup, SIGNAL ( buttonClicked ( int ) ),
            this, SLOT ( pointerGroupClicked ( int ) ) );

  sceneScaleCombo = new QComboBox;
  QStringList scales;
  scales << tr ( "50%" ) << tr ( "75%" ) << tr ( "100%" ) << tr ( "125%" ) << tr ( "150%" );
  sceneScaleCombo->addItems ( scales );
  sceneScaleCombo->setCurrentIndex ( 2 );
  connect ( sceneScaleCombo, SIGNAL ( currentIndexChanged ( QString ) ),
            this, SLOT ( sceneScaleChanged ( QString ) ) );

  pointerToolbar = addToolBar ( tr ( "Pointer type" ) );
  pointerToolbar->addWidget ( pointerButton );
  pointerToolbar->addWidget ( nodePointerButton );
  pointerToolbar->addWidget ( linePointerButton );
  pointerToolbar->addWidget ( sceneScaleCombo );
}

QWidget* MainWindow::createBackgroundCellWidget ( const QString& text,
    const QString& image ) {
  QToolButton* button = new QToolButton;
  button->setText ( text );
  button->setIcon ( QIcon ( image ) );
  button->setIconSize ( QSize ( 50, 50 ) );
  button->setCheckable ( true );
  backgroundButtonGroup->addButton ( button );

  QGridLayout* layout = new QGridLayout;
  layout->addWidget ( button, 0, 0, Qt::AlignHCenter );
  layout->addWidget ( new QLabel ( text ), 1, 0, Qt::AlignCenter );

  QWidget* widget = new QWidget;
  widget->setLayout ( layout );

  return widget;
}

QMenu* MainWindow::createColorMenu ( const char* slot, QColor defaultColor ) {
  QList<QColor> colors;
  colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
  QStringList names;
  names << tr ( "black" ) << tr ( "white" ) << tr ( "red" ) << tr ( "blue" )
        << tr ( "yellow" );

  QMenu* colorMenu = new QMenu ( this );

  for ( int i = 0; i < colors.count(); ++i ) {
    QAction* action = new QAction ( names.at ( i ), this );
    action->setData ( colors.at ( i ) );
    action->setIcon ( createColorIcon ( colors.at ( i ) ) );
    connect ( action, SIGNAL ( triggered() ),
              this, slot );
    colorMenu->addAction ( action );

    if ( colors.at ( i ) == defaultColor ) {
      colorMenu->setDefaultAction ( action );
    }
  }

  return colorMenu;
}

QIcon MainWindow::createColorToolButtonIcon ( const QString& imageFile,
    QColor color ) {
  QPixmap pixmap ( 50, 80 );
  pixmap.fill ( Qt::transparent );
  QPainter painter ( &pixmap );
  QPixmap image ( imageFile );
  QRect target ( 0, 0, 50, 60 );
  QRect source ( 0, 0, 42, 42 );
  painter.fillRect ( QRect ( 0, 60, 50, 80 ), color );
  painter.drawPixmap ( target, image, source );

  return QIcon ( pixmap );
}

QIcon MainWindow::createColorIcon ( QColor color ) {
  QPixmap pixmap ( 20, 20 );
  QPainter painter ( &pixmap );
  painter.setPen ( Qt::NoPen );
  painter.fillRect ( QRect ( 0, 0, 20, 20 ), color );

  return QIcon ( pixmap );
}

