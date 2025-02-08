#include "searchwidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

SearchWidget::SearchWidget(QWidget *parent)
    :QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    setMinimumHeight(80);
    setMaximumHeight(80);
    setStyle();

    findText = new QLineEdit(this);
    replaceText= new QLineEdit(this);

    caseSensitive = new QCheckBox("区分大小写", this);
    wholeWords = new QCheckBox("仅匹配整个单词", this);
    nextBtn =new QPushButton("查找下一个",this);
    prveBtn =new QPushButton("查找前一个",this);
    replaceBtn = new QPushButton("替换",this);
    replaceAllBtn = new QPushButton("替换全部",this);
    selectAllBtn=new QPushButton("选择全部",this);

    QLabel *findLab = new QLabel("查找:",this);
    QLabel *replaceLab = new QLabel("替换为:",this);

        // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *findLayout = new QHBoxLayout();
    QHBoxLayout *replaceLayout = new QHBoxLayout();
    QHBoxLayout *argLayout=new QHBoxLayout();

    // 添加查找输入框到布局
    // findLayout->addWidget(findLabel);
    argLayout->addWidget(wholeWords,1);
    argLayout->addWidget(caseSensitive,1);

    findLayout->addWidget(findLab,0);
    findLayout->addWidget(findText,7);
    findLayout->addWidget(prveBtn,1);
    findLayout->addWidget(nextBtn,1);
    findLayout->addWidget(selectAllBtn,1);

    replaceLayout->addWidget(replaceLab,0);
    replaceLayout->addWidget(replaceText,7);
    replaceLayout->addWidget(replaceBtn,1);
    replaceLayout->addWidget(replaceAllBtn,1);

    mainLayout->addLayout(argLayout,0);
    mainLayout->addLayout(findLayout,1);
    mainLayout->addLayout(replaceLayout,1);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    // 设置主布局
    setLayout(mainLayout);


    // connect(findText,&QLineEdit::cursorPositionChanged , this, &SearchWidget::findClicked);
    connect(nextBtn,&QPushButton::clicked,this,[this](){
        emit findNext(findText->text(), caseSensitive->isChecked(), wholeWords->isChecked(),true);
    });
    connect(prveBtn,&QPushButton::clicked,this,[this](){
        emit findNext(findText->text(), caseSensitive->isChecked(), wholeWords->isChecked(),false);
    });

}

SearchWidget::~SearchWidget()
{

}

void SearchWidget::findClicked()
{
    emit findNext(findText->text(), caseSensitive->isChecked(), wholeWords->isChecked());

}

void SearchWidget::setStyle()
{
    QString StyleSheet=R"(
           QLineEdit {
                color: rgb(220,220,220);
                background-color: rgb(56, 56, 56);
            }
            QPushButton {
                color: white;
                background-color: rgb(61, 61, 61);
            }
            QLabel{
                color: rgb(220,220,220);
            }
            QCheckBox{
                color: rgb(220,220,220);
            }

)";

    setStyleSheet(StyleSheet);

}
