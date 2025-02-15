#include "codetabwidget.h"
#include <QTabBar>
#include <QDebug>
#include <QMenu>
#include <QLineEdit>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QClipboard>

CodeTabWidget::CodeTabWidget(QWidget* parent)
    :QTabWidget(parent)
{

    creatAct();
    setContextMenuPolicy(Qt::CustomContextMenu);
    this->clear();///清空tabwidget内Tab
    this->setTabsClosable(true);///启用Tab关闭btn
    connect(this,&QTabWidget::customContextMenuRequested,this,&CodeTabWidget::showContextMenu);
}

CodeTabWidget::~CodeTabWidget(){}

void CodeTabWidget::bindFileExplorer(ResourceManager *FileExplorer)
{
    this->FileExplorer=FileExplorer;
    connect(FileExplorer,&ResourceManager::renameRequest,this,[this](QString path){
        this->renameFilePath=path;
        qDebug()<<"receive rename";
        renameTab();
    });
}

void CodeTabWidget::showContextMenu(const QPoint &pos)
{

    int index = this->tabBar()->tabAt(pos);
    // this->tabBar()->moveTab() //drag tab

    qDebug()<<"当前选项卡为:"<<QString::number(index);
    this->setCurrentIndex(index);
    curIdx=index;

    QMenu *tabMenu=new QMenu(this);
    tabMenu->addAction(closeAct);
    tabMenu->addAction(closeAllAct);
    tabMenu->addAction(renameAct);
    tabMenu->addAction(showInFileExploreAct);
    tabMenu->addAction(copyFilePathAct);

    if(index != -1)
    {
        tabMenu->exec(QCursor::pos());
    }


}

void CodeTabWidget::closeTab()
{

    emit this->tabCloseRequested(curIdx);
    //将对应Widget从map容器中移除
    TabMap.remove(this->widget(curIdx));
}

void CodeTabWidget::closeAllTab()
{
    //遍历删除Tab
    for(int i=this->count()-1;i>=0;i--){
        emit this->tabCloseRequested(i);
    }

    TabMap.clear();

}

void CodeTabWidget::renameTab()
{
    bool renameByFE = !renameFilePath.isEmpty();
    bool inTabMap=false;

    if(renameByFE){

        for(auto key:TabMap.keys()){
            if(TabMap[key]==renameFilePath){
                inTabMap=true;
                ///通过key获取index
                curIdx=this->indexOf(key);
                break;
            }
        }

    }

    QString OriginFilePath;

    if(renameByFE){
        OriginFilePath=renameFilePath;

    }else{
        OriginFilePath=TabMap[this->widget(curIdx)];
    }




    QString curFilePath=QFileInfo(OriginFilePath).absoluteFilePath();
    ///文件重命名
    bool ok;
    QString newName = QInputDialog::getText(nullptr, "重命名",
                                            "新文件名:", QLineEdit::Normal,
                                            QFileInfo(OriginFilePath).fileName(), &ok);

    if(newName.isEmpty()){
        return ;
    }

    ///修改文件名
    if (ok && !newName.isEmpty()) {
        QString newPath = QFileInfo(OriginFilePath).absolutePath() + "/" + newName;
        if (QFile::rename(curFilePath, newPath)) {
            qDebug() << "Successfully renamed" << curFilePath << "to" << newPath;
        } else {
            qDebug() << "Failed to rename" << curFilePath << "to" << newPath;
        }
    }

    if(!renameByFE||(renameByFE&&inTabMap)){
        this->setTabText(curIdx,newName);
    }


    emit haveRenameFile(newName);
    renameFilePath="";


}

void CodeTabWidget::showTabInFE()
{
    qDebug()<<"isshowTabInFE";
    if(FileExplorer){
        qDebug()<<"showTabInFE";
        QString folderPath=QFileInfo(TabMap[this->widget(curIdx)]).absolutePath();
        QString FilePath=QFileInfo(TabMap[this->widget(curIdx)]).absoluteFilePath();
        FileExplorer->SetCurPath(folderPath);
        emit haveOpenFolder(folderPath);
        FileExplorer->goToFilePos(FilePath);
    }

}

void CodeTabWidget::copyPath()
{
    // 将文件路径复制到剪贴板
    QApplication::clipboard()->setText(TabMap[this->widget(curIdx)]);
}

void CodeTabWidget::addCodeTab(QWidget *widget, const QString &filePath)
{
    this->addTab(widget,QFileInfo(filePath).fileName());

    ///将该Tab界面加入TabMap中
    TabMap[widget]=filePath;
}

void CodeTabWidget::creatAct()
{
    closeAct=new QAction("关闭该标签页",this);
    closeAllAct=new QAction("关闭全部标签页",this);
    renameAct=new QAction("重命名",this);
    showInFileExploreAct=new QAction("在文件资源管理器显示",this);
    copyFilePathAct=new QAction("复制该文件路径",this);

    connect(closeAct,&QAction::triggered,this,&CodeTabWidget::closeTab);
    connect(closeAllAct,&QAction::triggered,this,&CodeTabWidget::closeAllTab);
    connect(renameAct,&QAction::triggered,this,&CodeTabWidget::renameTab);
    connect(showInFileExploreAct,&QAction::triggered,this,&CodeTabWidget::showTabInFE);
    connect(copyFilePathAct,&QAction::triggered,this,&CodeTabWidget::copyPath);


}

