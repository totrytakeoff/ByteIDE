#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H


#include <QDialog>
#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();


private slots:
    void findClicked();
signals:
    void findNext(const QString &text, bool caseSensitive, bool wholeWords,bool forward=true);
    void inputSearchWord(const QString &text);
    void selectAll(const QString &text);
    void replace(const QString &origin,const QString &replaced);
    void replaceAll(const QString &origin,const QString &replaced);

private:
    void setStyle();

private:

    QLineEdit *findText;
    QLineEdit *replaceText;


    QCheckBox *caseSensitive;
    QCheckBox *wholeWords;
    QPushButton *nextBtn;
    QPushButton *prveBtn;
    QPushButton *replaceBtn;
    QPushButton *selectAllBtn;
    QPushButton *replaceAllBtn;
    QPushButton *CloseBtn;
};

#endif // SEARCHWIDGET_H

