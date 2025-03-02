#include "settingwidget.h"
#include <QPushButton>
#include <QAction>
#include <QStackedWidget>
#include <QFontComboBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
// #include <QCoreApplication>

SettingWidget::SettingWidget(QWidget *parent)
    : QWidget{parent}
{


    stackWidget=new QStackedWidget(this);
    settingListWidget=new QListWidget(this);
    settingListWidget->addItems(SettingOptionList);
    settingListWidget->setMinimumWidth(60);
    settingListWidget->setMaximumWidth(80);

    setOp_UI();


    QHBoxLayout *mainLayout=new QHBoxLayout();
    mainLayout->addWidget(settingListWidget);
    mainLayout->addWidget(stackWidget);

    connect(settingListWidget,&QListWidget::currentRowChanged,stackWidget,&QStackedWidget::setCurrentIndex);


    this->setLayout(mainLayout);


    resize(600,300);
}

SettingWidget::~SettingWidget(){}

void SettingWidget::closeEvent(QCloseEvent *e)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "保存设置", "是否保存并应用设置？",
                                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (reply == QMessageBox::Save) {
        saveSettings();
        e->accept();  // 关闭窗口
    } else if (reply == QMessageBox::Discard) {
        // 不保存，直接关闭窗口
        e->accept();
    } else {
        e->ignore();
    }

    emit save();
    QStringList sl =settings.allKeys();
    for(QString s:sl){
        qDebug()<<s<<":"<<settings.value(s);
    }

}

void SettingWidget::setOp_UI()
{
    QWidget *UI=new QWidget();

    QLabel *themelab=new QLabel("主题: ");
    ThemeCombo=new QComboBox();

    ThemeCombo->addItems(ThemesList);

    ThemeCombo->setCurrentIndex(ThemesList.indexOf(settings.value("UI/Theme",QString("dark"))));


    QLabel *font=new QLabel("字体与大小: ");
    fontCombo=new QFontComboBox();
    QFont ft = settings.value("UI/Font",QFont("Consolas",14)).value<QFont>();
    fontCombo->setCurrentFont(ft);


    FontSizeSpin= new QSpinBox();
    FontSizeSpin->setRange(1,50);
    FontSizeSpin->setSuffix("px");
    int fontsize=ft.pointSize();
    qDebug()<<"fs: -"<<fontsize;
    if(fontsize==-1){
        fontsize=ft.pixelSize();
    }
    qDebug()<<fontsize;
    FontSizeSpin->setValue(fontsize);



    QHBoxLayout *Hlayout1=new QHBoxLayout();
    Hlayout1->addWidget(themelab);
    Hlayout1->addWidget(ThemeCombo);

    QHBoxLayout *Hlayout2=new QHBoxLayout();
    Hlayout2->addWidget(font);
    Hlayout2->addWidget(fontCombo);
    Hlayout2->addWidget(FontSizeSpin);

    QVBoxLayout *Vlayout1=new QVBoxLayout();
    Vlayout1->addLayout(Hlayout1,1);
    Vlayout1->addLayout(Hlayout2,1);

    UI->setLayout(Vlayout1);


    stackWidget->addWidget(UI);
}

void SettingWidget::saveSettings()
{   QFont ft=fontCombo->currentFont();
    ft.setPointSize(FontSizeSpin->value());
    ft.setPointSize(FontSizeSpin->value());

    settings.beginGroup("UI");

    settings.setValue("Font",ft);
    settings.setValue("Theme",ThemesList[ThemeCombo->currentIndex()]);

    settings.endGroup();

}


