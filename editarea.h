#ifndef EDITAREA_H
#define EDITAREA_H
#include <QMap>
#include <QWidget>


class QsciScintilla;
class QsciLexerCPP;
class QTabWidget;
class QColor;
class QsciLexerPython;
class QsciLexerD;

// class QMap;
///world

class EditArea : public QWidget
{
    Q_OBJECT
public:
    EditArea(QWidget *parent=nullptr);
    ~EditArea();

    void InitLexer();
    void InitTextEdit();
    void ModifyLexerColor(int WordType,QColor &col);
    void ResortDefault();
    void CloseLineTag(bool flag);

    void setCurLexer(QString &type);

    void modifyLineTagColor(QColor &col);
    void modifyFoldingColor(QColor &col);
    void modifyMarginBackColor(QColor &col);
    void modifyLineHighLightColor(QColor &col);
    void modifyCursorColor(QColor &col);


    void modifyFontDefaultColor(QColor &col);
    void modifyEditorDefaultBackgroundColor(QColor &col);


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
    void highlightCurrentLine(int line,int index);

    void findNext(const QString &text, bool caseSensitive, bool wholeWords,bool forward=true);

    void selectAllText(const QString &text);

    void highLightAll(const QString &text);

    void replaceText(const QString &origin,const QString &replaced);

    void replaceAll(const QString &origin,const QString &replaced);
signals:


private:
    QMap<int,QColor> createDefaultColorMap();

private:
    enum{
        SymMargin1=0,
        SymMargin2=1,
        SymMargin3=3,
        LineNumMargin=2,
        FoldMargin=4
    };

    bool LineTagIsOpen=true;
    QMap<int,QColor> Lexer_Color;
    QColor currentLineTagColor=QColor(241, 196, 15);
    QColor FoldingColor=QColor(56, 57, 59);
    QColor MarginBackColor=QColor(30, 30, 30);
    QColor LineHighLightColor=QColor(46, 47, 48);
    QColor CursorColor=Qt::white;
    QColor DefaultTextColor=QColor(220,220,220);

    QColor FontDefaultColor=QColor(214, 207, 154);
    QColor EditorDefaultBackgroundColor= QColor(35, 35, 35);

    QColor MatchedBraceBackgroundColor=QColor(255, 107, 129);
    QColor MatchedBraceForegroundColor=QColor(241, 196, 15);

    int m_flags;
    int m_lastFindPos;
    int highlightIndicator = 7; // 需要初始化指示器ID

    QVector<QPair<int,int>> matchPosVec;

};

#endif // EDITAREA_H
