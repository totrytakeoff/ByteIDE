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
#include <QSettings>

EditArea::EditArea(QWidget* parent)
    :QWidget(parent)
{

    m_Theme=settings.value("UI/Theme","dark").toString();

    textEdit=new QsciScintilla(this);
    CppLexer=new QsciLexerCPP(this);
    PythonLexer=new QsciLexerPython(this);
    DefaultLexer=new QsciLexerD(this);

    ///设置主题
    if(m_Theme=="dark"){
        ColorsList=darkColorList;
        Lexer_Color=lightColorMap;
    }else{
        ColorsList=lightColorList;
        Lexer_Color=lightColorMap;
    }


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


    // textEdit->SendScintilla(QsciScintillaBase::SCI_CLEARCMDKEY, (QsciScintillaBase::SCMOD_CTRL << 16) | ('/' & 0xFF));

}

EditArea::~EditArea()
{

}

void EditArea::InitLexer()
{


/******************************set cppLexer*********************************************/
    CppLexer->setDefaultColor(ColorsList[ColorType::FontDefaultColor]);///默认字体颜色
    CppLexer->setDefaultPaper(ColorsList[ColorType::EditorDefaultBackgroundColor]);///默认背景色
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

    PythonLexer->setDefaultColor(ColorsList[FontDefaultColor]);///默认字体颜色
    PythonLexer->setDefaultPaper(ColorsList[EditorDefaultBackgroundColor]);///默认背景色
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
    DefaultLexer->setDefaultPaper(ColorsList[EditorDefaultBackgroundColor]);///默认背景色
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

    textEdit->setTabWidth(4);///设置Tab长度为4空格
    textEdit->setBraceMatching(QsciScintilla::SloppyBraceMatch);///启用括号匹配
    textEdit->setMatchedBraceBackgroundColor(ColorsList[MatchedBraceBackgroundColor]);
    textEdit->setMatchedBraceForegroundColor(ColorsList[MatchedBraceForegroundColor]);

/////////////////
    textEdit->setCallTipsStyle(QsciScintilla::CallTipsNoContext);///调用提示
    textEdit->setCallTipsPosition(QsciScintilla::CallTipsBelowText);
    textEdit->setCallTipsVisible(0);

    textEdit->setCallTipsBackgroundColor(ColorsList[CallTipBackgroundColor]);
    textEdit->setCallTipsForegroundColor(ColorsList[CallTipForegroundColor]);
    textEdit->setCallTipsHighlightColor(ColorsList[CallTipHighlightColor]);


    ///自动补全,tab enter 确认选择，esc取消AutoCompletion
    textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
    textEdit->setAutoCompletionThreshold(2); // 当用户输入至少一个字符后触发自动完成
    textEdit->setAutoCompletionReplaceWord(true); // 自动替换当前词
    textEdit->setAutoCompletionCaseSensitivity(false); // 不区分大小写
    textEdit->setAutoCompletionShowSingle(true); //只有一个匹配项也显示补全提示

////////////////

    ///无需设置TextEdit的背景色，直接设置lexer即可，会被lexer覆盖
    textEdit->setPaper(ColorsList[EditorDefaultBackgroundColor]);


    textEdit->setIndentationGuides(true);///启动缩进提示
    textEdit->setAutoIndent(true);///启动自动缩进

    textEdit->setCaretForegroundColor(ColorsList[CursorColor]);////设置光标color

    textEdit->setCaretLineVisible(true);//启用当前行高亮
    textEdit->setCaretLineBackgroundColor(ColorsList[LineHighLightColor]);///设置当前行高亮颜色

    textEdit->setMarginsBackgroundColor(ColorsList[MarginBackColor]);///设置margin边栏的背景color



    textEdit->setMarginType(EditArea::SymMargin1,QsciScintilla::SymbolMargin);///符号栏,后序实现断点等操作
    textEdit->setMarginWidth(EditArea::SymMargin1,20);

    textEdit->setMarginType(EditArea::SymMargin2,QsciScintilla::SymbolMargin);//设置符号栏,后序实现断点等标识
    textEdit->setMarginWidth(EditArea::SymMargin2,20);

    textEdit->setMarginLineNumbers(EditArea::LineNumMargin,true);///设置行号栏
    textEdit->setMarginType(EditArea::LineNumMargin,QsciScintilla::NumberMargin);
    textEdit->setMarginWidth(EditArea::LineNumMargin,"0000");///设置行号栏宽度
    textEdit->setMarginsForegroundColor(ColorsList[MarginsForeground]);///行号字体颜色

    textEdit->setMarginType(EditArea::SymMargin3,QsciScintilla::SymbolMargin);///设置符号栏,后序实现断点等标识
    textEdit->setMarginWidth(EditArea::SymMargin3,20);

    textEdit->setFolding(QsciScintilla::BoxedTreeFoldStyle,FoldMargin);///折叠代码块栏
    textEdit->setMarginWidth(EditArea::FoldMargin,20);
    textEdit->setFoldMarginColors(ColorsList[FoldingColor],ColorsList[FoldingColor]);///fold栏backgroundColor


    int  LineTag= 0;
    textEdit->markerDefine(QsciScintilla::VerticalLine , LineTag);///当前行标识
    textEdit->setMarkerBackgroundColor(ColorsList[LineTagColor], LineTag);



    ///符号自动补全
    QStringList customSeparators = {" ", "(", ")", ",", ";", ".", ":", "[", "]", "{", "}","->","::"};
    textEdit->setAutoCompletionWordSeparators(customSeparators);



    // textEdit->setStyleSheet(StyleSheet); //使用全局样式

}




