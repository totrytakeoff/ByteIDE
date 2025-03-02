#ifndef EDITAREA_H
#define EDITAREA_H
#include <QMap>
#include <QWidget>
#include <QSettings>
#include <QSci/qscilexercpp.h>

class QsciScintilla;
class QTabWidget;
class QColor;
class QsciLexerPython;
class QsciLexerD;


using ThemeColors=QVector<QColor>;
// class QMap;
///world

class EditArea : public QWidget
{
    Q_OBJECT
public:

    enum ColorType{
        LineTagColor,
        FoldingColor,
        MarginBackColor,
        MarginsForeground,
        LineHighLightColor,
        CursorColor,
        FontDefaultColor,
        EditorDefaultBackgroundColor,

        CallTipBackgroundColor,
        CallTipForegroundColor,
        CallTipHighlightColor,

        MatchedBraceBackgroundColor,
        MatchedBraceForegroundColor,
        ColorTypeCount // 保持最后一位,记录枚举总数
    };




    EditArea(QWidget *parent=nullptr);
    ~EditArea();

    void InitLexer();
    void InitTextEdit();
    void ModifyLexerColor(int WordType,QColor &col);
    void CloseLineTag(bool flag);

    void setCurLexer(QString &type);

    void modifyLineTagColor(QColor &col);
    void modifyFoldingColor(QColor &col);
    void modifyMarginBackColor(QColor &col);
    void modifyLineHighLightColor(QColor &col);
    void modifyCursorColor(QColor &col);


    void modifyFontDefaultColor(QColor &col);
    void modifyEditorDefaultBackgroundColor(QColor &col);


    void setTheme(QString theme) ;


    QsciScintilla *textEdit;
    QsciLexerPython *PythonLexer;
    QsciLexerCPP *CppLexer;
    QsciLexerD *DefaultLexer;

    QString curEditFile;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    //void keyPressEvent(QKeyEvent *event) override;
    //基类不是qsciscintilla，不能直接重写keyPressEvent，可通过事件过滤器来实现类似效果



public slots:
    //高亮当前行
    void highlightCurrentLine(int line,int index);

    void findNext(const QString &text, bool caseSensitive, bool wholeWords,bool forward=true);

    void selectAllText(const QString &text);

    void highLightAll(const QString &text);

    void replaceText(const QString &origin,const QString &replaced);

    void replaceAll(const QString &origin,const QString &replaced);

    //行注释
    void setCommentline();
signals:


private:


private:
    enum{
        SymMargin1=0,
        SymMargin2=1,
        SymMargin3=3,
        LineNumMargin=2,
        FoldMargin=4
    };


    bool LineTagIsOpen=true;
    // QColor currentLineTagColor=QColor(241, 196, 15);
    // QColor FoldingColor=QColor(56, 57, 59);
    // QColor MarginBackColor=QColor(30, 30, 30);
    // QColor LineHighLightColor=QColor(46, 47, 48);
    // QColor CursorColor=Qt::white;
    // QColor FontDefaultColor=QColor(214, 207, 154);
    // QColor EditorDefaultBackgroundColor= QColor(35, 35, 35);

    // QColor MatchedBraceBackgroundColor=QColor(255, 107, 129);
    // QColor MatchedBraceForegroundColor=QColor(241, 196, 15);


    QColor DefaultTextColor=QColor(220,220,220);
    QMap<int,QColor> Lexer_Color;

