#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

// #include <QTreeWidget>
// #include <QFileSystemModel>
class QTreeView;
class QFileSystemModel;
class QDockWidget;

class ResourceManager
{
public:
    ResourceManager(QDockWidget* Dock);




private:
    QTreeView* treeView;
    QFileSystemModel* fileModel;


};

#endif // RESOURCEMANAGER_H
