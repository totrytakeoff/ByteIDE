#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QEvent>
// #include <QCoreApplication>


class QStackedWidget;
class QSpinBox;
class QFontComboBox;
class QComboBox;
class QPushButton;
class QListWidget;


class SettingWidget : public QWidget
{
    Q_OBJECT
public:
    enum class Themes{Light,Dark};
    enum class SettingOption{UI};



    explicit SettingWidget(QWidget *parent = nullptr);
    ~SettingWidget();



    void setTheme(Themes theme);

    void setDefultFont(QFont &font);

    QFont loadFont();

    QString loadTheme();

protected:
    virtual void closeEvent(QCloseEvent *e)override;

private:

    void setOp_UI();

    void saveSettings();


signals:
    void save();
public:
    QSettings settings;

private:
    QStringList ThemesList={
        "light","dark"
    };
    QStringList SettingOptionList={
        "UI"
    };


    QStackedWidget *stackWidget;
    QSpinBox *FontSizeSpin;
    QFontComboBox *fontCombo;
    QComboBox *ThemeCombo;
    QListWidget *settingListWidget;

};

#endif // SETTINGWIDGET_H
