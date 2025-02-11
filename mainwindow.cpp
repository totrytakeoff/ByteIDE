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
#include <QLabel>

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
#include <QSplitter>
#include <QLayout>
#include <QGridLayout>
#include <QVBoxLayout>


#include "newfile.h"
#include "resourcemanager.h"
#include "editarea.h"
#include "terminal.h"
#include "coderunner.h"
#include "searchwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

pubilc:

    QWidget *centralWidget = new QWidget(this); // 创建一个新的QWidget作为中央部件
    mainLayout = new QVBoxLayout(centralWidget); // 为中央部件设置垂直布局





    runner=new CodeRunner(this);
    searchDia=new SearchWidget(this);
    searchDia->hide();
    codeTabWidget=new QTabWidget(this);
    codeTabWidget->clear();///清空tabwidget内Tab
    codeTabWidget->setTabsClosable(true);///启用Tab关闭btn


    // 将codeTabWidget添加到mainLayout中，并设置适当的拉伸因子以让它占据剩余空间
    mainLayout->addWidget(codeTabWidget, 1); // 使用拉伸因子1让codeTabWidget尽可能多地占据空间
    mainLayout->setContentsMargins(0,0,0,0);


    // 设置centralWidget为主窗口的中心部件
    setCentralWidget(centralWidget);

    CreatAction();
    CreatToolBar();
    CreatMenuBar();
    CreatDock();
    CreatStatusBar();
    SetStyles();

    connect(codeTabWidget,&QTabWidget::tabCloseRequested,this,&MainWindow::onTabClose);
    connect(codeTabWidget,&QTabWidget::currentChanged,this,&MainWindow::onTabChange);

    connect(fileExplorer,&ResourceManager::fileDoubleClick,this,&MainWindow::onFileTreeClicked);

    connect(searchDia,&SearchWidget::findNext,this,[=](const QString &text, bool caseSensitive, bool wholeWords){
        qDebug()<<"click search";
        if(curEditArea){
            curEditArea->findNext(text,caseSensitive,wholeWords);
        }
    });

    connect(searchDia,&SearchWidget::inputSearchWord,this,[=](const QString &text){
        qDebug()<<"highlightAlll";
        if(curEditArea){
            curEditArea->highLightAll(text);
        }
    });





    this->resize(1000,800);

    // setCentralWidget(codeTabWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreatAction()
{

    //// Add actions that will be used in toolbar and menus
    addEmptyFileAct=new QAction("&新建一个临时文本文件",this);
    connect(addEmptyFileAct,&QAction::triggered, this,[this](){
        EditArea* editor=new EditArea(codeTabWidget);
        editor->curEditFile="";///记录打开文件路径
        curEditArea=editor;///记录当前Tab所对应的EditArea,注意curEditArea空指针访问问题
        codeTabWidget->addTab(editor,"untitled.txt");///添加Tab页面
        // qDebug()<<"Tab_count:"<<codeTabWidget->count();
        codeTabWidget->setCurrentIndex(codeTabWidget->count()-1);

        // qDebug()<<"path::"<<path;
        setCurrentFile("");///更新当前Tab对应文件
    });

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

    saveAsFileAct = new QAction("&另存为", this);
    saveAsFileAct->setStatusTip("另存为...");
    connect(saveAsFileAct,&QAction::triggered,this,[this](){
        saveFile("");
    });


    openFolderAct = new QAction("&打开文件夹", this);
    openFolderAct->setStatusTip("打开一个文件夹并在文件管理器中显示");
    openFolderAct->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    // openFolderAct->setShortcut()
    connect(openFolderAct, &QAction::triggered, this, &MainWindow::openFolder);


    runAct = new QAction("&运行", this);
    runAct->setStatusTip("保存并运行当前文件");
    runAct->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    runAct->setShortcut(QKeySequence("Ctrl+F5"));
    runAct->setEnabled(false);
    connect(runAct, &QAction::triggered, this, &MainWindow::runCode);

    exitAct = new QAction("&退出", this);
    exitAct->setStatusTip("退出应用程序");
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    showFileExplorerAct = new QAction("文件资源管理器", this);
    connect(showFileExplorerAct, &QAction::triggered, this, &MainWindow::toggleFileExplorer);


    showTerminalAct = new QAction("Terminal(终端)", this);
    showTerminalAct->setShortcut(QKeySequence("Ctrl+`"));
    connect(showTerminalAct, &QAction::triggered, this, &MainWindow::toggleTerminal);


    aboutAct = new QAction("&关于", this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    // curEditArea->textEdit->redo();


    redoAct = new QAction("重做", this);
    redoAct->setShortcut(QKeySequence::Redo);  // 快捷键：Ctrl + Y 或 Ctrl + Shift + Z


    undoAct = new QAction("撤销", this);
    undoAct->setShortcut(QKeySequence::Undo);  // 快捷键：Ctrl + Z


    cutAct = new QAction("剪切", this);
    cutAct->setShortcut(QKeySequence::Cut);  // 快捷键：Ctrl + X


    copyAct = new QAction("复制", this);
    copyAct->setShortcut(QKeySequence::Copy);  // 快捷键：Ctrl + C


    pasteAct = new QAction("粘贴", this);
    pasteAct->setShortcut(QKeySequence::Paste);  // 快捷键：Ctrl + V


    selectAllAct = new QAction("全选", this);
    selectAllAct->setShortcut(QKeySequence::SelectAll);  // 快捷键：Ctrl + A


    cancelSelectAct = new QAction("取消选择", this);
    cancelSelectAct->setShortcut(Qt::Key_Escape);  // 快捷键：Esc


    foldAllAct = new QAction("折叠全部", this);
    foldAllAct->setShortcut(QKeySequence("Ctrl+Shift+["));  // 自定义快捷键：Ctrl + Shift + [


    openAllAct = new QAction("展开全部", this);
    openAllAct->setShortcut(QKeySequence("Ctrl+Shift+]"));  // 自定义快捷键：Ctrl + Shift + ]





    // 连接槽函数
    connect(redoAct, &QAction::triggered, this, [this](){
        if(curEditArea)
            curEditArea->textEdit->redo();
    });

    connect(undoAct, &QAction::triggered, this, [this](){
        if(curEditArea)
            curEditArea->textEdit->undo();
    });

    connect(cutAct, &QAction::triggered, this, [this](){
        if(curEditArea)
            curEditArea->textEdit->cut();
    });

    connect(copyAct, &QAction::triggered, this, [this](){
        if(curEditArea)
            curEditArea->textEdit->copy();
    });

    connect(pasteAct, &QAction::triggered, this, [this](){
        if(curEditArea)
            curEditArea->textEdit->paste();
    });

    connect(selectAllAct, &QAction::triggered, this, [this](){
        if(curEditArea)
            curEditArea->textEdit->selectAll(true);
    });

    connect(cancelSelectAct, &QAction::triggered, this, [this](){
        if(curEditArea)
            curEditArea->textEdit->selectAll(false);
    });

    connect(foldAllAct, &QAction::triggered, this, [this](){
        if(curEditArea)
            curEditArea->textEdit->foldAll(true);
    });

    connect(openAllAct, &QAction::triggered, this, [this](){
        if(curEditArea)
            curEditArea->textEdit->foldAll(false);
    });

    searchTextAct=new QAction("查找",this);
    searchTextAct->setShortcut(QKeySequence("Ctrl+F"));
    connect(searchTextAct,&QAction::triggered,this,[this](){
        qDebug()<<"searchwidget1";
        if (!searchDia->isVisible()) { // 如果searchWidget不可见，则显示它
            qDebug()<<"searchwidget2";
            mainLayout->addWidget(searchDia, 0); // 添加searchWidget到布局，使用0拉伸因子避免占据过多空间
            searchDia->show();
            mainLayout->update(); // 更新布局以反映更改
        } else {
            qDebug()<<"searchwidget3";
            mainLayout->removeWidget(searchDia); // 如果searchWidget已经可见，则从布局中移除
            searchDia->hide();
            mainLayout->update(); // 更新布局以反映更改
        }
    });

    setEditActEnable(false);///无Tab时设置为禁用状态
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
    fileMenu->addAction(addEmptyFileAct                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       );
    fileMenu->addAction(newFileAct);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(saveFileAct);
    fileMenu->addAction(saveAsFileAct);
    fileMenu->addAction(openFolderAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    // Edit Menu
    editMenu = menuBar()->addMenu("&编辑");

    editMenu->addAction(redoAct);
    editMenu->addAction(undoAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(selectAllAct);
    editMenu->addAction(cancelSelectAct);
    editMenu->addAction(foldAllAct);
    editMenu->addAction(openAllAct);
    editMenu->addAction(searchTextAct);
    // editMenu->addAction();


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
    setDockNestingEnabled(true);
    ///creat a file dock
    fileDock=new QDockWidget("File Explorer",this);
    fileDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);


    ///creat a resourceManager and set fileDock'widget with fileExolorer->treeView
    fileExplorer=new ResourceManager(fileDock,this);


    addDockWidget(Qt::LeftDockWidgetArea, fileDock);



    // // Terminal dock
    terminal=new Terminal(this);
    terminalViewDock= new QDockWidget("Terminal", this);
    terminalViewDock->setAllowedAreas(Qt::BottomDockWidgetArea);

    terminalViewDock->setWidget(terminal);

    addDockWidget(Qt::BottomDockWidgetArea,terminalViewDock);


    OutputDock=new QDockWidget("Output",this);
    OutputDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    OutputDock->setWidget(runner);

    addDockWidget(Qt::BottomDockWidgetArea,OutputDock);

    tabifyDockWidget(terminalViewDock,OutputDock);

}

void MainWindow::CreatStatusBar()
{
    QString style=R"(
    QLabel{
        color:rgb(220,220,220);
    }
    )";
    curLineLabel=new QLabel("行:",this);
    curLineLabel->setStyleSheet(style);

    curCharLabel=new QLabel("字符:",this);
    curCharLabel->setStyleSheet(style);


    statusBar()->addPermanentWidget(curLineLabel);
    statusBar()->addPermanentWidget(curCharLabel);

}

void MainWindow::setCurrentFile(const QString fileName)
{


    curFilePath=fileName;///记录当前所在文件



    setWindowModified(false);

    QString shownName;
    if (fileName.isEmpty()){
        shownName = "untitled.txt";
    }
    else{
        curEditArea->textEdit->setModified(false);//将modify标记设为false,为后序退出时保存弹窗做准备
        QFileInfo fileInfo(fileName);
        shownName=fileInfo.fileName();

    }
    qDebug()<<"curfile:"<<fileName;
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));///将窗口标题设为当前打开Tab

}

