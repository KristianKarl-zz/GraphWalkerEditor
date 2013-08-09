#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "VertexItem.h"

class GraphWalkerScene;
class GraphWalkerWidget;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow();
    MainWindow(const QString& graph_file);

  private slots:
    void open();

  private:
    void initialize();
    void createActions();
    void createMenus();

    GraphWalkerWidget* widget;

    QAction* openAct;
    QAction* exitAction;

    QAction* hierarchicalLayoutAction;
    QAction* energyBasedLayoutAction;
    QAction* orthogonalLayoutAction;

    QAction* aboutAction;

    QMenu* fileMenu;
    QMenu* layoutMenu;

    QFileInfo currentFile;
};

#endif
