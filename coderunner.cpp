#include "coderunner.h"
#include <QString>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProcess>

#include "terminal.h"
#include <cstdlib>


CodeRunner::CodeRunner(QWidget* parent)
    :Terminal(parent)
{

    searchRunner();

}

CodeRunner::~CodeRunner()
{

}

void CodeRunner::setMode(QString &fileType)
{
    qDebug()<<"setmode:"<<fileType;
    if(fileType=="py")
        RunMode=Mode::Python;
    else if(fileType=="cpp")
        RunMode=Mode::Cpp;
    else RunMode=Mode::UnKown;

}

void CodeRunner::runCode()
{
    isrunning=true;
    if(RunMode==Mode::Python){
        runPythonCode();
        return;
    }else if(RunMode==Mode::Cpp){
        runCppCode();
        return;
    }else {
        QMessageBox::warning(this,"运行错误","该文件不能被直接运行");
        return;
    }
}

void CodeRunner::runCppCode()
{
    // 确保输出目录存在
    QDir dir("./out");
    if (!dir.exists()) {
        bool success = dir.mkpath(".");
        if (!success) {
            qDebug() << "Failed to create output directory.";
            return;
        }
    }

    QString outputPath="./out/"+QFileInfo(runFile).completeBaseName();
    QStringList arguments;

    ///g++ 参数
    arguments<<runFile<<"-o"<<outputPath;

    process->start(CppRunner,arguments);
    process->waitForFinished();

    if(process->exitCode()==0&&process->exitStatus()==QProcess::NormalExit){

        #ifdef Q_OS_WIN
        outputPath=".\\out\\"+QFileInfo(runFile).completeBaseName()+".exe";
            ///     TEXT()<----_T();
            TCHAR cmdLine[128];
            #ifdef _UNICODE
            qDebug()<<"outputpath::"<<outputPath;
                swprintf(cmdLine,sizeof(cmdLine),TEXT("cmd /k %s && echo.&& pause && exit"),outputPath.toLocal8Bit().constData());
            #else
                snprintf(cmdLine,sizeof(cmdLine),TEXT("cmd /k %s && echo.&& pause && exit"),runFile.toLocal8Bit().constData());
            #endif

            qDebug()<<"cmdline::"<<QString(cmdLine);
            WinStartProcess(NULL,cmdLine);
        #else
        ///留给linux的
        #endif
    }else{
        qDebug()<<"编译错误！";
    }


}

void CodeRunner::runPythonCode()
{
    process->start(PythonRunner,QStringList()<<runFile);
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

void CodeRunner::setRunFile(QString &file)
{
    runFile=file;
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

        // qDebug()<<"Seaching File:"<<entry.fileName();
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

BOOL CodeRunner::WinStartProcess(TCHAR* lpApplicationName, TCHAR* lpCommandLine)
{
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        // 初始化 STARTUPINFO 结构体
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_SHOW; // 显示窗口

        // 初始化 PROCESS_INFORMATION 结构体
        ZeroMemory(&pi, sizeof(pi));

        // 创建进程
        BOOL bSuccess = CreateProcess(
            lpApplicationName,   // 应用程序名称（可以为 NULL）
            lpCommandLine,       // 命令行参数
            NULL,                // 进程安全属性
            NULL,                // 线程安全属性
            FALSE,               // 是否继承句柄
            CREATE_NEW_CONSOLE,  // 创建标志：创建新的控制台窗口
            NULL,                // 使用父进程的环境
            NULL,                // 使用父进程的当前目录
            &si,                 // STARTUPINFO 指针
            &pi                  // PROCESS_INFORMATION 指针
            );

        if (!bSuccess) {
            printf("CreateProcess failed (%d).\n", GetLastError());
            return FALSE;
        }
        else {
            // // 等待进程结束（可选）
            //这玩意会阻塞IDE主进程
            // WaitForSingleObject(pi.hProcess, INFINITE);

            // 关闭进程和线程句柄
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        return TRUE;
}


void CodeRunner::keyPressEvent(QKeyEvent *e)
{

    QTextCursor cursor = textCursor();
    // 防止修改历史命令输出
    if (cursor.position() < promptPosition && e->key() != Qt::Key_Control) {
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
    }

    qDebug()<<"clicked:::"<<e->key();
    ///处理运行时的交互输入
    keyPressEventWhileRunning(e);

}



