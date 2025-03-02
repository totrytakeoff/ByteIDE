#include "mainwindow.h"


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
#include <QSettings>
#include <QShortcut>


#include "newfile.h"
#include "resourcemanager.h"
#include "editarea.h"
#include "terminal.h"
#include "coderunner.h"
#include "searchwidget.h"
#include "codetabwidget.h"
#include "updatelogwidget.h"
#include "settingwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{



    QWidget *centralWidget = new QWidget(this); // 创建一个新的QWidget作为中央部件
    mainLayout = new QVBoxLayout(centralWidget); // 为中央部件设置垂直布局

    settings=new QSettings();

    runner=new CodeRunner(this);

    searchDia=new SearchWidget(this);
    searchDia->hide();

    codeTabWidget=new CodeTabWidget(this);
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

    codeTabWidget->bindFileExplorer(fileExplorer);

    connect(codeTabWidget,&QTabWidget::tabCloseRequested,this,&MainWindow::onTabClose);
    connect(codeTabWidget,&QTabWidget::currentChanged,this,&MainWindow::onTabChange);
    connect(codeTabWidget,&CodeTabWidget::haveRenameFile,this,[this](QString renameFilePath){
        curFilePath=renameFilePath;
        //更新当前文件类型，更改textEdit的Lexer
        QString type=QFileInfo(curFilePath).suffix();
        curEditArea->setCurLexer(type);
    });


    connect(fileExplorer,&ResourceManager::fileDoubleClick,this,&MainWindow::onFileTreeClicked);

    ///查找替换相关信号槽连接
    connect(searchDia,&SearchWidget::findNext,this,[=](const QString &text, bool caseSensitive, bool wholeWords,bool forward){
        if(curEditArea){
            curEditArea->findNext(text,caseSensitive,wholeWords,forward);
        }
    });

    connect(searchDia,&SearchWidget::inputSearchWord,this,[=](const QString &text){
        if(curEditArea){
            curEditArea->highLightAll(text);
        }
    });

    connect(searchDia,&SearchWidget::replace,this,[=](const QString &origin,const QString &replaced){
        if(curEditArea){
            curEditArea->replaceText(origin,replaced);
        }
    });

    connect(searchDia,&SearchWidget::replaceAll,this,[=](const QString &origin,const QString &replaced){
        if(curEditArea){
            curEditArea->replaceAll(origin,replaced);
        }
    });

    connect(searchDia,&SearchWidget::selectAll,this,[=](const QString &text){
        if(curEditArea){
            curEditArea->selectAllText(text);
        }
    });


    /// 运行相关信号槽
    connect(runner,&CodeRunner::processExit,this,[=](){
        stopRunAct->setEnabled(true);
    });
    connect(runner,&CodeRunner::startRunningCode,this,[=](){
        ///开始运行后设置stopBtn为可用状态
        stopRunAct->setEnabled(true);
    });





    this->resize(1000,800);


    ///第一次打开新版本软件显示更新日志
    const QString version="1.2.0";

    QString lastVersion = settings->value("Version/last").toString();
    if(lastVersion.isEmpty()||lastVersion!=version){
        settings->setValue("Version/last",version);
        ShowUpdataLog();
    }


}

MainWindow::~MainWindow()
{
}

void MainWindow::CreatAction()
{

    //// Add actions that will be used in toolbar and menus
    addEmptyFileAct=new QAction("&新建一个临时文本文件",this);

    addEmptyFileAct->setIcon(style()->standardIcon(QStyle::SP_FileIcon));

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
    runAct->setIcon(QIcon(":/img/runningBtn.png"));
    runAct->setShortcut(QKeySequence("Ctrl+F5"));
    runAct->setEnabled(false);
    connect(runAct, &QAction::triggered, this, &MainWindow::runCode);


    stopRunAct=new QAction("&终止运行",this);
    stopRunAct->setStatusTip("强制终止当前运行的进程");
    stopRunAct->setIcon(QIcon(":/img/stopBtn.png"));
    runAct->setShortcut(QKeySequence("Ctrl+C"));//这玩意会不会影响copy
    stopRunAct->setEnabled(false);
    connect(stopRunAct,&QAction::triggered,this,&MainWindow::forceStopProcess);



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

    updateLogAct =new QAction("&软件更新日志",this);
    connect(updateLogAct,&QAction::triggered,this,&MainWindow::ShowUpdataLog);


    settingAct = new QAction("&设置", this);
    connect(settingAct, &QAction::triggered, this, [this]() {
        SettingWidget* sett = new SettingWidget();
        settings = &sett->settings;
        connect(sett, &SettingWidget::save, this, &MainWindow::SetStyles);

        // 直接设置样式
        QString styleSheet = this->styleSheet();
        sett->setStyleSheet(styleSheet);

        sett->show();
    });


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


    resetTerAct=new QAction("重启终端",this);
    connect(resetTerAct,&QAction::triggered,this,[this](){
        qDebug()<<"rs";
        terminal->deleteLater();
        terminal=new Terminal(this);
        terminalViewDock->setWidget(terminal);
    });



    commentAct=new QAction("注释",this);
    commentAct->setShortcut(QKeySequence("Ctrl+Shift+/"));

///////////////////////////////////////////////////////////////
    setEditActEnable(false);///无Tab时设置为禁用状态
}

