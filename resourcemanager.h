#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

// #include <QTreeWidget>
// #include <QFileSystemModel>
class QTreeView;
class QFileSystemModel;

class ResourceManager
{
public:
    ResourceManager();




private:
    QTreeView* treeview;
    QFileSystemModel* filemodel;


};

#endif // RESOURCEMANAGER_H
