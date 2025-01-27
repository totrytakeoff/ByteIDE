#include "terminal.h"
#include <QKeyEvent>
#include <QScrollBar>
#include <QTextBlock>
#include <QDir>
#include <QDebug>

/**
 * @brief 构造函数，初始化终端界面和功能
 * @param parent 父窗口部件
 */
Terminal::Terminal(QWidget *parent)
    : QPlainTextEdit(parent)
    , process(new QProcess(this))
    // , workingDirectory(QDir::currentPath())// 设置初始工作目录
    // , prompt(getCurrentWorkingDir() + "> ")  // 设置初始提示符
    , promptPosition(0)
    , historyPosition(-1)
{
    ///初始化不按顺序？
    workingDirectory=QDir::currentPath();
    prompt=getCurrentWorkingDir()+"> ";

    // 设置终端外观
    setFont(QFont("Consolas", 10));  // 使用等宽字体
    
    // 设置终端颜色主题
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(30, 30, 30));     // 深色背景
    p.setColor(QPalette::Text, QColor(200, 200, 200));  // 浅色文本
    setPalette(p);

    // 配置进程
    process->setWorkingDirectory(workingDirectory);
    
    // 连接进程信号到对应的槽函数
    connect(process, &QProcess::readyReadStandardOutput, this, &Terminal::handleProcessOutput);
    connect(process, &QProcess::readyReadStandardError, this, &Terminal::handleProcessError);
    connect(process, &QProcess::finished, this, &Terminal::handleProcessFinished);

    // connect(process,&QProc)

    // 显示初始提示符
    insertPrompt();
    
    // 配置终端显示
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 禁用水平滚动条
    setLineWrapMode(QPlainTextEdit::WidgetWidth);         // 启用自动换行

    // process->start("cmd.exe");

}

/**
 * @brief 析构函数，确保进程正确终止
 */
Terminal::~Terminal()
{
    if (process->state() == QProcess::Running) {
        process->terminate();
        process->waitForFinished(1000);  // 等待进程终止，最多等待1秒
    }
}

/**
 * @brief 处理键盘事件
 * @param e 键盘事件对象
 * 
 * 处理用户输入，包括命令执行和历史命令浏览
 */
void Terminal::keyPressEvent(QKeyEvent *e)
{

    // qDebug()<<"status while keypress "<<process->exitStatus();
    // qDebug()<<"exitCode while keypress "<<process->exitCode();

    // 获取光标位置
    QTextCursor cursor = textCursor();
    // 防止修改历史命令输出
    if (cursor.position() < promptPosition && e->key() != Qt::Key_Control) {
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
    }

    if(isrunning){

        qDebug()<<"isrunning input...";
        switch(e->key()){
        case Qt::Key_Return:
        case Qt::Key_Enter: {

            QTextCursor cursor = textCursor();
            cursor.setPosition(cursorLinePos);///将cursor移动到prompt位置

            cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);///移动cursor到末尾位置并选中这段文本

            // write input
            QString command = cursor.selectedText();
            qDebug()<<"process->write::"<<command;

            // if (!command.isEmpty()) {
            //     commandHistory.append(command);  // 添加到历史记录
            //     historyPosition = commandHistory.size();
            // }
            // appendPlainText("");  // 换行
            // executeCommand(command);
            process->write(command.toUtf8() + "\n");
            process->waitForBytesWritten();

            break;
        }
        default:

            QTextCursor cursor = textCursor();

            // cursorLinePos=cursor.position();

            QPlainTextEdit::keyPressEvent(e);

            // 设置用户输入的颜色
            QTextCharFormat userInputFormat;
            userInputFormat.setForeground(Qt::yellow);
            // promptPosition
            cursor.setPosition(cursorLinePos);///将cursor移动到prompt位置

            cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);///移动cursor到末尾位置并选中这段文本
            QString input=cursor.selectedText();
            // qDebug()<<"cursor select::"<<cursor.selectedText();

            cursor.removeSelectedText();///删除这段文本
            cursor.setCharFormat(userInputFormat);///设置字体格式
            cursor.insertText(input);///插入格式化后的字体
            setTextCursor(cursor);


        }


        return;
    }



    


    // 处理不同的按键事件
    switch (e->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter: {


        // 获取并执行命令
        QString command = getCurrentCommand().mid(prompt.length());
        if (!command.isEmpty()) {
            commandHistory.append(command);  // 添加到历史记录
            historyPosition = commandHistory.size();
        }
        appendPlainText("");  // 换行
        executeCommand(command);
        break;
    }
    case Qt::Key_Backspace: {
        // 防止删除提示符
        if (cursor.position() <= promptPosition) {
            return;
        }
        QPlainTextEdit::keyPressEvent(e);
        break;
    }
    case Qt::Key_Up: {
        // 显示上一条历史命令
        if (historyPosition > 0) {
            historyPosition--;
            // 清除当前命令
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::End);
            cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            cursor.insertText(prompt + commandHistory[historyPosition]);
        }
        break;
    }
    case Qt::Key_Down: {
        // 显示下一条历史命令
        if (historyPosition < commandHistory.size() - 1) {
            historyPosition++;
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::End);
            cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            cursor.insertText(prompt + commandHistory[historyPosition]);
        }
        break;
    }
    default:
        QPlainTextEdit::keyPressEvent(e);



        QString common=getCurrentCommand().mid(prompt.length());
        QTextCursor cursor = textCursor();


        // 设置用户输入的颜色
        QTextCharFormat userInputFormat;
        userInputFormat.setForeground(Qt::yellow);

        cursor.setPosition(promptPosition);///将cursor移动到prompt位置
        cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);///移动cursor到末尾位置并选中这段文本
        cursor.removeSelectedText();///删除这段文本
        cursor.setCharFormat(userInputFormat);///设置字体格式
        cursor.insertText(common);///插入格式化后的字体
        setTextCursor(cursor);

        break;
    }
}

