#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <Qsci/qscilexer.h>
#include<Qsci/qscilexercpp.h>
#include <Qsci/qsciscintilla.h>

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPoint>

#include <QSettings>///读取，保存用户个性配置

#include <QSize>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>
#include <QLabel>
#include <QStyle>
#include <QHBoxLayout>
#include <QPalette>
#include <QToolButton>
#include <QTextEdit>
#include <QDockWidget>

#include "resourcemanager.h"
#include "editarea.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

pubilc:

    ui->setupUi(this);
    CreatAction();
    CreatToolBar();
    CreatMenuBar();
    CreatDock();
    SetStyles();

    // editarea=new EditArea();

    codeTabWidget=new QTabWidget(this);
    codeTabWidget->clear();
    codeTabWidget->setTabsClosable(true);

    connect(codeTabWidget,&QTabWidget::tabCloseRequested,this,&MainWindow::onTabClose);
    connect(codeTabWidget,&QTabWidget::currentChanged,this,&MainWindow::onTabChange);
    setCentralWidget(codeTabWidget);





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreatAction()
{

    //// Add actions that will be used in toolbar and menus

    newFileAct = new QAction("&新建文件", this);
    newFileAct->setStatusTip("创建一个新文件");
    newFileAct->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    newFileAct->setShortcut(QKeySequence::New);
    connect(newFileAct, &QAction::triggered, this, &MainWindow::createNewFile);

    openFileAct = new QAction("&打开文件", this);
    openFileAct->setStatusTip("打开一个已经存在的文件");
    openFileAct->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    openFileAct->setShortcut(QKeySequence::Open);
    connect(openFileAct, &QAction::triggered, this, &MainWindow::openFile);

    saveFileAct = new QAction("&保存", this);
    saveFileAct->setStatusTip("保存当前文件");
    saveFileAct->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    saveFileAct->setShortcut(QKeySequence::Save);

    connect(saveFileAct, &QAction::triggered, this, &MainWindow::saveCurFile);

    openFolderAct = new QAction("&打开文件夹", this);
    openFolderAct->setStatusTip("打开一个文件夹并在文件管理器中显示");
    openFolderAct->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    // openFolderAct->setShortcut()
    // connect(openFolderAct, &QAction::triggered, this, &MainWindow::openFolder);

    runAct = new QAction("&运行", this);
    runAct->setStatusTip("保存并运行当前文件");
    runAct->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    runAct->setShortcut(QKeySequence("Ctrl+F5"));
    // connect(runAct, &QAction::triggered, this, &MainWindow::runCode);

    exitAct = new QAction("&退出", this);
    exitAct->setStatusTip("退出应用程序");
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    showFileExplorerAct = new QAction("文件资源管理器", this);
    showFileExplorerAct->setCheckable(true);
    showFileExplorerAct->setChecked(true);
    // connect(showFileExplorerAct, &QAction::triggered, this, &MainWindow::toggleFileExplorer);


    showTerminalAct = new QAction("Terminal(终端)", this);
    showTerminalAct->setCheckable(true);
    showTerminalAct->setChecked(true);
    // connect(showTerminalAct, &QAction::triggered, this, &MainWindow::toggleTerminal);


    aboutAct = new QAction("&关于", this);
    // connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

}

void MainWindow::CreatToolBar()
{

    ///创建toolBar并添加action
    mainToolBar=addToolBar("MainToolBar");
    mainToolBar->setIconSize(QSize(20,20));
    mainToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);///

    mainToolBar->addAction(openFileAct);
    mainToolBar->addAction(saveFileAct);
    mainToolBar->addAction(newFileAct);
    mainToolBar->addAction(openFolderAct);
    mainToolBar->addSeparator();

    // mainToolBar->addWidget(spacerWidget);


    QWidget *spacer = new QWidget();///添加占位widget把runact挤到右边去
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mainToolBar->addWidget(spacer);



    mainToolBar->addSeparator();
    mainToolBar->addAction((runAct));





}

void MainWindow::CreatMenuBar()
{
    // File Menu
    fileMenu = menuBar()->addMenu("&文件");
    fileMenu->addAction(newFileAct);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(saveFileAct);
    fileMenu->addAction(openFolderAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    // Edit Menu
    editMenu = menuBar()->addMenu("&编辑");

    editMenu->addAction("剪切");
    editMenu->addAction("复制");
    editMenu->addAction("粘贴");
    editMenu->addAction("撤销");
    editMenu->addAction("重做");

    // View Menu
    viewMenu = menuBar()->addMenu("&视图");
    viewMenu->addAction(showFileExplorerAct);
    viewMenu->addAction(showTerminalAct);

    // Help Menu
    helpMenu = menuBar()->addMenu("&帮助");
    helpMenu->addAction(aboutAct);

}

void MainWindow::CreatDock()
{
    ///creat a file dock
    fileDock=new QDockWidget("File Explorer",this);
    fileDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    ///creat a resourceManager and set fileDock'widget with fileExolorer->treeView
    fileExplorer=new ResourceManager(fileDock);
    addDockWidget(Qt::LeftDockWidgetArea, fileDock);



    // // Terminal dock
    // terminalDock = new QDockWidget("Terminal", this);
    // terminalDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);

    // terminal = new Terminal(this);
    // terminalDock->setWidget(terminal);
    // addDockWidget(Qt::BottomDockWidgetArea, terminalDock);

}

void MainWindow::setCurrentFile(const QString fileName)
{
    curFilePath=fileName;
    curEditArea->textEdit->setModified(false);
    // codeTabWidget->currentWidget()->setWindowModified(false);
    setWindowModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else{
        QFileInfo fileInfo(fileName);
        shownName=fileInfo.fileName();

    }
    qDebug()<<"curfile:"<<fileName;
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));

}