    QMap<int, QColor> darkColorMap{
        // 默认文本
        {QsciLexerCPP::Default, QColor(214, 207, 154)},

        // 注释
        {QsciLexerCPP::Comment, QColor(87, 166, 74)},
        {QsciLexerCPP::CommentLine, QColor(87, 166, 74)},
        {QsciLexerCPP::CommentDoc, QColor(87, 166, 74)},
        {QsciLexerCPP::CommentLineDoc, QColor(87, 166, 74)},

        // 数字
        {QsciLexerCPP::Number, QColor(181, 206, 168)},

        // 关键字
        {QsciLexerCPP::Keyword, QColor(86, 156, 214)},
        {QsciLexerCPP::KeywordSet2, QColor(216, 160, 223)},  // 第二组关键字

        // 字符串
        {QsciLexerCPP::DoubleQuotedString, QColor(214, 157, 133)},
        {QsciLexerCPP::SingleQuotedString, QColor(214, 157, 133)},
        {QsciLexerCPP::UnclosedString, QColor(138, 96, 44)},
        {QsciLexerCPP::VerbatimString, QColor(138, 96, 44)},
        {QsciLexerCPP::TripleQuotedVerbatimString, QColor(214, 157, 133)},

        // 预处理器指令
        {QsciLexerCPP::PreProcessor, QColor(155, 155, 155)},

        // 操作符
        {QsciLexerCPP::Operator, QColor(180, 180, 180)},

        // 标识符
        {QsciLexerCPP::Identifier, QColor(220, 220, 170)},

        // 其他特殊样式
        {QsciLexerCPP::UserLiteral, QColor(78, 201, 176)},
        {QsciLexerCPP::TaskMarker, QColor(193, 44, 31)},
        {QsciLexerCPP::EscapeSequence, QColor(213, 235, 225)},
        // {QsciLexerCPP::EscapeChar, QColor()},  // 保留注释的未实现项

        // 不活动区域的样式
        {QsciLexerCPP::InactiveDefault, QColor(214, 207, 154)},
        {QsciLexerCPP::InactiveComment, QColor(87, 166, 74)},
        {QsciLexerCPP::InactiveCommentLine, QColor(87, 166, 74)},
        {QsciLexerCPP::InactiveNumber, QColor(181, 206, 168)},
        {QsciLexerCPP::InactiveKeyword, QColor(86, 156, 214)},
        {QsciLexerCPP::InactiveDoubleQuotedString, QColor(214, 157, 133)},
        {QsciLexerCPP::InactiveSingleQuotedString, QColor(214, 157, 133)},
        {QsciLexerCPP::InactivePreProcessor, QColor(155, 155, 155)},
        {QsciLexerCPP::InactiveOperator, QColor(180, 180, 180)},
        {QsciLexerCPP::InactiveIdentifier, QColor(218, 218, 218)},
        {QsciLexerCPP::InactiveUnclosedString, QColor(218, 218, 218)},
        {QsciLexerCPP::InactiveRawString, QColor(218, 218, 218)}
    };

