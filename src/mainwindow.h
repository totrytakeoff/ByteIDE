#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class ResourceManager;
class EditArea;
class Terminal;
class CodeRunner;
class SearchWidget;
class CodeTabWidget;

class QVBoxLayout;
class QFile;
class QSplitter;
class QLabel;


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
    void CreatStatusBar();
    void setCurrentFile(const QString fileName);

    void loadFromFile(QString path);///加载文件
    void saveFile(QString fileName);


    void SetStyles();
    void ShowFileDock();
    QString GetCurFileType();

private:
    void setEditActEnable(bool b);
private slots:

    void openFile();//打开文件,调用loadFromFile;

    void saveCurFile();
    void openFolder();
    void createNewFile();///创建一个新文件,调用newfile类实现新建文件界面
    void onFileTreeClicked(const QString FileName);
    void onTabClose(int index);
    void toggleFileExplorer(bool show);
    void toggleTerminal(bool show);
    void about();

    void runCode();
    void forceStopProcess();

    void onTabChange();

    void updateStatusBar(int line,int index);

    // void onFileRename();
    ///响应文件资源管理器右键菜单栏重命名操作对应的Tab修改

protected:
    void closeEvent(QCloseEvent* event) override;


private:
    Ui::MainWindow *ui;

    QVBoxLayout* mainLayout;
    EditArea* curEditArea;
    Terminal * terminal;
    CodeRunner* runner;
    SearchWidget *searchDia;
    CodeTabWidget *codeTabWidget;

    QAction* addEmptyFileAct;
    QAction* newFileAct;
    QAction* openFileAct;
    QAction* saveFileAct;
    QAction* saveAsFileAct;
    QAction* openFolderAct;
    QAction* exitAct;
    QAction* aboutAct;
    QAction *updateLogAct;


    QAction* redoAct;//重做
    QAction* undoAct;//撤销
    QAction* cutAct;
    QAction* copyAct;
    QAction* pasteAct;
    QAction* selectAllAct;
    QAction* cancelSelectAct;
    QAction* foldAllAct;
    QAction* openAllAct;

    QAction* searchTextAct;


    QAction* showFileExplorerAct;
    QAction* showTerminalAct;

    QAction* runAct;
    QAction *stopRunAct;

    QToolBar *mainToolBar;

    QDockWidget *fileDock;
    QDockWidget *terminalViewDock;
    QDockWidget *OutputDock;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    ResourceManager * fileExplorer;
    QString curFilePath;
    QString curFolderPath;
    QString curFileType;
    int curLineIndex=0;
    int curCharIndex=0;
    QLabel* curLineLabel;
    QLabel* curCharLabel;

    bool isModified=false;

    QSplitter* mainSplitter;


};
#endif // MAINWINDOW_H
