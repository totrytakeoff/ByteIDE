#include "updatelogwidget.h"
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPalette>

UpdateLogWidget::UpdateLogWidget(QWidget *parent)
    : QWidget(parent)
{
    // 窗口属性设置
    setWindowTitle(tr("版本更新内容"));
    setMinimumSize(500, 400);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建带样式的文本浏览器
    QTextBrowser *textBrowser = new QTextBrowser(this);
    textBrowser->setOpenExternalLinks(true);

    // 适配暗色主题的配色方案
    QPalette palette = textBrowser->palette();
    palette.setColor(QPalette::Base, QColor(53, 53, 53));
    palette.setColor(QPalette::Text, Qt::white);
    textBrowser->setPalette(palette);

    // 构建带格式的更新日志内容
    QString htmlContent =
        "<h1 style='color: #87CEEB;'>ByteOJ IDE 更新日志</h1>"

        "<h2 style='color: #98FB98;'>版本 1.2.0 - 2025年3月1日</h2>"
        "<ul>"
        "<li>🎉 新增设置页面，支持亮/暗双主题切换</li>"
        "<li>🔄 新增终端重启功能，增强稳定性</li>"
        "<li>⌨️ 新增注释快捷键 (Ctrl+Shift+/)  支持多行注释</li>"
        "</ul>"

        "<h2 style='color: #98FB98;'>版本 1.1.2 - 2025年2月20日</h2>"
        "<ul>"
        "<li>🐛 修复Python连续input读取错误问题</li>"
        "<li>🔧 优化终端输出缓冲机制</li>"
        "</ul>"

        "<div style='margin-top: 20px;'>"
        "📢 更多信息请访问：<br>"
        "<a href='https://github.com/totrytakeoff/IDE-based-on-Qt' style='color: #00BFFF;'>GitHub仓库</a> | "
        "<a href='https://www.byteoj.com/posts/257' style='color: #00BFFF;'>ByteOJ官网</a>"
        "</div>";

    textBrowser->setHtml(htmlContent);
    mainLayout->addWidget(textBrowser);


    setLayout(mainLayout);
}