    QMap<int, QColor> lightColorMap{
        // 默认文本（深灰色确保对比度）
        {QsciLexerCPP::Default, QColor(60, 60, 60)},

        // 注释（森林绿保持专业感）
        {QsciLexerCPP::Comment, QColor(34, 139, 34)},
        {QsciLexerCPP::CommentLine, QColor(34, 139, 34)},
        {QsciLexerCPP::CommentDoc, QColor(34, 139, 34)},
        {QsciLexerCPP::CommentLineDoc, QColor(34, 139, 34)},

        // 数字（深蓝绿色提升辨识度）
        {QsciLexerCPP::Number, QColor(28, 112, 140)},

        // 关键字（深蓝色系增强重点）
        {QsciLexerCPP::Keyword, QColor(0, 90, 156)},        // 主关键字
        {QsciLexerCPP::KeywordSet2, QColor(123, 31, 162)},  // 辅关键字（紫色区分）

        // 字符串（深橙色系保持温暖感）
        {QsciLexerCPP::DoubleQuotedString, QColor(191, 54, 12)},
        {QsciLexerCPP::SingleQuotedString, QColor(191, 54, 12)},
        {QsciLexerCPP::UnclosedString, QColor(206, 92, 0)},     // 错误提示色
        {QsciLexerCPP::VerbatimString, QColor(206, 92, 0)},
        {QsciLexerCPP::TripleQuotedVerbatimString, QColor(191, 54, 12)},

        // 预处理器（深灰色保持低调）
        {QsciLexerCPP::PreProcessor, QColor(96, 96, 96)},

        // 操作符（中灰色保持可见性）
        {QsciLexerCPP::Operator, QColor(120, 120, 120)},

        // 标识符（深土黄色避免苍白）
        {QsciLexerCPP::Identifier, QColor(150, 150, 50)},

        // 特殊样式
        {QsciLexerCPP::UserLiteral, QColor(0, 137, 123)},      // 青蓝色提升科技感
        {QsciLexerCPP::TaskMarker, QColor(229, 57, 53)},       // 警示红突出标记
        {QsciLexerCPP::EscapeSequence, QColor(21, 101, 112)},  // 深青色保证清晰
        // {QsciLexerCPP::EscapeChar, QColor()},                // 保留占位

        // 非活动区域（降低对比度但保持关联）
        {QsciLexerCPP::InactiveDefault, QColor(150, 150, 150)},
        {QsciLexerCPP::InactiveComment, QColor(154, 205, 50)}, // 橄榄绿区分状态
        {QsciLexerCPP::InactiveCommentLine, QColor(154, 205, 50)},
        {QsciLexerCPP::InactiveNumber, QColor(106, 153, 160)},
        {QsciLexerCPP::InactiveKeyword, QColor(84, 110, 122)},
        {QsciLexerCPP::InactiveDoubleQuotedString, QColor(194, 139, 108)},
        {QsciLexerCPP::InactiveSingleQuotedString, QColor(194, 139, 108)},
        {QsciLexerCPP::InactivePreProcessor, QColor(158, 158, 158)},
        {QsciLexerCPP::InactiveOperator, QColor(158, 158, 158)},
        {QsciLexerCPP::InactiveIdentifier, QColor(158, 147, 94)},
        {QsciLexerCPP::InactiveUnclosedString, QColor(234, 195, 176)},
        {QsciLexerCPP::InactiveRawString, QColor(234, 195, 176)}
    };

    ThemeColors ColorsList;

    ThemeColors darkColorList={
        QColor(241, 196, 15),
        QColor(56, 57, 59),
        QColor(30, 30, 30),
        QColor(255,255,255),
        QColor(46, 47, 48),
        QColor(255,255,255),
        QColor(214, 207, 154),
        QColor(35, 35, 35),

        QColor(30, 30, 30),
        QColor(210,210,210),
        QColor(255, 71, 87),


        QColor(255, 107, 129),
        QColor(241, 196, 15)

    };


    ThemeColors lightColorList={
        QColor(41, 128, 185),      // LineTagColor（更醒目的蓝色）
        QColor(220, 220, 220),     // FoldingColor（浅灰色折叠标记）
        QColor(245, 245, 245),     // MarginBackColor（浅灰边栏背景）
        QColor(0,0,0),
        QColor(240, 248, 255),     // LineHighLightColor（淡蓝色行高亮）
        QColor(30, 30, 30),        // CursorColor（深色光标）
        QColor(50, 50, 50),        // FontDefaultColor（深灰文字）
        QColor(255,255,255),       // EditorDefaultBackgroundColor（纯白编辑器背景）

        QColor(255, 255, 255),     // CallTipBackgroundColor（白色提示背景）
        QColor(80, 80, 80),        // CallTipForegroundColor（深灰提示文字）
        QColor(231, 76, 60),       // CallTipHighlightColor（强调红色）

        QColor(155, 203, 255),     // MatchedBraceBackground（浅蓝括号匹配背景）
        QColor(41, 128, 185)       // MatchedBraceForeground（与LineTag同色系）
    };



    int m_flags;
    int m_lastFindPos;
    int highlightIndicator = 7; // 需要初始化指示器ID
    QString commentFlag;

    QVector<QPair<int,int>> matchPosVec;


    QString m_Theme;
    QSettings settings;
};

#endif // EDITAREA_H
