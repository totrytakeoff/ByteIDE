#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QHBoxLayout;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


private slots:


    // void openFile();
    // void saveFile();
    // void openFolder();
    // void createNewFile();
    // void onFileTreeClicked(const QModelIndex &index);
    // void onTabClose(int index);
    // void toggleFileExplorer(bool show);
    // void toggleTerminal(bool show);
    // void about();
    // void runCode();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void CreatAction();
    void CreatToolBar();
    void CreatMenuBar();
    void CreatEdit();



    void SetStyles();

private:
    Ui::MainWindow *ui;


    QAction* newFileAct;
    QAction* openFileAct;
    QAction* saveFileAct;
    QAction* openFolderAct;
    QAction* exitAct;
    QAction* aboutAct;

    QAction* showFileExplorerAct;
    QAction* showTerminalAct;

    QAction* runAct;

    QTabWidget *codeTabWidget;
    QToolBar *mainToolBar;

    QDockWidget *FileTree;
    QDockWidget *TerminalView;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;


};
#endif // MAINWINDOW_H