void EditArea::ModifyLexerColor(int WordType,QColor &col)
{
    Lexer_Color.value(WordType)=col;
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
        commentFlag="#";
        textEdit->setLexer(PythonLexer);
    }else if(type=="cpp"||type=="c"){
        qDebug()<<"change to cpplexer";
        commentFlag="//";
        textEdit->setLexer(CppLexer);
    }else{
        ///更改默认高亮为Python高亮
        textEdit->setLexer(PythonLexer);
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
    ColorsList[LineTagColor]=col;
}

void EditArea::modifyFoldingColor(QColor &col)
{
    ColorsList[FoldingColor]=col;
}

void EditArea::modifyMarginBackColor(QColor &col)
{
    ColorsList[MarginBackColor]=col;
}

void EditArea::modifyLineHighLightColor(QColor &col)
{
    ColorsList[LineHighLightColor]=col;
}

void EditArea::modifyCursorColor(QColor &col)
{
    ColorsList[CursorColor]=col;
}

void EditArea::modifyFontDefaultColor(QColor &col)
{
    ColorsList[FontDefaultColor]=col;
}

void EditArea::modifyEditorDefaultBackgroundColor(QColor &col)
{
    ColorsList[EditorDefaultBackgroundColor]=col;
}



void EditArea::setTheme(QString theme)
{
    if(theme==m_Theme){
        return ;
    }

    m_Theme=theme;
    ///设置主题
    if(m_Theme=="dark"){
        ColorsList=darkColorList;
        Lexer_Color=darkColorMap;
    }else{
        ColorsList=lightColorList;
        Lexer_Color=lightColorMap;
    }

    InitLexer();
    InitTextEdit();

}



