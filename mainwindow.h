#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QHBoxLayout;
class ResourceManager;
class EditArea;
class Terminal;
class CodeRunner;
class QFile;
class QSplitter;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void CreatAction();
    void CreatToolBar();
    void CreatMenuBar();
    void CreatEdit();
    void CreatDock();
    void setCurrentFile(const QString fileName);

    void saveFile(QString fileName);


    void SetStyles();
    void ShowFileDock();
    QString GetCurFileType();

private slots:

    void loadFormFile(QString path);
    void openFile();

    void saveCurFile();

    void openFolder();
    void createNewFile();
    void onFileTreeClicked(const QString FileName);
    void onTabClose(int index);
    void toggleFileExplorer(bool show);
    void toggleTerminal(bool show);
    void about();
    void runCode();

    void onTabChange();



private:
    Ui::MainWindow *ui;

    EditArea* curEditArea;
    Terminal * terminal;
    CodeRunner* runner;

    QAction* newFileAct;
    QAction* openFileAct;
    QAction* saveFileAct;
    QAction* openFolderAct;
    QAction* exitAct;
    QAction* aboutAct;

    QAction* redo;//重做
    QAction* undo;//撤销
    QAction* cut;
    QAction* copy;
    QAction* paste;
    QAction* seletAll;
    QAction* cancelSelect;
    QAction* foldAll;
    QAction* openAll;
    // QAction* ;
    // QAction* ;
    // QAction* ;

    QAction* showFileExplorerAct;
    QAction* showTerminalAct;

    QAction* runAct;

    QTabWidget *codeTabWidget;
    QToolBar *mainToolBar;

    QDockWidget *fileDock;
    QDockWidget *terminalViewDock;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    ResourceManager * fileExplorer;
    QString curFilePath;
    QString curFolderPath;

    QSplitter* mainSplitter;


};
#endif // MAINWINDOW_H
