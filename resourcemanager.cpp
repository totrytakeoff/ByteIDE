#include "resourcemanager.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QDockWidget>
#include <QPushButton>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QWidget>
#include <QProcess>

ResourceManager::ResourceManager(QDockWidget* Dock,QWidget *parent)
    :QObject(parent)
{


    treeView=new QTreeView();
    fileModel=new QFileSystemModel();

    fileModel->setRootPath(QDir::currentPath());///init the model
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    ///设置根路径
    // treeView->setRootIndex(fileModel->index(QDir::currentPath()));


    treeView->setModel(fileModel);

    treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // Hide size, type, date columns - show only filename
    treeView->setHeaderHidden(true);
    for (int i = 1; i < fileModel->columnCount(); ++i) {
        treeView->hideColumn(i);
    }

    Dock->setWidget(treeView);

    initSheet();
    initAct();

    connect(treeView,&QTreeView::clicked,this,&ResourceManager::on_FileClick);
    connect(treeView,&QTreeView::doubleClicked,this,&ResourceManager::on_FileDoubleClick);
    connect(treeView,&QTreeView::customContextMenuRequested,this,&ResourceManager::showContextMenu);

}

ResourceManager::~ResourceManager()
{
    if(treeView)
        delete treeView;
    if(fileModel)
        delete fileModel;
}

void ResourceManager::SetCurPath(QString path)
{
    fileModel->setRootPath(path);
    treeView->setRootIndex(fileModel->index(path));

    qDebug()<<"the fileModel has been set to :"<<path;

}

void ResourceManager::RefreshModel()
{

}

void ResourceManager::ModifyStyle()
{

}

void ResourceManager::initSheet()
{
    treeViewStyleSheet=R"(
        QTreeView{background-color: rgb(31, 31, 31);
            color: rgb(214, 214, 214);
        }

        /* 基本项样式 */
        QTreeView::item {
            padding: 1px;
        }

        /* 悬停时的样式 */
        QTreeView::item:hover {
            background-color:rgb(80,80,80);
            color: white;
        }

        /* 选中时的样式 */
        QTreeView::item:selected {
            background-color: rgb(34, 91, 149);
            color: white;
        }

        /* 非活动窗口中选中的项 */
        QTreeView::item:selected:!active {
            background-color: #c8e6c9;
        }

        /* 展开箭头样式 */
        QTreeView::branch:has-children:!adjoins-item {
            color: white;
        }

        QTreeView::branch:has-children:adjoins-item {
            color: white;
        }

        QTreeView::branch:!has-children {
            color: white;
        }

        /* 展开/折叠状态 */
        QTreeView::branch:open:has-children {
            color: white;
        }

        QTreeView::branch:closed:has-children {
            color: white;
        }


    )";
    //border-image: url(:/icons/branch-closed.png) 0;

    // Dock->setStyleSheet(R"(

    //         color:rgb(214,214,214);
    //         boder: 10px solid white;

    //     )");

    treeView->setStyleSheet(treeViewStyleSheet);
}

void ResourceManager::initAct()
{
    openAct = new QAction("打开", this);
    deleteAct = new QAction("删除", this);
    renameAct = new QAction("重命名", this);
    openInExplorerAct = new QAction("在本地文件资源管理器中显示", this);
    copyFilePathAct = new QAction("复制文件路径", this);
    newFileAct=new QAction("新建文件",this);
    newFolderAct=new QAction("新建文件夹",this);

    connect(openAct, &QAction::triggered, this, &ResourceManager::openFile);
    connect(deleteAct, &QAction::triggered, this, &ResourceManager::deleteFile);
    connect(renameAct, &QAction::triggered, this, &ResourceManager::renameFile);
    connect(openInExplorerAct, &QAction::triggered, this, &ResourceManager::openInExplorer);
    connect(copyFilePathAct, &QAction::triggered, this, &ResourceManager::copyFilePath);
    connect(newFileAct, &QAction::triggered, this, &ResourceManager::newFile);
    connect(newFolderAct, &QAction::triggered, this, &ResourceManager::newFolder);

}

