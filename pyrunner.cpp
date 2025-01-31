#include "pyrunner.h"
#include <QScrollBar>
#include <QDebug>


// #include


PyRunner::PyRunner(QWidget *parent)
    : Terminal{parent}
{
}

PyRunner::~PyRunner()
{

}

void PyRunner::executeCommand(const QString &command)
{
    Q_UNUSED(command);
    // isrunning=true;
}

void PyRunner::runCode(QString &runner, QStringList &target)
{
    isrunning=true;

    process->start(runner,target);

}