///设置相关样式
void MainWindow::SetStyles()
{


    QString styleSheet = R"(


        QListView {
            background-color: rgb(30,30,30);
            color: rgb(220,220,220);
            border: 1px solid gray;
            selection-background-color: blue;
            selection-color: white;
        }

        QMainWindow{
           background-color: rgb(80, 80, 80);
            border: 1px solid rgb(20,20,20);
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
        QMenu::item:disabled {
            background-color: rgb(60, 60, 60);
            color: rgb(150, 150, 150);
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

       QDialog{
            background-color: rgb(36, 36, 36);
            color:white;
        }
        NewFile{
            color:white;
        }
        QTabBar{
            background-color: rgb(80, 80, 80);
        }
        QTabBar::tab {
            background-color: rgb(60, 60, 60);
            color: rgb(240, 240, 240);
            padding: 5px;
            border: 1px solid black;
        }
        QTabBar::tab:selected {
            background-color: rgb(80, 80, 80);
        }

        QTabBar::close-button:hover {
            background: rgb(100, 100, 100);
        }
        QStatusBar {
            background-color: rgb(60, 60, 60);
            color: rgb(240, 240, 240);
        }
        QDockWidget {
            color:rgb(214,214,214);


        }
        QDockWidget::title {

            background-color: rgb(60, 60, 60);
            padding-left: 5px;
            border-top: 1px solid rgb(160,160,160);

        }
        QDockWidget::close-button, QDockWidget::float-button {
            background: rgb(100, 100, 100);
            padding: 2px;

        }
        QDockWidget::close-button:hover, QDockWidget::float-button:hover {
            background: rgb(100, 100, 100);
        }
        QMessageBox QLabel{
            color:white;
        }
        QTabWidget{
            background-color:rgb(60,60,60);
            margin: 0,0,0,0;
            padding 0,0,0,0;
        }

    )";

    /*


*/



    setStyleSheet(styleSheet);
}

void MainWindow::ShowFileDock()
{
    fileExplorer->SetCurPath(curFolderPath);

}

QString MainWindow::GetCurFileType()
{
    QFileInfo FileInfo(curFilePath);
    return FileInfo.suffix();
}

void MainWindow::setEditActEnable(bool b)
{
    redoAct->setEnabled(b);//重做
    undoAct->setEnabled(b);//撤销
    cutAct->setEnabled(b);
    copyAct->setEnabled(b);
    pasteAct->setEnabled(b);
    selectAllAct->setEnabled(b);
    cancelSelectAct->setEnabled(b);
    foldAllAct->setEnabled(b);
    openAllAct->setEnabled(b);
    searchTextAct->setEnabled(b);
    runAct->setEnabled(b);
}

void MainWindow::loadFromFile(QString path)
{
    qDebug()<<"is loading:"<<path ;
    EditArea* editor=new EditArea(codeTabWidget);
    editor->curEditFile=path;///记录打开文件路径
    curEditArea=editor;///记录当前Tab所对应的EditArea,注意curEditArea空指针访问问题

    QFile file(path);


    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream fileStream(&file);
        editor->textEdit->clear();///清空Textedit

        QApplication::setOverrideCursor(Qt::WaitCursor);///鼠标转圈圈
        editor->textEdit->setText(file.readAll());
        QApplication::restoreOverrideCursor();///鼠标恢复
        file.close();


        codeTabWidget->addTab(editor,QFileInfo(file).fileName());///添加Tab页面
        // qDebug()<<"Tab_count:"<<codeTabWidget->count();
        setEditActEnable(true);///将editAct设为可用状态

        codeTabWidget->setCurrentIndex(codeTabWidget->count()-1);

        // qDebug()<<"path::"<<path;
        setCurrentFile(path);///更新当前Tab对应文件
        //更新当前文件类型，更改textEdit的Lexer
        curFileType=GetCurFileType();
        curEditArea->setCurLexer(curFileType);
    }
    else {
        QMessageBox::warning(this, tr("error"),
                             tr("不能读取文件 %1:\n%2.")
                                 .arg(path)
                                 .arg(file.errorString()));
        return;
    }

}

