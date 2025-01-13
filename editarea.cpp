#include "editarea.h"
#include<Qsci/qscilexercpp.h>
#include<Qsci/qsciscintilla.h>
#include<QTabWidget>
#include<QDebug>
#include<QMetaMethod>
#include<QString>

#include<Qsci/qscistyledtext.h>
#include<Qsci/qsciapis.h>



EditArea::EditArea() {


    tabwidget=new QTabWidget();
    textEdit=new QsciScintilla();
    CppLexer=new QsciLexerCPP();
    Lexer_Color=createDefaultColorMap();

    InitLexer();
    InitTextEdit();




    // 连接光标位置变化信号到自定义槽函数

    if (this->LineTagIsOpen) {
        connect(textEdit, &QsciScintilla::cursorPositionChanged, this, &EditArea::highlightCurrentLine);
        qDebug() << "Line tag has open";
    } else {
        qDebug() << "Line tag has close";
    }


}

EditArea::~EditArea()
{

}

void EditArea::InitLexer()
{

    CppLexer->setDefaultColor(FontDefaultColor);///默认字体颜色
    CppLexer->setDefaultPaper(EditorDefaultBackgroundColor);///默认背景色
    CppLexer->setFont(QFont("Consolas",14));///默认字体

    CppLexer->setFoldAtElse(true);       // 在 else 处折叠代码块
    CppLexer->setFoldCompact(true);      // 折叠时隐藏空行



    //.................................................................


    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Keyword),QsciLexerCPP::Keyword);//设置关键字高亮颜色
    // 设置注释颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Comment), QsciLexerCPP::Comment);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::CommentLine), QsciLexerCPP::CommentLine);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::CommentDoc), QsciLexerCPP::CommentDoc);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::CommentLineDoc), QsciLexerCPP::CommentLineDoc);

    // 设置数字颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Number), QsciLexerCPP::Number);

    // 设置关键字颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Keyword), QsciLexerCPP::Keyword);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::KeywordSet2), QsciLexerCPP::KeywordSet2); // 第二组关键字

    // 设置字符串颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::DoubleQuotedString), QsciLexerCPP::DoubleQuotedString);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::SingleQuotedString), QsciLexerCPP::SingleQuotedString);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::UnclosedString), QsciLexerCPP::UnclosedString);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::VerbatimString), QsciLexerCPP::VerbatimString);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::TripleQuotedVerbatimString), QsciLexerCPP::TripleQuotedVerbatimString);

    // 设置预处理器指令颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::PreProcessor), QsciLexerCPP::PreProcessor);

    // 设置操作符颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Operator), QsciLexerCPP::Operator);

    // 设置标识符颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Identifier), QsciLexerCPP::Identifier);

    // 设置其他特殊样式颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::UserLiteral), QsciLexerCPP::UserLiteral);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::TaskMarker), QsciLexerCPP::TaskMarker);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::EscapeSequence), QsciLexerCPP::EscapeSequence);
    // CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::EscapeChar), QsciLexerCPP::EscapeChar);

    // 设置不活动区域样式颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveDefault), QsciLexerCPP::InactiveDefault);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveComment), QsciLexerCPP::InactiveComment);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveCommentLine), QsciLexerCPP::InactiveCommentLine);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveNumber), QsciLexerCPP::InactiveNumber);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveKeyword), QsciLexerCPP::InactiveKeyword);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveDoubleQuotedString), QsciLexerCPP::InactiveDoubleQuotedString);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveSingleQuotedString), QsciLexerCPP::InactiveSingleQuotedString);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactivePreProcessor), QsciLexerCPP::InactivePreProcessor);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveOperator), QsciLexerCPP::InactiveOperator);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveIdentifier), QsciLexerCPP::InactiveIdentifier);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveUnclosedString), QsciLexerCPP::InactiveUnclosedString);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::InactiveRawString), QsciLexerCPP::InactiveRawString);


    textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
    textEdit->setAutoCompletionThreshold(1); // 当用户输入至少一个字符后触发自动完成
    textEdit->setAutoCompletionReplaceWord(true); // 自动替换当前词

    QStringList customSeparators = {" ", "(", ")", ",", ";", ".", ":", "[", "]", "{", "}"};
    textEdit->setAutoCompletionWordSeparators(customSeparators);


    ///自动补全的实现
   /* QsciAPIs *apis = new QsciAPIs(textEdit->lexer());
    apis->load("path/to/api/file.api");*////后序添加语言api文件解析keyword实现自动补全

    // 获取并添加Lexer提供的关键字到自动完成列表


    QString keywords =  CppLexer->keywords(-1);; // -1 表示所有关键字集合
    QStringList keywordList = keywords.split(" ", Qt::SkipEmptyParts);
    QsciAPIs *apis = new QsciAPIs(CppLexer);
    foreach (const QString &keyword, keywordList) {
        apis->add(keyword);
    }
    apis->prepare();

}