///设置相关样式
void MainWindow::SetStyles()
{


    QString styleSheet = R"(


        #runAct{
             background-color: red;
             color: blue;
            padding: 30px;
        }

        QMainWindow{
           background-color: rgb(80, 80, 80);
        }
        QMenuBar {
            background-color: rgb(60, 60, 60);
            color: rgb(240, 240, 240);
            border-bottom: 1px solid black;

        }
        QMenuBar::item:selected {
            background-color: rgb(80, 80, 80);

        }
        QMenu {
            background-color: rgb(60, 60, 60);
            color: rgb(240, 240, 240);
            border: 1px solid rgb(100, 100, 100);
        }
        QMenu::item:selected {
            background-color: rgb(80, 80, 80);
        }

        QToolBar {
            background-color: rgb(60, 60, 60);
            border: none;
        }

        QToolButton {
            background-color: rgb(60, 60, 60);
            border: none;
            padding: 3px;
        }

        QToolButton:hover {
            background-color: rgb(80, 80, 80);
        }

        QToolTip{
            background-color: rgb(60, 60, 60);
            color:white
        }

        /* 特定于 runAct 的样式 */



    )";

    /*
        QStatusBar {
            background-color: rgb(60, 60, 60);
            color: rgb(240, 240, 240);
        }
        QTabBar::tab {
            background-color: rgb(60, 60, 60);
            color: rgb(240, 240, 240);
            padding: 5px;
            border: 1px solid rgb(80, 80, 80);
        }
        QTabBar::tab:selected {
            background-color: rgb(80, 80, 80);
        }
        QTabBar::close-button {
            image: url(close.png);
        }
        QTabBar::close-button:hover {
            background: rgb(100, 100, 100);
        }
        QDockWidget {
            titlebar-close-icon: url(close.png);
            titlebar-normal-icon: url(float.png);
        }
        QDockWidget::title {
            background-color: rgb(60, 60, 60);
            color: rgb(240, 240, 240);
            padding-left: 5px;
        }
        QDockWidget::close-button, QDockWidget::float-button {
            background: rgb(80, 80, 80);
            padding: 2px;
        }
        QDockWidget::close-button:hover, QDockWidget::float-button:hover {
            background: rgb(100, 100, 100);
        }


*/



    setStyleSheet(styleSheet);
}

void MainWindow::loadFormFile(QString path)
{
    qDebug()<<"is loading:"<<path ;
    EditArea* editor=new EditArea(codeTabWidget);
    QFile file(path);


    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream fileStream(&file);
        editor->textEdit->clear();///清空Textedit

        QApplication::setOverrideCursor(Qt::WaitCursor);
        qDebug()<<"loadfile";
        editor->textEdit->setText(file.readAll());
        // editor->textEdit->parent();
        QApplication::restoreOverrideCursor();
        file.close();
        // QFileInfo(file).fileName();

        codeTabWidget->addTab(editor,"tab");
        qDebug()<<"?:"<<codeTabWidget->count();

        setCurrentFile(path);
    }
    else {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(path)
                                 .arg(file.errorString()));
        return;
    }

}

void MainWindow::openFile()
{
    QString path=QFileDialog::getOpenFileName(this,"打开一个文件",
                                                QDir::currentPath(),
                                                "C/C++(*.cpp *.c *.h);;python(*.py);;文本文件(*.txt);;所有文件(*.*)");

    if(path.isEmpty()){
        qDebug()<<"path is empty";
        return ;
    }

    loadFormFile(path);

}

void MainWindow::saveCurFile()
{
    saveFile(curFilePath);
}

void MainWindow::saveFile(QString fileName)
{
    // QString fileName(curFilePath);
    if(fileName.isEmpty()){
        fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                        "",
                                                        tr("Text files (*.txt);;All files (*)"));

    }
    if (fileName.isEmpty()) {
        return; // 用户取消了对话框
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << curEditArea->textEdit->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    qDebug()<<"have save the file";

}

void MainWindow::openFolder()
{

}

void MainWindow::createNewFile()
{

}

void MainWindow::onFileTreeClicked(const QModelIndex &index)
{

}

void MainWindow::onTabClose(int index)
{
    int idx=codeTabWidget->currentIndex();
    if (idx<0)
        return;

    saveFile(curEditArea->curEditFile);
    codeTabWidget->removeTab(idx);

}

void MainWindow::toggleFileExplorer(bool show)
{

}

void MainWindow::toggleTerminal(bool show)
{

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>Application</b> example demonstrates how to "
                          "write modern GUI applications using Qt, with a menu bar, "
                          "toolbars, and a status bar."));
}

void MainWindow::runCode()
{

}

void MainWindow::onTabChange()
{
    // curEditArea=qobject_cast<EditArea*>(codeTabWidget->currentWidget());
    setCurrentFile(curEditArea->curEditFile);

}
