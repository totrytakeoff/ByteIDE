#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

// #include <QTreeWidget>
// #include <QFileSystemModel>
//hello
#include <QObject>
#include <QString>
class QTreeView;
class QFileSystemModel;
class QDockWidget;

class QPushButton;


class ResourceManager :public QObject
{
    Q_OBJECT
public:
    ResourceManager(QDockWidget* Dock);
    ~ResourceManager();

    void SetCurPath(QString path);

    void RefreshModel();

    void ModifyStyle();

public slots:
    void on_FileClick(const QModelIndex &index);
    void on_FileDoubleClick(const QModelIndex &index);

signals:
    void fileClick(QString FileName);
    void fileDoubleClick(QString FileName);
    ///FileName-->is full path+FileName
private:
    QTreeView* treeView;
    QFileSystemModel* fileModel;
    QPushButton* refreshModelBtn;

    QString treeViewStyleSheet;

};

#endif // RESOURCEMANAGER_H