/**
 * @brief 执行命令
 * @param command 要执行的命令
 */
void Terminal::executeCommand(const QString &command)
{
    if (command.isEmpty()) {
        insertPrompt();
        return;
    }


    // if (handleInternalCommand(command)) {
    //     return;
    // }

    // 特殊处理cd命令
    if (command.startsWith("cd ")) {
        handleCdCommand(command.mid(3).trimmed());
        return;
    }

    // 执行其他命令
    #ifdef Q_OS_WIN

        qDebug()<<"execommand::"<<command;
        process->start("cmd.exe",QStringList()<<"/c"<<command);
        // process->write((command + "\n").toUtf8());
        // process->waitForBytesWritten();


    #else
        process->start("bash", QStringList() << "-c" << command);
    #endif
}





bool Terminal::handleInternalCommand(const QString &command)
{

    QStringList args = command.split(" ", Qt::SkipEmptyParts);///以空格为分隔符，将命令分割为参数列表
    if (args.isEmpty()) return false;

    QString cmd = args[0].toLower();
    args.removeFirst();  // 移除命令名，保留参数

    // if (cmd == "ls") {
    //     handleLsCommand(args);
    //     return true;
    // }
    // else if (cmd == "pwd") {
    //     handlePwdCommand();
    //     return true;
    // }
    // else if (cmd == "clear") {
    //     handleClearCommand();
    //     return true;
    // }

    return false;

}



/**
 * @brief 处理cd命令
 * @param dir 目标目录
 */
void Terminal::handleCdCommand(const QString &dir)
{
    QString newPath;
    if (dir.startsWith("/")) {
        newPath = dir;  // 绝对路径
    } else {
        newPath = QDir(workingDirectory).absoluteFilePath(dir);  // 相对路径
    }

    QDir newDir(newPath);
    if (newDir.exists()) {
        // 更新工作目录
        workingDirectory = newDir.absolutePath();
        process->setWorkingDirectory(workingDirectory);
        prompt = getCurrentWorkingDir() + "> ";
        qDebug()<<" cd->prompt:"<<prompt;

        insertPrompt();
    } else {
        appendPlainText("Directory does not exist: " + newPath);
        insertPrompt();
    }
}

/**
 * @brief 获取当前工作目录名
 * @return 当前目录名
 */
QString Terminal::getCurrentWorkingDir() const
{
    // qDebug()<<"getcurrentWorkingDir:"<<workingDirectory;
    return QDir(workingDirectory).dirName();
}

/**
 * @brief 处理进程标准输出
 */
void Terminal::handleProcessOutput()
{
    QByteArray output = process->readAllStandardOutput();

    QTextCharFormat outputFormat;
    outputFormat.setForeground(QColor(116, 185, 255));


    // insertPlainText(QString::fromLocal8Bit(output));
    

    qDebug()<<"output::"<<QString::fromLocal8Bit(output);
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n"+QString::fromLocal8Bit(output), outputFormat);
    setTextCursor(cursor);
    cursorLinePos=cursor.position();


    // 滚动到底部
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

/**
 * @brief 处理进程错误输出
 */

void Terminal::handleProcessError()
{
    QByteArray error = process->readAllStandardError();
    QString errorString = QString::fromLocal8Bit(error);

    // 设置错误信息的颜色（例如红色）
    QTextCharFormat errorFormat;
    errorFormat.setForeground(Qt::red);  // 设置字体颜色为红色

    // 插入带有格式的错误信息
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n"+errorString, errorFormat);
    setTextCursor(cursor);

    // 滚动到底部
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

/**
 * @brief 处理进程结束
 */
void Terminal::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // qDebug()<<"process:"<<process->processId()<<"have finish";
    // qDebug()<<"exitcode:"<<exitCode;
    // qDebug()<<"exitstatus:"<<exitStatus;
    isrunning=false;
    insertPrompt();  // 显示新的提示符
}

/**
 * @brief 插入命令提示符
 */
void Terminal::insertPrompt()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextCharFormat FontFormat;
    FontFormat.setForeground(Qt::white);

    cursor.insertText("\n");

    // qDebug()<<"currentPrompt:"<<prompt;
    cursor.insertText(prompt,FontFormat);
    promptPosition = cursor.position();
    setTextCursor(cursor);
}

/**
 * @brief 获取当前命令文本
 * @return 当前输入的命令
 */
QString Terminal::getCurrentCommand() const
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);

    // qDebug()<<"curcmd:"<<line;
    // qDebug()<<"prompt:"<<prompt;
    // qDebug()<<"promptlen::"<<prompt.length();

///获取当前命令（带prompt）
/// .mid(prompt.length())去prompt
    return cursor.selectedText();

}

/**
 * @brief 设置进程环境变量
 * @param environment 环境变量设置,调用解释器/编译器运行文件时要用
 */
void Terminal::setProcessEnvironment(const QProcessEnvironment &environment)
{
    process->setProcessEnvironment(environment);
}

void Terminal::setIsRunning(bool flag)
{
    isrunning=flag;
}

QProcess *Terminal::getProcess()
{
    return this->process;
}
