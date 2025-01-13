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

#include"editarea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

pubilc:

    ui->setupUi(this);
    EditArea* editarea=new EditArea();


    setCentralWidget(editarea->textEdit);



}

MainWindow::~MainWindow()
{
    delete ui;
}
