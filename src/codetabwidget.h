#ifndef CODETABWIDGET_H
#define CODETABWIDGET_H

#include <QTabWidget>

#include "resourcemanager.h"

class CodeTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    CodeTabWidget(QWidget *parent);
    ~CodeTabWidget();

    void bindFileExplorer(ResourceManager *FileExplorer);



signals:
    void haveRenameFile(QString &renameFilePath);
    void haveOpenFolder(QString &folderPath);
public slots:
    void showContextMenu(const QPoint &pos);
    void closeTab();
    void closeAllTab();
    void renameTab();
    void showTabInFE();
    void copyPath();

    void addCodeTab(QWidget *widget, const QString &filePath);

private:
    void creatAct();

private:
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *renameAct;
    QAction *showInFileExploreAct;
    QAction *copyFilePathAct;

    QMap<QWidget*,QString> TabMap;//用于保存每个Tab所对应的文件
    int curIdx=0;///当前Tab的index
    ResourceManager *FileExplorer=nullptr;///对应文件资源管理器
    QString renameFilePath="";///重命名文件的路基
};

#endif // CODETABWIDGET_H
