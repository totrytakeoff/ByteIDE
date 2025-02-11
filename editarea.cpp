#include "editarea.h"
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerd.h>
#include <Qsci/qscistyledtext.h>
#include <Qsci/qsciapis.h>
#include <QTabWidget>
#include <QDebug>
#include <QString>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QTextCursor>
#include <QDir>
#include <QMessageBox>
#include <QScrollBar>

EditArea::EditArea(QWidget* parent)
    :QWidget(parent)
{


    textEdit=new QsciScintilla(this);
    CppLexer=new QsciLexerCPP(this);
    PythonLexer=new QsciLexerPython(this);
    DefaultLexer=new QsciLexerD(this);
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

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);
    layout->setContentsMargins(0, 0, 0, 0); // 可选：移除布局的边距
    setLayout(layout);

    // 安装事件过滤器,模拟keyEvent
    textEdit->installEventFilter(this);
}

EditArea::~EditArea()
{

}

void EditArea::InitLexer()
{

/******************************set cppLexer*********************************************/
    CppLexer->setDefaultColor(FontDefaultColor);///默认字体颜色
    CppLexer->setDefaultPaper(EditorDefaultBackgroundColor);///默认背景色
    CppLexer->setFont(QFont("Consolas",14));///默认字体

    CppLexer->setFoldAtElse(true);       // 在 else 处折叠代码块
    CppLexer->setFoldCompact(true);      // 折叠时隐藏空行



    //.................................................................


    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Default),QsciLexerCPP::Default);

    // 设置注释颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Comment), QsciLexerCPP::Comment);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::CommentLine), QsciLexerCPP::CommentLine);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::CommentDoc), QsciLexerCPP::CommentDoc);
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::CommentLineDoc), QsciLexerCPP::CommentLineDoc);

    // 设置数字颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Number), QsciLexerCPP::Number);

    // 设置关键字颜色
    CppLexer->setColor(Lexer_Color.value(QsciLexerCPP::Keyword), QsciLexerCPP::Keyword);//设置关键字高亮颜色
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



    QsciAPIs *cppAPIs = new QsciAPIs(CppLexer);
    if(cppAPIs->load("./apis/cpp.api")){
        qDebug()<<"cpp api is ok";
        cppAPIs->prepare();
    }


/******************************set pythonLexer*********************************************/

    PythonLexer->setDefaultColor(FontDefaultColor);///默认字体颜色
    PythonLexer->setDefaultPaper(EditorDefaultBackgroundColor);///默认背景色
    PythonLexer->setFont(QFont("Consolas",14));///默认字体

    PythonLexer->setFoldCompact(true);      // 折叠时隐藏空行


    //.................................................................


    PythonLexer->setColor(Lexer_Color.value(QsciLexerCPP::Default),QsciLexerPython::Default);//设置关键字高亮颜色
    // 设置注释颜色
    PythonLexer->setColor(Lexer_Color.value(QsciLexerCPP::Comment), QsciLexerPython::Comment);


    // 设置数字颜色
    PythonLexer->setColor(Lexer_Color.value(QsciLexerCPP::Number), QsciLexerPython::Number);

    // 设置关键字颜色
    PythonLexer->setColor(Lexer_Color.value(QsciLexerCPP::Keyword), QsciLexerPython::Keyword);

    // 设置字符串颜色
    PythonLexer->setColor(Lexer_Color.value(QsciLexerCPP::DoubleQuotedString), QsciLexerPython::DoubleQuotedString);
    PythonLexer->setColor(Lexer_Color.value(QsciLexerCPP::SingleQuotedString), QsciLexerPython::SingleQuotedString);
    PythonLexer->setColor(Lexer_Color.value(QsciLexerCPP::UnclosedString), QsciLexerPython::UnclosedString);

    // 设置操作符颜色
    PythonLexer->setColor(Lexer_Color.value(QsciLexerCPP::Operator), QsciLexerPython::Operator);

    // 设置标识符颜色
    PythonLexer->setColor(Lexer_Color.value(QsciLexerCPP::Identifier), QsciLexerPython::Identifier);

    // ///自动补全的实现
    QsciAPIs *pyAPIs = new QsciAPIs(PythonLexer);
    if(pyAPIs->load("./apis/Python-3.11.api")){
        qDebug()<<"py api is ok";
        pyAPIs->prepare();
    }

    ///添加语言api文件解析keyword实现自动补全

    /******************************set defaultLexer*********************************************/

    DefaultLexer->setDefaultColor(DefaultTextColor);///默认字体颜色
    DefaultLexer->setDefaultPaper(EditorDefaultBackgroundColor);///默认背景色
    DefaultLexer->setFont(QFont("Consolas",14));///默认字体

    // 设置默认 Lexer
    // 设置默认文本颜色为白色
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::Default);

    // 设置注释颜色
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::Comment);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::CommentLine);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::CommentDoc);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::CommentNested);

    // 设置数字颜色
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::Number);

    // 设置关键字颜色
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::Keyword);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::KeywordSecondary);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::KeywordDoc);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::Typedefs);

    // 设置字符串颜色
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::String);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::UnclosedString);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::Character);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::BackquoteString);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::RawString);

    // 设置操作符颜色
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::Operator);

    // 设置标识符颜色
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::Identifier);

    // 设置文档注释颜色
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::CommentLineDoc);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::CommentDocKeyword);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::CommentDocKeywordError);

    // 设置其他关键字颜色
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::KeywordSet5);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::KeywordSet6);
    DefaultLexer->setColor(DefaultTextColor, QsciLexerD::KeywordSet7);





}