void EditArea::setCommentline()
{

    qDebug() << "commentLine";

    int startLine, endLine;
    int dummy; // 占位参数

    // 安全获取选中行号（无需动态分配）
    if (textEdit->hasSelectedText()) {
        textEdit->getSelection(&startLine, &dummy, &endLine, &dummy);
    } else {
        textEdit->getCursorPosition(&startLine, &dummy);
        endLine = startLine;
    }

    // 遍历所有选中行（包含最后一行）
    for (int i = startLine; i <= endLine; ++i) {
        // 获取完整行文本（保留首尾空格）
        QString text = textEdit->text(i);

        // 判断是否已注释
        QString temp=text;
        qDebug()<<temp;
        temp=temp.trimmed();
        if (temp.isEmpty()) continue; // 跳过空行


        bool isCommented = temp.startsWith(commentFlag);

        // 获取行首位置（确保多字节字符正确处理）
        int pos = textEdit->positionFromLineIndex(i, 0);

        if (isCommented) {
            int comment=text.indexOf(commentFlag);
            // textEdit->text().remove(pos+comment,commentFlag.length());
            // 移除注释符号（直接替换行首的commentFlag）
            textEdit->setSelection(i, comment, i, comment+commentFlag.length());
            textEdit->removeSelectedText();
        } else {
            // 插入注释符号（使用SCI_INSERTTEXT确保编码正确）
            textEdit->SendScintilla(QsciScintilla::SCI_INSERTTEXT, pos, commentFlag.toUtf8().constData());
        }
    }
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
{   qDebug()<<"finding "<<forward;

    // 设置搜索参数
    bool found = textEdit->findFirst(text, false, caseSensitive, wholeWords, true, forward, -1, -1, true, false, false);

    ///因为find后一项后，cursor位于匹配项的end，故再find前一项会匹配到当前项，则需利用findNext再往前推一项
    if(!forward)
        textEdit->findNext();


    qDebug()<<"found1:"<<found;
    if (!found) {
        // 如果没有找到，可以从文档开头重新开始搜索
        textEdit->setCursorPosition(0,0);
        found = textEdit->findFirst(text, false, caseSensitive, wholeWords, true, true, -1, -1, true, false, false);
        qDebug()<<"found2:"<<found;
    }

    if (!found) {
        QMessageBox::information(this, "查找", "未找到指定文本");
    }


}

void EditArea::selectAllText(const QString &text)
{
    qDebug()<<"selectAll "<<text;
    textEdit->clearIndicatorRange(0, 0, textEdit->lines(), 0, highlightIndicator);
    if(text.isEmpty())
        return;


    int startPos=0;
    int endPos=textEdit->length();


    if(textEdit->hasSelectedText()&&textEdit->selectedText()!=text){
        startPos=textEdit->SendScintilla(QsciScintilla::SCI_GETSELECTIONSTART);
        endPos=textEdit->SendScintilla(QsciScintilla::SCI_GETSELECTIONEND);
    }

    // 清除现有选区
    textEdit->SendScintilla(QsciScintilla::SCI_CLEARSELECTIONS);

    // 设置多选区模式
    textEdit->SendScintilla(QsciScintilla::SCI_SETMULTIPLESELECTION, 1);
    textEdit->SendScintilla(QsciScintilla::SCI_SETADDITIONALSELECTIONTYPING, 1);

    // 设置选择样式，透明度和Color
    textEdit->SendScintilla(QsciScintilla::SCI_SETSELALPHA, 75); // 半透明效果
    // textEdit->SendScintilla(QsciScintilla::SCI_SETSELBACK, true, QColor(0, 0, 255).rgb());


    ///设置选区
    textEdit->SendScintilla(QsciScintilla::SCI_SETSELECTION, matchPosVec[0].first, matchPosVec[0].second);
    for(int i=1;i<matchPosVec.size();i++){
        ///限定多选范围
        if(matchPosVec[i].first>=startPos&&matchPosVec[i].second<=endPos){
            textEdit->SendScintilla(QsciScintilla::SCI_ADDSELECTION,  matchPosVec[i].first, matchPosVec[i].second);
        }
    }


}




void EditArea::highLightAll(const QString &text)
{
    // return;
    // 清除所有旧的高亮
    textEdit->clearIndicatorRange(0, 0, textEdit->lines(), 0, highlightIndicator);

    //清空匹配cursor Start to End map
    matchPosVec.clear();

    if(text.isEmpty())
        return;

    // 设置指示器样式
    textEdit->SendScintilla(QsciScintilla::SCI_INDICSETSTYLE, highlightIndicator, QsciScintilla::INDIC_FULLBOX); // 可选择其他样式如 INDIC_PLAIN, INDIC_SQUIGGLE 等
    textEdit->SendScintilla(QsciScintilla::SCI_INDICSETFORE, highlightIndicator, QColor(255, 0, 0)); // 设置前景色（可选）


    int startPos = 0;
    int endPos = textEdit->length();
    bool found=true;
    int line1, index1,line2,index2;
    qDebug()<<"start:"<<startPos<<" endPos:"<<endPos;

     while (found){
        ///注意该处匹配需要wrap(回转到开头匹配),不然会从cursor匹配,导致前面的匹配项丢失
        found = textEdit->findFirst(text, false, false, false, true, true);
        if (found) {

            ///获取选择的初始和结束位置
            const int matchStart = textEdit->SendScintilla(QsciScintilla::SCI_GETSELECTIONSTART);
            const int matchEnd = textEdit->SendScintilla(QsciScintilla::SCI_GETSELECTIONEND);

            ///将位置转化为 Line 和 Index
            textEdit->lineIndexFromPosition(matchStart,&line1,&index1);
            textEdit->lineIndexFromPosition(matchEnd,&line2,&index2);

            qDebug()<<"startPos:"<<startPos<<" matchStart:"<<matchStart<<" matchEnd: "<<matchEnd<<"line index::"<<line1<<" "
                     <<index1<<" "<<line2<<" "<<index2;
            /// 高亮匹配区域
            textEdit->fillIndicatorRange(line1,index1, line2,index2, highlightIndicator);

            ///存储匹配项的起始位置
            matchPosVec.push_back(qMakePair(matchStart,matchEnd));


            /// 防越wrap死循环
            if (startPos > matchStart) break;

            // 更新起始位置到当前匹配结束位置
            startPos = matchEnd;

        }
    }
}




void EditArea::replaceText(const QString &origin,const QString &replaced)
{   qDebug()<<"replace text :"<<replaced ;
    if (textEdit->hasSelectedText()) {
        textEdit->replaceSelectedText(replaced);
        textEdit->findNext(); // 自动查找下一个匹配项
    }
}

void EditArea::replaceAll(const QString &origin, const QString &replaced)
{   bool found=true;
    while(textEdit->hasSelectedText()&&found) {
        textEdit->replaceSelectedText(replaced);
        found=textEdit->findNext(); // 自动查找下一个匹配项
    }

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




