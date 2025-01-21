#ifndef CODERUNNER_H
#define CODERUNNER_H

#include <QObject>
#include <QWidget>

class QString;


class CodeRunner:public QWidget
{
public:
    CodeRunner(QWidget* parent);
    ~CodeRunner();

    enum class Mode{
        Cpp,
        Python
    };

    void setMode(QString &fileType);

    void runCode();

    void runCppCode();

    void runPythonCode();

    void searchRunner();

    QString searchFiles(const QString &dirPath, const QString &searchPattern);




private:
    QString FileType;
    QString runFile;

    Mode RunMode;

    QString PythonRunner;
    QString CppRunner;


    bool isRunnerExist;
};

#endif // CODERUNNER_H