void MainWindow::openFile()
{
    QString path=QFileDialog::getOpenFileName(this,"打开一个文件",
                                                QDir::currentPath(),
                                                "所有文件(*.*);;C/C++(*.cpp *.c *.h);;python(*.py);;文本文件(*.txt)");

    if(path.isEmpty()){
        qDebug()<<"path is empty";
        return ;
    }

    loadFromFile(path);

}

void MainWindow::saveCurFile()
{
    saveFile(curFilePath);
}

void MainWindow::saveFile(QString fileName)
{
    ///判断文件路径是否为空，若为空则先调用保存文件框
    if(fileName.isEmpty()){
        fileName = QFileDialog::getSaveFileName(this, tr("另存为..."),
                                                        "",
                                                        tr("所有文件(*.*);;C/C++(*.cpp *.c *.h);;python(*.py);;文本文件(*.txt)"));

        if (fileName.isEmpty()) {
            return; // 用户取消了对话框
        }
        curFilePath=fileName;
        if(curEditArea){
            curEditArea->curEditFile=fileName;
        }
        codeTabWidget->setTabText(codeTabWidget->currentIndex(),QFileInfo(fileName).fileName());
    }


    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("error"),
                             tr("无法写入文件 %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(curEditArea->textEdit){
        out << curEditArea->textEdit->text();
    }
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("文件已保存"), 2000);///设置状态栏保存成功
    isModified=false;
    qDebug()<<"have save the file";

}

