#include "resourcemanager.h"
#include <QTreeView>
#include <QFileSystemModel>



ResourceManager::ResourceManager() {

    treeview=new QTreeView();
    filemodel=new QFileSystemModel();





    treeview->setModel(filemodel);



}
