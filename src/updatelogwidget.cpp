#include "updatelogwidget.h"

#include <QVBoxLayout>
#include <QTextBrowser>

UpdateLogWidget::UpdateLogWidget(QWidget *parent)
    : QWidget{parent}
{
    // 设置窗口标题
    setWindowTitle("版本更新内容");

    // 创建一个中心部件
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 创建一个QTextBrowser来显示更新日志
    QTextBrowser *textBrowser = new QTextBrowser(this);
    textBrowser->setOpenExternalLinks(true);  // 如果有链接可以点击打开
    textBrowser->setHtml(
        "<h1>更新内容</h1>"
        "<h2>版本 1.1.2 - 2025年2月20日</h2>"
        "<ul>"
        "<li>修复了Pyhton连续两个input读取内容错误的问题</li>"
        "</ul>"
        "<br><br><br>"
        "更多信息请关注<a href= \"https://github.com/totrytakeoff/IDE-based-on-Qt\" >github仓库</a>与<a href=\" https://www.byteoj.com/posts/257\">ByteOJ网站</a>"
        );

    textBrowser->setTextBackgroundColor(Qt::black);
    textBrowser->setTextColor(Qt::white);
    layout->addWidget(textBrowser);
    centralWidget->setLayout(layout);
    this->setLayout(layout);
    resize(400,300);
}