void MainWindow::openFolder()
{
    QString path=QFileDialog::getExistingDirectory(this,"打开文件夹",QDir::currentPath());
    if(path.isEmpty()){
        qDebug()<<"the folder path is empty";
        return;
    }
    curFolderPath=path;
    ShowFileDock();

}

void MainWindow::createNewFile()
{


    QString Dir=QDir::currentPath();
    NewFile* newFile=new NewFile(this,Dir);
    if(newFile->exec()==QDialog::Accepted){
        QString filePath=newFile->GetNewFile();
        if(filePath.isEmpty()){
            qDebug()<<"newfile is empty";
            return;
        }
        loadFromFile(filePath);


    }



}

void MainWindow::onFileTreeClicked(const QString FileName)
{

    if(FileName==curFilePath){

        qDebug()<<"clicked exist file tab";
        // codeTabWidget->setCurrentIndex(0);
        return ;
    }
    qDebug()<<"onfileTreeClicked:"<<FileName;
    loadFromFile(FileName);

}

void MainWindow::onTabClose(int index)
{
    int idx=codeTabWidget->currentIndex();
    if (idx<0)
        return;

    if(curEditArea->curEditFile.isEmpty()){///另存为保存提示
        if (isModified) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "保存文件", "文件已修改，是否保存？",
                                          QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

            if (reply == QMessageBox::Save) {
                // 保存文件
                saveCurFile();
            }

        }
    }else{
        saveFile(curEditArea->curEditFile);
    }


    // 获取要删除的 EditArea 对象
    EditArea* editAreaToDelete = qobject_cast<EditArea*>(codeTabWidget->widget(idx));
    if (!editAreaToDelete) {
        return; // 如果没有有效的 EditArea，直接返回
    }

    // 移除 Tab
    codeTabWidget->removeTab(idx);

    // 安全地删除 EditArea 对象
    editAreaToDelete->deleteLater();

    // 如果当前 Tab 是最后一个，将 curEditArea 置空（tabchange中处理）

    statusBar()->showMessage(tr("文件已自动保存并关闭"), 2000);
}