void EditArea::InitTextEdit()
{
    textEdit->setLexer(CppLexer);
    textEdit->setTabWidth(4);///设置Tab长度为4空格

    ///无需设置TextEdit的背景色，直接设置lexer即可，会被lexer覆盖

    textEdit->setIndentationGuides(true);///启动缩进提示
    textEdit->setAutoIndent(true);///启动自动缩进
    // void setCaretForegroundColor(const QColor &col)
    textEdit->setCaretForegroundColor(CursorColor);////设置光标color


    textEdit->setCaretLineVisible(true);//启用当前行高亮
    textEdit->setCaretLineBackgroundColor(LineHighLightColor);///设置当前行高亮颜色

    textEdit->setMarginsBackgroundColor(MarginBackColor);///设置margin边栏的背景color



    textEdit->setMarginType(EditArea::SymMargin1,QsciScintilla::SymbolMargin);///符号栏,后序实现断点等操作
    textEdit->setMarginWidth(EditArea::SymMargin1,20);

    textEdit->setMarginType(EditArea::SymMargin2,QsciScintilla::SymbolMargin);
    textEdit->setMarginWidth(EditArea::SymMargin2,20);

    textEdit->setMarginLineNumbers(EditArea::LineNumMargin,true);
    textEdit->setMarginType(EditArea::LineNumMargin,QsciScintilla::NumberMargin);
    textEdit->setMarginWidth(EditArea::LineNumMargin,"0000");///设置行号栏宽度
    textEdit->setMarginsForegroundColor(Qt::white);///行号字体颜色

    textEdit->setMarginType(EditArea::SymMargin3,QsciScintilla::SymbolMargin);
    textEdit->setMarginWidth(EditArea::SymMargin3,20);

    textEdit->setFolding(QsciScintilla::BoxedTreeFoldStyle,FoldMargin);///折叠代码块栏

    textEdit->setMarginWidth(EditArea::FoldMargin,20);
    textEdit->setFoldMarginColors(FoldingColor,FoldingColor);


    int  LineTag= 0;
    textEdit->markerDefine(QsciScintilla::VerticalLine , LineTag);
    textEdit->setMarkerBackgroundColor(currentLineTagColor, LineTag);


}




void EditArea::ModifyLexerColor(int WordType,QColor &col)
{
    Lexer_Color.value(WordType)=col;
    InitLexer();
}

void EditArea::ResortDefault()
{
    Lexer_Color=createDefaultColorMap();
    InitLexer();
}

void EditArea::CloseLineTag(bool flag)
{
    this->LineTagIsOpen=flag;
}

void EditArea::modifyLineTagColor(QColor &col)
{
    LineHighLightColor=col;
}

void EditArea::modifyFoldingColor(QColor &col)
{
    FoldingColor=col;
}

void EditArea::modifyMarginBackColor(QColor &col)
{
    MarginBackColor=col;
}

void EditArea::modifyLineHighLightColor(QColor &col)
{
    LineHighLightColor=col;
}

void EditArea::modifyCursorColor(QColor &col)
{
    CursorColor=col;
}

void EditArea::modifyFontDefaultColor(QColor &col)
{
    FontDefaultColor=col;
}

void EditArea::modifyEditorDefaultBackgroundColor(QColor &col)
{
    EditorDefaultBackgroundColor=col;
}

