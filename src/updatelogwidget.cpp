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
        "<h2>版本 1.1.0 - 2025年2月18日</h2>"
        "<ul>"
        "<li>修复了重命名时高亮的高亮丢失问题</li>"
        "<li>修复了因Python默认输入输出缓存导致的不能实时输出的问题</li>"
        "<li>优化了界面布局，新增强制终止运行中的程序按钮</li>"
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