void ResourceManager::on_FileClick(const QModelIndex &index)
{
    QString filename=fileModel->filePath(index);
    emit(fileClick(filename));

}

void ResourceManager::on_FileDoubleClick(const QModelIndex &index)
{

    QString filename=fileModel->filePath(index);

    if(index.isValid()&&fileModel->isDir(index))
    {
        qDebug()<<filename<<"dir has been doubleClick";
        return;
    }


    emit(fileDoubleClick(filename));
    qDebug()<<"emit doubleClick,filename:"<<filename;
    qDebug()<<"filePath:"<<fileModel->filePath(index);

}

void ResourceManager::newFile()
{

}

void ResourceManager::newFolder()
{

}

void ResourceManager::openFile()
{

    if(curIdx.isValid()&&fileModel->isDir(curIdx))
    {
        treeView->expand(curIdx);
    }else{
        emit(fileDoubleClick(curFilePath));
    }

}

void ResourceManager::deleteFile()
{
        // 确认用户是否真的想要删除
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "确认删除",
                                      QString("你确定要删除 %1 吗?").arg(curFilePath),
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            bool result;
            if (QFileInfo(curFilePath).isDir()) {
                QDir dir(curFilePath);
                result = dir.removeRecursively(); // 删除整个目录及其内容
            } else {
                result = QFile::remove(curFilePath); // 删除文件
            }
            if (result) {
                qDebug() << "Successfully deleted:" << curFilePath;
            } else {
                qDebug() << "Failed to delete:" << curFilePath;
            }
        }

}

void ResourceManager::renameFile()
{
        bool ok;
        QString newName = QInputDialog::getText(nullptr, "重命名",
                                                "New name:", QLineEdit::Normal,
                                                fileModel->fileName(curIdx), &ok);
        if (ok && !newName.isEmpty()) {
            QString newPath = fileModel->fileInfo(curIdx).absolutePath() + "/" + newName;
            if (QFile::rename(curFilePath, newPath)) {
                qDebug() << "Successfully renamed" << curFilePath << "to" << newPath;
            } else {
                qDebug() << "Failed to rename" << curFilePath << "to" << newPath;
            }
        }
}

void ResourceManager::openInExplorer()
{
    // 使用 QDesktopServices::openUrl 打开文件或目录,这玩意使用默认打开方式打开，不能实现本地文件资源管理器打开
    // QDesktopServices::openUrl(QUrl::fromLocalFile(curFilePath));
    QProcess::startDetached("explorer.exe", {"/select,", QDir::toNativeSeparators(curFilePath)});
}

void ResourceManager::copyFilePath()
{
    // 将文件路径复制到剪贴板
    QApplication::clipboard()->setText(curFilePath);
}



void ResourceManager::showContextMenu(const QPoint &pos)
{
    qDebug()<<"right click file explorer";
    // 获取点击位置对应的模型索引
    QModelIndex index = treeView->indexAt(pos);
    if (!index.isValid()) {
        return; // 如果没有有效的索引，直接返回
    }
    curIdx=index;

    // 转换为全局位置
    QPoint globalPos = treeView->viewport()->mapToGlobal(pos);

    // 创建一个菜单
    QMenu menu(treeView);

    // 获取文件路径
    QString filePath = fileModel->filePath(index);
    curFilePath=filePath;

    qDebug()<<filePath;


    // 根据是否是目录来添加不同的菜单项
    if (fileModel->isDir(index)) {
        menu.addAction(openAct);
        menu.addAction(newFolderAct);
        menu.addAction(deleteAct);
        menu.addAction(renameAct);
        menu.addAction(openInExplorerAct);
        menu.addAction(copyFilePathAct);
    } else {
        menu.addAction(openAct);
        menu.addAction(newFileAct);
        menu.addAction(deleteAct);
        menu.addAction(renameAct);
        menu.addAction(openInExplorerAct);
        menu.addAction(copyFilePathAct);
    }

    // 显示菜单
    menu.exec(globalPos);
}




