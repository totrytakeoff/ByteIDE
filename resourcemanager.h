#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QWidget>
#include <QString>
#include <QModelIndex>
class QTreeView;
class QFileSystemModel;
class QDockWidget;

class QPushButton;
class QAction;
class QInputDialog;

class ResourceManager :public QWidget
{
    Q_OBJECT
public:
    ResourceManager(QDockWidget* Dock,QWidget *parent = nullptr);
    ~ResourceManager();

    void SetCurPath(QString path);

    void RefreshModel();

    void ModifyStyle();

    void initSheet();

    void initAct();
public slots:
    void on_FileClick(const QModelIndex &index);
    void on_FileDoubleClick(const QModelIndex &index);

    void newFile();
    void newFolder();
    void openFile();
    void deleteFile();
    void renameFile();
    void openInExplorer();
    void copyFilePath();

protected slots:
    void showContextMenu(const QPoint &pos);

signals:
    void fileClick(QString FileName);
    void fileDoubleClick(QString FileName);
    ///FileName-->is full path+FileName
private:
    QTreeView* treeView;
    QFileSystemModel* fileModel;
    QPushButton* refreshModelBtn;

    QString treeViewStyleSheet;
    QString curFilePath;
    QModelIndex curIdx;


    QAction *newFileAct;
    QAction *newFolderAct;
    QAction *openAct;
    QAction *deleteAct;
    QAction *renameAct;
    QAction *openInExplorerAct;
    QAction *copyFilePathAct;

    QInputDialog *styleDialog;

};

#endif // RESOURCEMANAGER_H
