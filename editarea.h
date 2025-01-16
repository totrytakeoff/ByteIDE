#ifndef EDITAREA_H
#define EDITAREA_H
#include<QMap>
#include<QWidget>
#include<QColor>

class QsciScintilla;
class QsciLexerCPP;
class QTabWidget;
class QColor;
// class QMap;

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

    void modifyLineTagColor(QColor &col);
    void modifyFoldingColor(QColor &col);
    void modifyMarginBackColor(QColor &col);
    void modifyLineHighLightColor(QColor &col);
    void modifyCursorColor(QColor &col);


    void modifyFontDefaultColor(QColor &col);
    void modifyEditorDefaultBackgroundColor(QColor &col);


    QMap<int,QColor> createDefaultColorMap();






    QsciScintilla *textEdit;
    QsciLexerCPP *CppLexer;
    QTabWidget* tabwidget;
    QString curEditFile;

public slots:
    void highlightCurrentLine(int line,int index);

signals:
    void test(int a,int b);


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

    QColor FontDefaultColor=QColor(214, 207, 154);
    QColor EditorDefaultBackgroundColor= QColor(35, 35, 35);


};

#endif // EDITAREA_H
