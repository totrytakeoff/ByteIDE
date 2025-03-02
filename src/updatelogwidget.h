#ifndef UPDATELOGWIDGET_H
#define UPDATELOGWIDGET_H

#include <QWidget>

class UpdateLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UpdateLogWidget(QWidget *parent = nullptr);

    void applyTheme(const QString &theme);




signals:
};

#endif // UPDATELOGWIDGET_H
