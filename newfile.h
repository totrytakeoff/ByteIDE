#ifndef NEWFILE_H
#define NEWFILE_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class QLabel;

class NewFile : public QDialog
{
    Q_OBJECT
public:
    explicit NewFile(QWidget *parent,QString path);
    ~NewFile();
    void CreatNewFiles(QString &path);
    QString GetNewFile();


private slots:
    void onConfirm();
    void onCancel();
    void onSelectDir();
    void onInputFileName();
private:
    QLineEdit *lineEdit;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;
    QPushButton *selectDirBtn;
    QLabel *pathLab;
    QString filePath;
    QString FileString;
};

#endif // NEWFILE_H
