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
    , prompt(getCurrentWorkingDir() + "> ")  // 设置初始提示符
    , promptPosition(0)
    , workingDirectory(QDir::currentPath())  // 设置初始工作目录
    , historyPosition(-1)
{
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

    // 显示初始提示符
    insertPrompt();
    
    // 配置终端显示
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 禁用水平滚动条
    setLineWrapMode(QPlainTextEdit::WidgetWidth);         // 启用自动换行
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
    // 获取光标位置
    QTextCursor cursor = textCursor();
    
    // 防止修改历史命令输出
    if (cursor.position() < promptPosition && e->key() != Qt::Key_Control) {
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
    }

    // 处理不同的按键事件
    switch (e->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter: {
        // 获取并执行命令
        QString command = getCurrentCommand();
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

    // 特殊处理cd命令
    if (command.startsWith("cd ")) {
        handleCdCommand(command.mid(3).trimmed());
        return;
    }

    // 执行其他命令
    #ifdef Q_OS_WIN
        process->start("cmd.exe", QStringList() << "/c" << command);
    #else
        process->start("bash", QStringList() << "-c" << command);
    #endif
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
    return QDir(workingDirectory).dirName();
}

/**
 * @brief 处理进程标准输出
 */
void Terminal::handleProcessOutput()
{
    QByteArray output = process->readAllStandardOutput();
    insertPlainText(QString::fromLocal8Bit(output));
    
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
    insertPlainText(QString::fromLocal8Bit(error));
    
    // 滚动到底部
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

/**
 * @brief 处理进程结束
 */
void Terminal::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    insertPrompt();  // 显示新的提示符
}

/**
 * @brief 插入命令提示符
 */
void Terminal::insertPrompt()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(prompt);
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
    QString line = cursor.selectedText();
    return line.mid(prompt.length());
}

/**
 * @brief 设置进程环境变量
 * @param environment 环境变量设置
 */
void Terminal::setProcessEnvironment(const QProcessEnvironment &environment)
{
    process->setProcessEnvironment(environment);
} 
