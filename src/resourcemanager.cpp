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
    :QWidget(parent)
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

    initAct();

    connect(treeView,&QTreeView::clicked,this,&ResourceManager::on_FileClick);
    connect(treeView,&QTreeView::doubleClicked,this,&ResourceManager::on_FileDoubleClick);
    connect(treeView,&QTreeView::customContextMenuRequested,this,&ResourceManager::showContextMenu);
    initSheet();

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
    ///QInputDialog不能独立设置样式，故构造一个父Dialog单独设置样式，再让其他QInputDialog继承其样式
    styleDialog=new QInputDialog();

    // styleDialog->setStyleSheet(
    //     R"(QInputDialog { background-color: rgb(31, 31, 31); }
    //         QLineEdit {
    //             color: white;
    //             background-color: rgb(56, 56, 56);
    //             border:1px solid rgb(153,153,153);
    //         }
    //         QPushButton {
    //             color: white;
    //             background-color: rgb(61, 61, 61);
    //         }
    //         QLabel{
    //             color: white;
    //         }
    //     )");





    // treeView->setStyleSheet(treeViewStyleSheet);
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

void ResourceManager::goToFilePos(QString &path)
{
    QModelIndex index=fileModel->index(path);
    // 选择该项
    QItemSelectionModel *selectionModel = treeView->selectionModel();
    selectionModel->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

    // 滚动到该项
    treeView->scrollTo(index);

}

void ResourceManager::on_FileClick(const QModelIndex &index)
{
    QString filename=fileModel->filePath(index);
    emit fileClick(filename);

}

void ResourceManager::on_FileDoubleClick(const QModelIndex &index)
{

    QString filename=fileModel->filePath(index);

    if(index.isValid()&&fileModel->isDir(index))
    {
        qDebug()<<filename<<"dir has been doubleClick";
        return;
    }


    emit fileDoubleClick(filename);
    qDebug()<<"filePath:"<<fileModel->filePath(index);

}

void ResourceManager::newFile()
{
    bool ok;
    QString newfileName = QInputDialog::getText(styleDialog, "新建文件",
                                            "文件名:", QLineEdit::Normal,
                                            "", &ok);
    if (ok && !newfileName.isEmpty()) {
        QString newfilePath = curFilePath + "/" + newfileName;
         qDebug()<<"newFilePath->"<<newfilePath;
        QFile file(newfilePath);
        if (file.exists()) {
            QMessageBox::warning(this,"创建失败!","该文件已经存在!",QMessageBox::Ok);
            return;
        }
         file.open(QIODevice::WriteOnly);

    }

    goToFilePos(newfileName);

}

void ResourceManager::newFolder()
{


    bool ok;
    QString newfileName = QInputDialog::getText(styleDialog, "新建文件夹",
                                                "文件夹名:", QLineEdit::Normal,
                                                "", &ok);
    if (ok && !newfileName.isEmpty()) {
        QString newfilePath = curFilePath + "/" + newfileName;
        qDebug()<<"newFilePath->"<<newfilePath;
        if (QDir(newfilePath).exists()) {
             QMessageBox::warning(this,"创建失败!","该文件夹已经存在!",QMessageBox::Ok);
            return;
        }
        QDir dir;
        dir.mkpath(newfilePath);

    }

}

void ResourceManager::openFile()
{

    if(curIdx.isValid()&&fileModel->isDir(curIdx))
    {
        treeView->expand(curIdx);
        return;
    }
    emit fileDoubleClick(curFilePath);
}

void ResourceManager::deleteFile()
{


        // 显示一个包含三个选项的对话框：取消、永久删除和移至回收站
        QMessageBox messageBox(this);
        messageBox.setWindowTitle("确认删除");
        messageBox.setText(QString("你确定要删除 %1 吗?").arg(curFilePath));
        QPushButton *permanentDeleteButton = messageBox.addButton(tr("永久删除"), QMessageBox::YesRole);
        QPushButton *moveToRecycleBinButton = messageBox.addButton(tr("移至回收站"), QMessageBox::NoRole);
        QPushButton *cancelBtn=messageBox.addButton(tr("取消"),QMessageBox::RejectRole);
        messageBox.exec();

        if (messageBox.clickedButton() == permanentDeleteButton) {
            // 用户选择了永久删除
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
        } else if (messageBox.clickedButton() == moveToRecycleBinButton) {
            // 用户选择了移至回收站
            bool result = QFile::moveToTrash(curFilePath);
            if (result) {
                qDebug() << "Successfully moved to trash:" << curFilePath;
            } else {
                qDebug() << "Failed to move to trash:" << curFilePath;
            }
        }else if(messageBox.clickedButton() == cancelBtn){
            return;
        }


}

void ResourceManager::renameFile()
{
    ///直接发信号使用Tab的rename，避免Tab名未跟随重命名
    qDebug()<<"renameFile:<"<<curFilePath;
    emit renameRequest(curFilePath);

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
        menu.addAction(newFileAct);
        menu.addAction(newFolderAct);
        menu.addAction(deleteAct);
        menu.addAction(renameAct);
        menu.addAction(openInExplorerAct);
        menu.addAction(copyFilePathAct);
    } else {
        menu.addAction(openAct);

        menu.addAction(deleteAct);
        menu.addAction(renameAct);
        menu.addAction(openInExplorerAct);
        menu.addAction(copyFilePathAct);
    }

    // 显示菜单
    menu.exec(globalPos);
}