void MainWindow::toggleFileExplorer(bool show)
{
    fileDock->show();
    qDebug()<<"show file dock";


}

void MainWindow::toggleTerminal(bool show)
{
    terminalViewDock->show();
    qDebug()<<"show terminal dock";

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("关于软件"),
                       tr("<h3>ByteIDE</h3>"
                          "<div style='font-size: 14px;'>"
                          "<p>ByteIDE 是一个基于 <b>Qt</b> 开发的简易文本编辑器和简单的集成开发环境 (IDE)。"
                          "它使用了 <b>QScintilla</b> 库来实现编辑器功能，提供了基本的文本编辑功能和常用的快捷键。</p>"
                          "<p>该软件集成了文件资源管理器和终端界面，能够识别并运行 <b>Python</b> 和 <b>C++</b> 文件。"
                          "此外，MyIDE 还支持 <b>Python</b> 和 <b>C++</b> 的语法高亮和自动补全功能，"
                          "使用户无需进行复杂的安装和配置，开箱即用，非常适合 <b>Python</b> 和 <b>C/C++</b> 初学者使用。</p>"
                          "<p>项目已上传<a href= \"https://github.com/totrytakeoff/IDE-based-on-Qt\" >github仓库</a>,欢迎大家点点star或者提issue</p>"
                          "</div>"
                          "</br></br></br></br></br></br></br></br></br></br></br></br></br>"
                          "<div width=100px; height=100px;></div>"
                          "<div style='font-size: 8px; color: gray;'>"
                          "<p>版本: 1.0.0</p>"
                          "<p>作者: myself</p>"
                          "<p>反馈邮箱: 2467315534@qq.com</p>"

                          "</div>"
                          ));
}


void MainWindow::runCode()
{


    if(codeTabWidget->count() <= 0){
        QMessageBox::warning(this,"warning!","请先打开一个文件再运行！");
        return;
    }
    saveCurFile();
    QString filetype=GetCurFileType();

    // qDebug()<<"the curfile with file type :"<<curFilePath<<" "<<filetype;
    ///设置当前运行文件
    OutputDock->show();
    runner->setRunFile(curFilePath);

    ///设置运行模式(文件类型)
    runner->setMode(filetype);
    runner->runCode();


}

void MainWindow::onTabChange()
{
    QString path;

    ///注意处理空指针情况
    if(codeTabWidget->count()>0){

        setEditActEnable(true);
        curEditArea=qobject_cast<EditArea*>(codeTabWidget->currentWidget());
        path=curEditArea->curEditFile;

        //获取文件修改状态
        connect(curEditArea->textEdit,&QsciScintilla::textChanged,this,[this](){
            isModified=true;
        });

        connect(curEditArea->textEdit,&QsciScintilla::cursorPositionChanged,this,&MainWindow::updateStatusBar);

        //更新当前文件类型，更改textEdit的Lexer
        QString type=QFileInfo(path).suffix();
        curEditArea->setCurLexer(type);


    }else{

        setEditActEnable(false);///无Tab时设置为禁用状态

        ///当Tab=0时无现存curEditArea，将curEditArea置空
        curEditArea=nullptr;
        isModified=false;///将修改状态改为未修改;
        path="";
    }


    setCurrentFile(path);

}

void MainWindow::updateStatusBar(int line,int index)
{
    if(curEditArea){
        // curEditArea->textEdit->getCursorPosition(&curLineIndex,&curCharIndex);
        curLineIndex=line+1;
        curCharIndex=index;
        curLineLabel->setText(QString("行:%1").arg(curLineIndex));
        curCharLabel->setText(QString("字符:%1").arg(curCharIndex));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 如果文件已修改，弹出保存提示
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "保存文件", "文件已修改，是否保存？",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            // 保存文件
            saveCurFile();
            event->accept();  // 关闭窗口
        } else if (reply == QMessageBox::Discard) {
            // 不保存，直接关闭窗口
            event->accept();
        } else {
            // 取消关闭操作
            event->ignore();
        }
    } else {
        // 文件未修改，直接关闭窗口
        event->accept();
    }

}
