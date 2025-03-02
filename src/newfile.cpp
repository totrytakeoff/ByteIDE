#include "newfile.h"
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

NewFile::NewFile(QWidget *parent,QString path)
    : QDialog{parent},filePath{path}
{

    this->setWindowTitle("新建文件");

    filePath+="/";
    lineEdit = new QLineEdit(this);
    pathLab=new QLabel("文件路径:"+filePath,this);
    confirmBtn = new QPushButton("确认", this);
    cancelBtn = new QPushButton("取消", this);
    selectDirBtn = new QPushButton("选择目录", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(confirmBtn);
    buttonLayout->addWidget(cancelBtn);

    mainLayout->addWidget(pathLab);
    mainLayout->addWidget(lineEdit);
    mainLayout->addWidget(selectDirBtn);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(confirmBtn, &QPushButton::clicked, this, &NewFile::onConfirm);
    connect(cancelBtn, &QPushButton::clicked, this, &NewFile::onCancel);
    connect(selectDirBtn, &QPushButton::clicked, this, &NewFile::onSelectDir);
    connect(lineEdit,&QLineEdit::cursorPositionChanged,this,&NewFile::onInputFileName);


}

NewFile::~NewFile()
{

}

void NewFile::CreatNewFiles(QString &path)
{

}

QString NewFile::GetNewFile()
{
    return FileString;
}

void NewFile::onConfirm()
{
    // 实现确认按钮的功能
    ///从lineEdit中获取纯文件名
    QString fileName = lineEdit->text().trimmed();
    qDebug()<<"--new file name:"<<fileName;
    // 检查文件名是否为空
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Error", "文件名不能为空!");
        return;
    }

    // 创建文件
    QFile file(filePath+fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.close();
        QMessageBox::information(this, "Success", "文件创建成功!");
        FileString=filePath+fileName;
        accept(); // 关闭窗口

    } else {
        QMessageBox::critical(this, "Error", "文件创建失败: " + file.errorString());
    }

}

void NewFile::onCancel()
{
    reject();
}

void NewFile::onSelectDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择目录", QString());
    if (!dir.isEmpty()) {
        filePath=dir+"/";
    }
    onInputFileName();
}

void NewFile::onInputFileName()
{

    pathLab->setText(filePath+lineEdit->text());

}
