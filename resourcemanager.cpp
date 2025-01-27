#include "resourcemanager.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QDockWidget>
#include <QPushButton>

ResourceManager::ResourceManager(QDockWidget* Dock) {


    treeView=new QTreeView();
    fileModel=new QFileSystemModel();

    fileModel->setRootPath(QDir::currentPath());///init the model
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    ///设置根路径
    // treeView->setRootIndex(fileModel->index(QDir::currentPath()));


    treeView->setModel(fileModel);

    // Hide size, type, date columns - show only filename
    treeView->setHeaderHidden(true);
    for (int i = 1; i < fileModel->columnCount(); ++i) {
        treeView->hideColumn(i);
    }

    Dock->setWidget(treeView);

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


    connect(treeView,&QTreeView::clicked,this,&ResourceManager::on_FileClick);
    connect(treeView,&QTreeView::doubleClicked,this,&ResourceManager::on_FileDoubleClick);


}

ResourceManager::~ResourceManager()
{

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

void ResourceManager::on_FileClick(const QModelIndex &index)
{
    QString filename=fileModel->filePath(index);
    emit(fileClick(filename));
    qDebug()<<"emit Click,filename:"<<filename;
    qDebug()<<"filePath:"<<fileModel->filePath(index);

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