void EditArea::InitTextEdit()
{

    // textEdit->setLexer(CppLexer);

    textEdit->setTabWidth(4);///设置Tab长度为4空格
    textEdit->setBraceMatching(QsciScintilla::SloppyBraceMatch);///启用括号匹配
    textEdit->setMatchedBraceBackgroundColor(MatchedBraceBackgroundColor);
    textEdit->setMatchedBraceForegroundColor(MatchedBraceForegroundColor);

/////////////////
    textEdit->setCallTipsStyle(QsciScintilla::CallTipsNoContext);///调用提示
    textEdit->setCallTipsPosition(QsciScintilla::CallTipsBelowText);
    textEdit->setCallTipsVisible(0);

    textEdit->setCallTipsBackgroundColor(QColor(30,30,30));
    textEdit->setCallTipsForegroundColor(QColor(210,210,210));
    textEdit->setCallTipsHighlightColor(QColor(255, 71, 87));


    ///自动补全,tab enter 确认选择，esc取消AutoCompletion
    textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
    textEdit->setAutoCompletionThreshold(2); // 当用户输入至少一个字符后触发自动完成
    textEdit->setAutoCompletionReplaceWord(true); // 自动替换当前词
    textEdit->setAutoCompletionCaseSensitivity(false); // 不区分大小写
    textEdit->setAutoCompletionShowSingle(true); //只有一个匹配项也显示补全提示

////////////////

    ///无需设置TextEdit的背景色，直接设置lexer即可，会被lexer覆盖
    textEdit->setPaper(QColor(30,30,30));


    textEdit->setIndentationGuides(true);///启动缩进提示
    textEdit->setAutoIndent(true);///启动自动缩进

    textEdit->setCaretForegroundColor(CursorColor);////设置光标color

    textEdit->setCaretLineVisible(true);//启用当前行高亮
    textEdit->setCaretLineBackgroundColor(LineHighLightColor);///设置当前行高亮颜色

    textEdit->setMarginsBackgroundColor(MarginBackColor);///设置margin边栏的背景color



    textEdit->setMarginType(EditArea::SymMargin1,QsciScintilla::SymbolMargin);///符号栏,后序实现断点等操作
    textEdit->setMarginWidth(EditArea::SymMargin1,20);

    textEdit->setMarginType(EditArea::SymMargin2,QsciScintilla::SymbolMargin);//设置符号栏,后序实现断点等标识
    textEdit->setMarginWidth(EditArea::SymMargin2,20);

    textEdit->setMarginLineNumbers(EditArea::LineNumMargin,true);///设置行号栏
    textEdit->setMarginType(EditArea::LineNumMargin,QsciScintilla::NumberMargin);
    textEdit->setMarginWidth(EditArea::LineNumMargin,"0000");///设置行号栏宽度
    textEdit->setMarginsForegroundColor(Qt::white);///行号字体颜色

    textEdit->setMarginType(EditArea::SymMargin3,QsciScintilla::SymbolMargin);///设置符号栏,后序实现断点等标识
    textEdit->setMarginWidth(EditArea::SymMargin3,20);

    textEdit->setFolding(QsciScintilla::BoxedTreeFoldStyle,FoldMargin);///折叠代码块栏
    textEdit->setMarginWidth(EditArea::FoldMargin,20);
    textEdit->setFoldMarginColors(FoldingColor,FoldingColor);///fold栏backgroundColor


    int  LineTag= 0;
    textEdit->markerDefine(QsciScintilla::VerticalLine , LineTag);///当前行标识
    textEdit->setMarkerBackgroundColor(currentLineTagColor, LineTag);



    ///符号自动补全
    QStringList customSeparators = {" ", "(", ")", ",", ";", ".", ":", "[", "]", "{", "}","->","::"};
    textEdit->setAutoCompletionWordSeparators(customSeparators);


    QString StyleSheet=R"(


.QsciScintilla QScrollBar:horizontal {
    border-top:1px solid gray;
    background-color: rgb(45,45,45);
    height: 12px;
    margin: 0px 0px 0px 0px;
    padding-left:12px;
    padding-right:12px;
    border-radius: 7px;

}




/* 定义 QScintilla 内部垂直滚动条的样式 */
.QsciScintilla QScrollBar:vertical {
    border-left:1px solid gray;
    background-color: rgb(45,45,45);
    width: 12px;
    margin: 0px 0px 0px 0px;
    padding-top:12px;
    padding-bottom:12px;
    border-radius: 7px;
}


QScrollBar::handle:vertical:hover{
    background-color:rgb(30,30,30);
    border-left:1px solid gray;
}
QScrollBar::handle:horizontal:hover{
    background-color:rgb(30,30,30);
    border-left:1px solid gray;
}

QScrollBar::handle:horizontal{
  background-color:rgb(80,80,80);
  border-radius:2px;
  min-width:20px;
  margin:2px 1px 2px 1px;
}

QScrollBar::handle:vertical{
  background-color:rgb(80,80,80);
  border-radius:2px;
  min-width:20px;
  margin:1px 2px 1px 2px;
}

/* 移除滚动区域 */
.QsciScintilla QScrollBar::add-page:horizontal, .QsciScintilla QScrollBar::sub-page:horizontal {
    background: none; /* 设置背景为无 */
}


/* 移除滚动区域 */
.QsciScintilla QScrollBar::add-page:vertical, .QsciScintilla QScrollBar::sub-page:vertical {
    background: none; /* 设置背景为无 */
}

    )";



    textEdit->setStyleSheet(StyleSheet);

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
    if(flag){
        connect(textEdit, &QsciScintilla::cursorPositionChanged, this, &EditArea::highlightCurrentLine);
    }else{
        disconnect(textEdit, &QsciScintilla::cursorPositionChanged, this, &EditArea::highlightCurrentLine);
    }

}

