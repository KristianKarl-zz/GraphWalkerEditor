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

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow();
    MainWindow ( const QString& graph_file );

  private slots:
    void open();
    void deleteItem();
    void pointerGroupClicked ( int id );
    void itemInserted ( VertexItem* item );
    void sceneScaleChanged ( const QString& scale );
    void itemColorChanged();
    void lineColorChanged();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void about();

  private:
    void initialize();
    void createActions();
    void createMenus();
    void createToolbars();
    QWidget* createBackgroundCellWidget ( const QString& text,
        const QString& image );
    QWidget* createCellWidget ( const QString& text );
    QMenu* createColorMenu ( const char* slot, QColor defaultColor );
    QIcon createColorToolButtonIcon ( const QString& image, QColor color );
    QIcon createColorIcon ( QColor color );

    GraphWalkerScene* scene;
    GraphWalkerWidget* view;

    QAction* newAct;
    QAction* openAct;
    QAction* exitAction;

    QAction* hierarchicalLayoutAction;

    QAction* addAction;
    QAction* deleteAction;

    QAction* aboutAction;

    QMenu* fileMenu;
    QMenu* layoutMenu;
    QMenu* aboutMenu;

    QToolBar* editToolBar;
    QToolBar* colorToolBar;
    QToolBar* pointerToolbar;

    QComboBox* sceneScaleCombo;
    QComboBox* itemColorCombo;

    QToolBox* toolBox;
    QButtonGroup* pointerTypeGroup;
    QButtonGroup* backgroundButtonGroup;
    QToolButton* fontColorToolButton;
    QToolButton* fillColorToolButton;
    QToolButton* lineColorToolButton;
    QAction* fillAction;
    QAction* lineAction;

    QFileInfo currentFile;

};

#endif