void MainWindow::CreatToolBar()
{

    ///创建toolBar并添加action
    mainToolBar=addToolBar("MainToolBar");
    mainToolBar->setIconSize(QSize(20,20));
    mainToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);///

    mainToolBar->addAction(addEmptyFileAct);
    mainToolBar->addAction(openFileAct);
    mainToolBar->addAction(saveFileAct);
    mainToolBar->addAction(openFolderAct);
    mainToolBar->addAction(settingAct);

    mainToolBar->addSeparator();


    mainToolBar->addSeparator();
    mainToolBar->addAction((runAct));
    mainToolBar->addAction(stopRunAct);



}

void MainWindow::CreatMenuBar()
{

    QMenu *temp=menuBar()->addMenu("菜单栏: ");
    temp->setDisabled(true);
    // temp=menuBar()->addMenu("|||||") ;

    fileMenu = menuBar()->addMenu("&文 件");

    fileMenu->addAction(addEmptyFileAct);
    fileMenu->addAction(newFileAct);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(saveFileAct);
    fileMenu->addAction(saveAsFileAct);
    fileMenu->addAction(openFolderAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    // Edit Menu
    editMenu = menuBar()->addMenu("&编 辑");

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
    editMenu->addAction(commentAct);


    // View Menu
    viewMenu = menuBar()->addMenu("&视 图");
    viewMenu->addAction(showFileExplorerAct);
    viewMenu->addAction(showTerminalAct);
    viewMenu->addAction(resetTerAct);

    // Help Menu
    optionMenu = menuBar()->addMenu("&选项");
    optionMenu->addAction(settingAct);
    optionMenu->addAction(aboutAct);
    optionMenu->addAction((updateLogAct));

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

    ///默认关闭
    fileDock->close();
    // fileDock->setVisible(false);


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

    //QColor(116, 185, 255) out

    qDebug()<<"is seting style";

    QString Theme;
    Theme=settings->value("UI/Theme","dark").value<QString>();

    for(int i=0;i<codeTabWidget->count();i++){
        qobject_cast<EditArea*>(codeTabWidget->currentWidget())->setTheme(Theme);
    }

    m_Theme=Theme;
    if(Theme=="dark"){
        settingAct->setIcon(QIcon(":/img/setting_light.png"));
        terminal->setTheme(Qt::yellow,QColor(116, 185, 255),Qt::white);
    }else{
        settingAct->setIcon(QIcon(":/img/setting_dark.png"));
        terminal->setTheme(QColor(214, 149, 62),QColor(116, 185, 255),Qt::black);
    }

    QString styleSheet;
    QString qssName;
    qssName="./theme/"+Theme+".qss";

    qDebug()<<"qss:"<<qssName;
    QFile qssfile(qssName);

    while(qssfile.open(QIODeviceBase::ReadOnly)){

        styleSheet=qssfile.readAll();

        if(styleSheet.isEmpty()){
            break;
        }
        setStyleSheet(styleSheet);
        return ;
    }


    QMessageBox::warning(this,"error","加载主题"+Theme+"失败！");
    settings->setValue("UI/Theme","dark");

}

void MainWindow::ShowFileDock()
{
    fileExplorer->SetCurPath(curFolderPath);
    fileDock->show();

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
    commentAct->setEnabled(b);
}

void MainWindow::loadFromFile(QString path)
{
    qDebug()<<"is loading:"<<path ;
    EditArea* editor=new EditArea(codeTabWidget);
    editor->curEditFile=path;///记录打开文件路径
    curEditArea=editor;///记录当前Tab所对应的EditArea,注意curEditArea空指针访问问题

    QFile file(path);


    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        // QTextStream fileStream(&file);
        editor->textEdit->clear();///清空Textedit

        QApplication::setOverrideCursor(Qt::WaitCursor);///鼠标转圈圈
        editor->textEdit->setText(file.readAll());
        QApplication::restoreOverrideCursor();///鼠标恢复
        file.close();


        ///添加Tab页面
        codeTabWidget->addCodeTab(editor,path);
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

    // 获取要删除的 EditArea 对象
    EditArea* editAreaToDelete = qobject_cast<EditArea*>(codeTabWidget->widget(index));
    if (!editAreaToDelete) {
        return; // 如果没有有效的 EditArea，直接返回
    }

    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "保存文件", "文件已修改，是否保存？",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            // 保存文件
            saveFile(editAreaToDelete->curEditFile);
            statusBar()->showMessage(tr("文件已保存并关闭"), 2000);
        }else{
            statusBar()->showMessage(tr("文件已关闭"), 2000);
        }

    }else{
        statusBar()->showMessage(tr("文件已关闭"), 2000);
    }


    // 移除 Tab
    codeTabWidget->removeTab(index);

    // 安全地删除 EditArea 对象
    editAreaToDelete->deleteLater();

    // 如果当前 Tab 是最后一个，将 curEditArea 置空（tabchange中处理）

    statusBar()->showMessage(tr("文件已保存并关闭"), 2000);

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
    QMessageBox::about(this, tr("关于 ByteIDE"),
                       tr("<style>"
                          "body { font-family: 'Microsoft YaHei', sans-serif; }"
                          "h3 { color: #2c3e50; margin: 8px 0; }"
                          ".feature {"
                          "  font-size: 13px;"
                          "  line-height: 1.6;"
                          "  color: #34495e;"
                          "  margin: 8px 0;"
                          "}"
                          "a {"
                          "  color: #3498db;"
                          "  text-decoration: none;"
                          "  font-weight: 500;"
                          "}"
                          "a:hover { color: #2980b9; }"
                          ".divider {"
                          "  height: 1px;"
                          "  background: #ecf0f1;"
                          "  margin: 12px 0;"
                          "}"
                          ".footer {"
                          "  font-size: 10px;"
                          "  color: #7f8c8d;"
                          "  margin-top: 20px;"
                          "}"
                          "</style>"

                          "<div style='max-width: 500px;'>"
                          "<h3>ByteIDE</h3>"

                          "<div class='divider'></div>"

                          "<div class='feature'>"
                          "<p>基于 <b>Qt</b> 和 <b>QScintilla</b> 开发的集成开发环境，</p>"
                          "<p>✨ 核心功能：</p>"
                          "<ul style='margin: 5px 0; padding-left: 20px;'>"
                          "<li>基本的文本编辑与代码运行功能</li>"
                          "<li>Python/C++ 语法高亮与自动补全</li>"
                          "<li>内置 Python 解释器与 C/C++ 编译器，无需配置环境即可一键运行</li>"
                          "<li>集成文件浏览器与终端，方便运行指令与文件管理</li>"
                          "<li>提供常用的快捷键以及查找替换功能</li>"
                          "</ul>"
                          "</div>"

                          "<div class='divider'></div>"

                          "<div class='feature'>"
                          "<p>获取与支持：</p>"
                          "<ul style='margin: 5px 0; padding-left: 20px;'>"
                          "<li>📂 项目仓库：<a href='https://github.com/totrytakeoff/IDE-based-on-Qt'>GitHub</a></li>"
                          "<li>🌐 最新版本：<a href='https://www.byteoj.com/posts/257'>ByteOJ 网站</a></li>"
                          "<li>📧 问题反馈：<a href='mailto:2467315534@qq.com'>2467315534@qq.com</a></li>"
                          "</ul>"
                          "</div>"

                          "<div class='divider'></div>"

                          "<div class='footer'>"
                          "<p>版本 1.2.0 | © 2025 Myself</p>"
                          "<p>基于 Qt %1 | 遵循 MIT 开源协议</p>"  // 自动显示 Qt 版本
                          "</div>"
                          "</div>"
                          ).arg(QT_VERSION_STR));
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

void MainWindow::forceStopProcess()
{
    runner->forceExitProcess();
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

        //注释

        connect(commentAct,&QAction::triggered,curEditArea,&EditArea::setCommentline);

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

void MainWindow::ShowUpdataLog()
{
    UpdateLogWidget *showlog=new UpdateLogWidget();
    showlog->setWindowModality(Qt::ApplicationModal); // 应用级模;
    showlog->show();
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
