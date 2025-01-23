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

    void redoAction();          // 重做
    void undoAction();          // 撤销
    void cutAction();           // 剪切
    void copyAction();          // 复制
    void pasteAction();         // 粘贴
    void selectAllAction();     // 全选
    void cancelSelectAction();  // 取消选择
    void foldAllAction();       // 折叠全部
    void openAllAction();       // 展开全部

    void runCode();


    void onTabChange();


protected:
    void closeEvent(QCloseEvent* event) override;


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

    QAction* redoAct;//重做
    QAction* undoAct;//撤销
    QAction* cutAct;
    QAction* copyAct;
    QAction* pasteAct;
    QAction* selectAllAct;
    QAction* cancelSelectAct;
    QAction* foldAllAct;
    QAction* openAllAct;
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

    bool isModified=false;

    QSplitter* mainSplitter;


};
#endif // MAINWINDOW_H
