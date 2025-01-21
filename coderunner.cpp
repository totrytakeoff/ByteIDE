#include "coderunner.h"
#include <QString>
#include <QDir>
#include <QFile>
#include <QMessageBox>

#include "terminal.h"



CodeRunner::CodeRunner(QWidget* parent)
    :QWidget(parent)
{


}

CodeRunner::~CodeRunner()
{

}

void CodeRunner::setMode(QString &fileType)
{
    if(fileType==".py")
        RunMode=Mode::Python;
    if(fileType==".cpp")
        RunMode=Mode::Cpp;

}



void CodeRunner::searchRunner()
{

    QString Path;

    QString searchPattern="python.exe";

    // searchPattern="CMakeCache.txt";

    Path=QDir::currentPath()+"/Runner/python312";


    PythonRunner=searchFiles(Path,searchPattern);
    qDebug()<<"py:::"<<PythonRunner;


    Path=QDir::currentPath()+"/Runner/MinGW64/bin";
    searchPattern="g++.exe";
    CppRunner=searchFiles(Path,searchPattern);
    qDebug()<<"cpp:"<<CppRunner;


    if(PythonRunner.isEmpty()){
        QMessageBox::warning(this,"未找到解释器","未找到Python解释器,请检查PYTHON目录是否完整或自定义路径是否正确");
    }



    if(CppRunner.isEmpty()){
        QMessageBox::warning(this,"未找到编译器","未找到Cpp编译器,请检查CPP目录是否完整或自定义路径是否正确");

    }

}


QString CodeRunner::searchFiles(const QString &dirPath, const QString &searchPattern) {
    QDir dir(dirPath);

    if (!dir.exists()) {
        qDebug() << "Directory does not exist:" << dirPath;
        return "";
    }

    // 获取所有匹配的文件和子目录（不包括 "." 和 ".."）
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QFileInfo &entry, entries) {

        qDebug()<<"Seaching File:"<<entry.fileName();
        if (entry.isDir()) {
            // 递归搜索子目录
            searchFiles(entry.absoluteFilePath(), searchPattern);
        } else if (entry.isFile() && entry.fileName().contains(searchPattern, Qt::CaseInsensitive)) {
            qDebug() << "Found file:" << entry.absoluteFilePath();
            return entry.absoluteFilePath();
        }

    }
    return "";
}