void EditArea::highlightCurrentLine(int line,int index)
{
    Q_UNUSED(index);
    qDebug()<<"curLine:"<<line;
    // 清除所有标记
    for (int i = 0; i < textEdit->lines(); ++i) {
        textEdit->markerDelete(i, 0);
        textEdit->markerDelete(i, 1);

    }

    // 在当前行添加标记
    if (line >= 0 && line < textEdit->lines()) {
        textEdit->markerAdd(line, 0);

    }
}

QMap<int, QColor> EditArea::createDefaultColorMap()
{
    QMap<int, QColor> colorMap;

    // 默认文本
    colorMap.insert(QsciLexerCPP::Default, QColor(214, 207, 154));

    // 注释
    colorMap.insert(QsciLexerCPP::Comment, QColor(87, 166, 74));
    colorMap.insert(QsciLexerCPP::CommentLine, QColor(87, 166, 74));
    colorMap.insert(QsciLexerCPP::CommentDoc, QColor(87, 166, 74));
    colorMap.insert(QsciLexerCPP::CommentLineDoc, QColor(87, 166, 74));

    // 数字
    colorMap.insert(QsciLexerCPP::Number, QColor(181, 206, 168));

    // 关键字
    colorMap.insert(QsciLexerCPP::Keyword, QColor(86, 156, 214));
    colorMap.insert(QsciLexerCPP::KeywordSet2, QColor(216, 160, 223)); // 第二组关键字

    // 字符串
    colorMap.insert(QsciLexerCPP::DoubleQuotedString, QColor(214, 157, 133));
    colorMap.insert(QsciLexerCPP::SingleQuotedString, QColor(214, 157, 133));
    colorMap.insert(QsciLexerCPP::UnclosedString, QColor(138, 96, 44));
    colorMap.insert(QsciLexerCPP::VerbatimString, QColor(138, 96, 44));
    colorMap.insert(QsciLexerCPP::TripleQuotedVerbatimString, QColor(214, 157, 133));

    // 预处理器指令
    colorMap.insert(QsciLexerCPP::PreProcessor, QColor(155, 155, 155));

    // 操作符
    colorMap.insert(QsciLexerCPP::Operator, QColor(180, 180, 180));

    // 标识符
    colorMap.insert(QsciLexerCPP::Identifier, QColor(220, 220, 170));

    // 其他特殊样式
    colorMap.insert(QsciLexerCPP::UserLiteral, QColor(78, 201, 176));
    colorMap.insert(QsciLexerCPP::TaskMarker, QColor(193, 44, 31));
    colorMap.insert(QsciLexerCPP::EscapeSequence, QColor(213, 235, 225));
    // colorMap.insert(QsciLexerCPP::EscapeChar, QColor());

    // 不活动区域的样式
    colorMap.insert(QsciLexerCPP::InactiveDefault, QColor(214, 207, 154));
    colorMap.insert(QsciLexerCPP::InactiveComment, QColor(87, 166, 74));
    colorMap.insert(QsciLexerCPP::InactiveCommentLine, QColor(87, 166, 74));
    colorMap.insert(QsciLexerCPP::InactiveNumber, QColor(181, 206, 168));
    colorMap.insert(QsciLexerCPP::InactiveKeyword, QColor(86, 156, 214));
    colorMap.insert(QsciLexerCPP::InactiveDoubleQuotedString, QColor(214, 157, 133));
    colorMap.insert(QsciLexerCPP::InactiveSingleQuotedString, QColor(214, 157, 133));
    colorMap.insert(QsciLexerCPP::InactivePreProcessor, QColor(155, 155, 155));
    colorMap.insert(QsciLexerCPP::InactiveOperator, QColor(180, 180, 180));
    colorMap.insert(QsciLexerCPP::InactiveIdentifier, QColor(218, 218, 218));
    colorMap.insert(QsciLexerCPP::InactiveUnclosedString, QColor(218, 218, 218));
    colorMap.insert(QsciLexerCPP::InactiveRawString, QColor(218, 218, 218));

    return colorMap;
}