void EditArea::setCurLexer(QString &type)
{
    qDebug()<<"type:"<<type;
    if(type=="py"){
        qDebug()<<"change to pylexer";
        textEdit->setLexer(PythonLexer);
    }else if(type=="cpp"){
        qDebug()<<"change to cpplexer";
        textEdit->setLexer(CppLexer);
    }else{

        textEdit->setLexer(DefaultLexer);
    }

    ///修改lexer后textEdit一些属性会被清空,故需要重新设置
    InitTextEdit();
    // textEdit->setMatchedBraceBackgroundColor(MatchedBraceBackgroundColor);
    // textEdit->setMatchedBraceForegroundColor(MatchedBraceForegroundColor);
    // textEdit->setMarkerBackgroundColor(currentLineTagColor, 0);
    // textEdit->setMarginsBackgroundColor(MarginBackColor);///设置margin边栏的背景color
    // textEdit->setMarginsForegroundColor(Qt::white);///行号字体颜色

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

void EditArea::findNext(const QString &text, bool caseSensitive, bool wholeWords,bool forward)
{   qDebug()<<"finding";
    // 设置搜索参数
    bool found = textEdit->findFirst(text, false, caseSensitive, wholeWords, true, forward, -1, -1, true, false, false);

    if (!found) {
        // 如果没有找到，可以从文档开头重新开始搜索
        textEdit->setCursorPosition(0,0);
        found = textEdit->findFirst(text, false, caseSensitive, wholeWords, true, forward, -1, -1, true, false, false);
    }

    if (!found) {
        QMessageBox::information(this, "查找", "未找到指定文本");
    }


}

void EditArea::selectAllText(const QString &text)
{
    qDebug()<<"selectAll";
}

void EditArea::highLightAll(const QString &text)
{
    // 清除之前的所有指示器
    textEdit->clearIndicatorRange(0, 0, textEdit->lines(), 0, highlightIndicator);

    int line = 0;
    int index = 0;
    while (textEdit->findFirst(text, false, true, false, true, true, line, index)) {
        int startPos = textEdit->positionFromLineIndex(line, index);
        int endPos = startPos + text.length();
        textEdit->fillIndicatorRange(line, index, line, index + text.length(), highlightIndicator);
        // 更新索引和行号以便继续查找下一个匹配项
        textEdit->lineIndexFromPosition(endPos,&line,&index);
        // textEdit
    }
}

void EditArea::replaceText(QString &origin, QString &replaced)
{
    if (textEdit->hasSelectedText()) {
        textEdit->replaceSelectedText(replaced);
        // findNext(origin,); // 自动查找下一个匹配项
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

bool EditArea::eventFilter(QObject *obj, QEvent *event)
{

    if (obj == textEdit && event->type() == QEvent::KeyPress) {

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // 获取当前光标位置
        int line, index;
        textEdit->getCursorPosition(&line, &index);

        // 获取输入的字符
        QString text = keyEvent->text();


        // 检查是否输入了左括号
        if (text == "(" || text == "{" || text == "["||text=="\""||text=="'") {
            // 阻止默认行为（插入左括号）防止插入右括号覆盖自动插入的左括号
            event->accept();

            // 插入对应的右括号
            QString closingBracket;
            if (text == "(") closingBracket = ")";
            else if (text == "{") closingBracket = "}";
            else if (text == "[") closingBracket = "]";
            else if (text == "\"") closingBracket = "\"";
            else if (text == "'") closingBracket = "'";

            textEdit->insertAt(text,line,index);//手动插入左括号
            textEdit->insertAt(closingBracket, line, index + 1);//手动插入右括号

            // 将光标移动到括号内
            textEdit->setCursorPosition(line, index + 1);

            return true;  // 事件已处理
        }
    }

    // 调用基类的事件过滤器
    return QWidget::eventFilter(obj, event);

}



