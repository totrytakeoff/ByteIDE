#ifndef TERMINAL_H
#define TERMINAL_H

#include <QPlainTextEdit>
#include <QProcess>

/**
 * @brief 终端模拟器类
 * 
 * Terminal类继承自QPlainTextEdit，实现了一个简单的命令行终端模拟器。
 * 它可以执行系统命令，显示输出，并支持命令历史记录功能。
 */
class Terminal : public QPlainTextEdit
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件
     * 
     * 初始化终端，设置外观，创建进程，连接信号槽
     */
    explicit Terminal(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     * 
     * 确保在销毁对象时正确终止运行中的进程
     */
    ~Terminal();

    /**
     * @brief 执行命令
     * @param command 要执行的命令字符串
     * 
     * 在终端中执行指定的命令，并显示输出结果
     */
    void executeCommand(const QString &command);

    /**
     * @brief 设置进程环境变量
     * @param environment QProcessEnvironment对象，包含环境变量设置
     * 
     * 为终端进程设置环境变量，用于Python解释器等外部程序的执行
     */
    void setProcessEnvironment(const QProcessEnvironment &environment);


    void setIsRunning(bool flag);

protected:
    /**
     * @brief 键盘事件处理函数
     * @param e 键盘事件对象
     * 
     * 处理用户输入，包括：
     * - Enter键执行命令
     * - Up/Down键浏览命令历史
     * - 防止修改历史输出
     */
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    /**
     * @brief 处理进程标准输出
     * 
     * 读取并显示命令执行的标准输出
     */
    void handleProcessOutput();

    /**
     * @brief 处理进程错误输出
     * 
     * 读取并显示命令执行的错误输出
     */
    void handleProcessError();

    /**
     * @brief 处理进程结束
     * @param exitCode 进程退出码
     * @param exitStatus 进程退出状态
     * 
     * 当命令执行完成时，显示新的提示符
     */
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);



private:
    /**
     * @brief 插入命令提示符
     * 
     * 在终端末尾插入一个新的命令提示符
     */
    void insertPrompt();

    /**
     * @brief 获取当前命令
     * @return 返回当前输入的命令文本
     * 
     * 获取提示符后面用户输入的命令文本
     */
    QString getCurrentCommand() const;

    /**
     * @brief 处理cd命令
     * @param dir 目标目录路径
     * 
     * 特殊处理cd命令，改变当前工作目录
     */
    void handleCdCommand(const QString &dir);

    /**
     * @brief 获取当前工作目录名
     * @return 返回当前工作目录的名称
     * 
     * 获取当前工作目录的名称，用于显示在提示符中
     */
    QString getCurrentWorkingDir() const;


    bool handleInternalCommand(const QString &command);
    
    // 成员变量
    QProcess *process;          ///< 用于执行命令的进程对象
    QString prompt;            ///< 命令提示符文本
    int promptPosition;        ///< 提示符在文本中的位置
    QString workingDirectory;  ///< 当前工作目录路径
    QStringList commandHistory; ///< 命令历史记录列表
    int historyPosition;       ///< 当前历史记录的位置索引

    bool isrunning=false;
};

#endif // TERMINAL_H 
