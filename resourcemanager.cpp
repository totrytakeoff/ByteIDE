#include "resourcemanager.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QDockWidget>


ResourceManager::ResourceManager(QDockWidget* Dock) {


    treeView=new QTreeView();
    fileModel=new QFileSystemModel();

    fileModel->setRootPath(QDir::currentPath());///init the model
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    ///设置根路径
    treeView->setRootIndex(fileModel->index(QDir::currentPath()));

    treeView->setModel(fileModel);

    // Hide size, type, date columns - show only filename
    treeView->setHeaderHidden(true);
    for (int i = 1; i < fileModel->columnCount(); ++i) {
        treeView->hideColumn(i);
    }


}

