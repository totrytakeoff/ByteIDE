#ifndef CODERUNNER_H
#define CODERUNNER_H

#include <QObject>
#include <QWidget>
#include <QProcess>

class QString;


class CodeRunner:public QWidget
{
    Q_OBJECT
public:
    CodeRunner(QWidget* parent);
    ~CodeRunner();

    enum class Mode{
        UnKown,
        Cpp,
        Python
    };

    void setMode(QString &fileType);

    QString runCode();

    QString runCppCode();

    QString runPythonCode();

    void searchRunner();

    void setRunFile(QString &file);

    QString searchFiles(const QString &dirPath, const QString &searchPattern);

    // void initProcess();

    // void handleProcessOutput();

    // void handleProcessError();

    // void handleProcessFinish(int exitCode, QProcess::ExitStatus exitstatus);

    // void handleProcessInput();

private:
    QString FileType;
    QString runFile;
    QStringList runFileList;

    Mode RunMode=Mode::UnKown;

    QString PythonRunner;
    QString CppRunner;
    // QProcess *process;

    bool isRunnerExist;
};

#endif // CODERUNNER_H
