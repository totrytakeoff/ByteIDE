#include "searchwidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>


SearchWidget::SearchWidget(QWidget *parent)
    :QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    setMinimumHeight(80);
    setMaximumHeight(80);


    findText = new QLineEdit(this);
    replaceText= new QLineEdit(this);

    caseSensitive = new QCheckBox("区分大小写", this);
    wholeWords = new QCheckBox("仅匹配整个单词", this);

    nextBtn =new QPushButton("查找下一个",this);
    prveBtn =new QPushButton("查找前一个",this);
    replaceBtn = new QPushButton("替换",this);
    replaceAllBtn = new QPushButton("替换全部",this);
    selectAllBtn=new QPushButton("选择全部",this);

    CloseBtn=new QPushButton("X",this);
    // 推荐方式：通过布局管理器控制尺寸
    CloseBtn->setFixedSize(24, 24);  // 固定尺寸
    CloseBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // 固定尺寸策略



    QLabel *findLab = new QLabel("查找:",this);
    QLabel *replaceLab = new QLabel("替换为:",this);

        // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *argLayout=new QHBoxLayout();

    QVBoxLayout * labelLayout=new QVBoxLayout;
    QVBoxLayout * lineEditLayout=new QVBoxLayout;

    QHBoxLayout * HLayout=new QHBoxLayout;

    QGridLayout *gridLayout=new QGridLayout();

    argLayout->addWidget(wholeWords,1);
    argLayout->addWidget(caseSensitive,1);
    argLayout->addWidget(CloseBtn,0);

    labelLayout->addWidget(findLab,0);
    labelLayout->addWidget(replaceLab,0);

    lineEditLayout->addWidget(findText,0);
    lineEditLayout->addWidget(replaceText,0);

    gridLayout->addWidget(prveBtn,0,0,1,1);
    gridLayout->addWidget(nextBtn,0,1,1,1);
    gridLayout->addWidget(selectAllBtn,0,2,1,1);
    gridLayout->addWidget(replaceBtn,1,0,1,1);
    gridLayout->addWidget(replaceAllBtn,1,1,1,1);

    // 添加占位控件保持列数一致
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    gridLayout->addWidget(spacer, 1, 2);

    gridLayout->setColumnStretch(0,1);
    gridLayout->setColumnStretch(1,1);
    gridLayout->setColumnStretch(2,1);

    HLayout->addLayout(labelLayout,0);
    HLayout->addLayout(lineEditLayout,8);
    HLayout->addLayout(gridLayout,2);

    mainLayout->addLayout(argLayout,0);
    mainLayout->addLayout(HLayout,1);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    // 设置主布局
    setLayout(mainLayout);


    connect(findText,&QLineEdit::cursorPositionChanged , this,[this](){

            emit inputSearchWord(findText->text());

    });
    connect(nextBtn,&QPushButton::clicked,this,[this](){
        emit findNext(findText->text(), caseSensitive->isChecked(), wholeWords->isChecked(),true);
    });
    connect(prveBtn,&QPushButton::clicked,this,[this](){
        emit findNext(findText->text(), caseSensitive->isChecked(), wholeWords->isChecked(),false);
    });

    connect(replaceBtn,&QPushButton::clicked,this,[this](){
        emit replace(findText->text(),replaceText->text());
    });
    connect(replaceAllBtn,&QPushButton::clicked,this,[this](){
        emit replaceAll(findText->text(),replaceText->text());
    });
    connect(selectAllBtn,&QPushButton::clicked,this,[this](){
        emit selectAll(findText->text());
    });


    connect(CloseBtn,&QPushButton::clicked,this,&QWidget::close);

}

SearchWidget::~SearchWidget()
{

}

void SearchWidget::findClicked()
{
    emit findNext(findText->text(), caseSensitive->isChecked(), wholeWords->isChecked());

}

