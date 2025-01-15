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

#include"editarea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

pubilc:

    ui->setupUi(this);
    CreatAction();
    CreatToolBar();



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

    newFileAct = new QAction("New", this);
    newFileAct->setStatusTip("Create a new file");
    newFileAct->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    newFileAct->setShortcut(QKeySequence::New);
    // connect(newFileAct, &QAction::triggered, this, &MainWindow::createNewFile);

    openFileAct = new QAction("Open File", this);
    openFileAct->setStatusTip("Open an existing file");
    openFileAct->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    openFileAct->setShortcut(QKeySequence::Open);
    // connect(openFileAct, &QAction::triggered, this, &MainWindow::openFile);

    saveFileAct = new QAction("Save", this);
    saveFileAct->setStatusTip("Save the current file");
    saveFileAct->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    saveFileAct->setShortcut(QKeySequence::Save);
    // connect(saveFileAct, &QAction::triggered, this, &MainWindow::saveFile);

    openFolderAct = new QAction("Open Folder", this);
    openFolderAct->setStatusTip("Open a folder");
    openFolderAct->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    // openFolderAct->setShortcut()
    // connect(openFolderAct, &QAction::triggered, this, &MainWindow::openFolder);

    runAct = new QAction("Run", this);
    runAct->setStatusTip("Run the current file");
    runAct->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    runAct->setShortcut(QKeySequence("Ctrl+F5"));
    // connect(runAct, &QAction::triggered, this, &MainWindow::runCode);

    exitAct = new QAction("E&xit", this);
    exitAct->setStatusTip("Exit the application");
    connect(exitAct, &QAction::triggered, this, &QWidget::close);


}

void MainWindow::CreatToolBar()
{

    ///创建toolBar并添加action
    mainToolBar=addToolBar("MainToolBar");
    mainToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    mainToolBar->setIconSize(QSize(20,20));
    // mainToolBar->setBackgroundRole(QPalette::ToolTipBase);

    mainToolBar->setStyleSheet("background-color:rgb(46, 47, 48);");

    mainToolBar->addAction(openFileAct);
    mainToolBar->addAction(saveFileAct);
    mainToolBar->addAction(newFileAct);
    mainToolBar->addAction(openFolderAct);
    mainToolBar->addSeparator();

    // mainToolBar->addWidget(spacerWidget);


    AddSpaceWidget(12);

    mainToolBar->addSeparator();
    mainToolBar->addAction((runAct));
    mainToolBar->addSeparator();
    mainToolBar->addSeparator();
    mainToolBar->addSeparator();
    mainToolBar->addSeparator();
    mainToolBar->addSeparator();
    mainToolBar->addSeparator();



}

void MainWindow::AddSpaceWidget(int n)
{
    ///设置分隔widget
    /// 创建一个带有固定宽度 Spacer 的 QWidget
    QWidget *spacerWidget;///分隔widget
    QHBoxLayout *spacerLayout;
    for(int i=0;i<n;i++){
        spacerWidget = new QWidget();
        spacerLayout = new QHBoxLayout(spacerWidget);
        spacerLayout->setContentsMargins(0, 0, 0, 0); // 没有边距
        spacerLayout->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Expanding));
        mainToolBar->addWidget(spacerWidget);
    }
}
