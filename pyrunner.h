#ifndef PYRUNNER_H
#define PYRUNNER_H

#include <QWidget>
#include "terminal.h"

class QProcess;


class PyRunner : public Terminal
{
    Q_OBJECT
public:
    explicit PyRunner(QWidget *parent = nullptr);

    ~PyRunner();




    void executeCommand(const QString &command) override;

    void runCode(QString &runner,QStringList &target);


signals:

private:

};

#endif // PYRUNNER_H
