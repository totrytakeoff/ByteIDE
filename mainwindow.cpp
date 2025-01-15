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


#include"editarea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

pubilc:

    ui->setupUi(this);
    CreatAction();
    CreatToolBar();
    CreatMenuBar();
    SetStyles();

    EditArea* editarea=new EditArea();

    QPalette palette = mainToolBar->palette();
    palette.setColor(QPalette::Window, QColor("lightblue"));
    mainToolBar->setPalette(palette);
    mainToolBar->setAutoFillBackground(true); // 确保背景颜色被填充

    setCentralWidget(editarea->textEdit);





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
    // connect(newFileAct, &QAction::triggered, this, &MainWindow::createNewFile);

    openFileAct = new QAction("&打开文件", this);
    openFileAct->setStatusTip("打开一个已经存在的文件");
    openFileAct->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    openFileAct->setShortcut(QKeySequence::Open);
    // connect(openFileAct, &QAction::triggered, this, &MainWindow::openFile);

    saveFileAct = new QAction("&保存", this);
    saveFileAct->setStatusTip("保存当前文件");
    saveFileAct->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    saveFileAct->setShortcut(QKeySequence::Save);
    // connect(saveFileAct, &QAction::triggered, this, &MainWindow::saveFile);

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
