#include "gtapi.h"

#include <QtGui>
#include <QtWidgets>


class TCPosDictWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit TCPosDictWidget(QWidget *parent = 0);

signals:

public slots:
    void setPosDictionary(const QList<GTPosDict>& posDicts);

};

class GTTestWidget: public QWidget{

    Q_OBJECT

    QLineEdit *lneInput;
    QTextEdit *txeOutput;
    TCPosDictWidget *posDictWgt;
    GTApi     *gtApi;

public:
    GTTestWidget(QWidget* parent=0) : QWidget(parent){
        lneInput = new QLineEdit(this);
        txeOutput = new QTextEdit(this);
        gtApi = new GTApi(this);

        posDictWgt = new TCPosDictWidget(this);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(lneInput);
        layout->addWidget(txeOutput);
        layout->addWidget(posDictWgt);

        connect(lneInput, SIGNAL(returnPressed()), SLOT(onInputReturnPressed()));
        connect(gtApi, SIGNAL(translationReady(GTApiTranslation)), SLOT(onTranslationReady(GTApiTranslation)));
    }

public slots:
    void onInputReturnPressed(){
        gtApi->translate(lneInput->text(), "ru");
    }

    void onTranslationReady(const GTApiTranslation& gtApiTr) {
        txeOutput->setText( gtApiTr.translation().join("\n") );
        posDictWgt->setPosDictionary(gtApiTr.getPosDictionary());
    }
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GTTestWidget wgt;
    wgt.show();
    return app.exec();
}

struct TCPosDictEntryItem : public QTreeWidgetItem{
    TCPosDictEntryItem(const GTPosDictEntry& entry){

        QString trans = entry.wordTarget;
        if ( trans.size() > 12 ) {
            trans.insert(trans.size()/2, "-\n");
        }

        setText(0, trans);
        setText(1, entry.wordRetranslations.join(", "));

        setTextColor(1, QColor("#777"));
    }
};

struct TCPosDictItem : public QTreeWidgetItem{

    TCPosDictItem(const GTPosDict& dict){

        int index = 0;
        QStringList entriesWord;
        foreach (const GTPosDictEntry & entry, dict.entries) {
            TCPosDictEntryItem *entrieItem = new TCPosDictEntryItem(entry);
            insertChild(index++, entrieItem);
            entriesWord << entry.wordTarget;
        }
        setText(0, dict.posNameHl);
        setText(1, entriesWord.join(", "));

        QFont font;
        font.setItalic(true);
        setFont(0, font);
        setTextColor(0, QColor("#777"));
    }
};



TCPosDictWidget::TCPosDictWidget(QWidget *parent) :
    QTreeWidget(parent)
{

    setColumnCount(2);
    setHeaderHidden(true);
    setAnimated(true);
    setIndentation(15);
    setRootIsDecorated(false);

    setWordWrap(true);
    //setUniformRowHeights(true);

    header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void TCPosDictWidget::setPosDictionary(const QList<GTPosDict>& posDicts)
{
    QTreeWidget::clear();
    int index =0;

    foreach ( const GTPosDict& dict, posDicts ) {
        TCPosDictItem *posDictItem = new TCPosDictItem(dict);
        insertTopLevelItem(index++, posDictItem);
    }
}

struct TCDefDictItem : public QTreeWidgetItem{

    TCDefDictItem(const GTDefDict& dict)   {

        int index = 0;
        //QStringList entriesWord;
        foreach (const GTDefDictEntry & entry, dict.entries) {
            //TCDefDictEntryItem *entrieItem = new TCDefDictEntryItem(entry);
            ////insertChild(index++, entrieItem);
            //entriesWord << entry.wordTarget;
        }
        setText(0, dict.posName);
        //setText(1, entriesWord.join(", "));

        QFont font;
        font.setItalic(true);
        setFont(0, font);
        setTextColor(0, QColor("#777"));
    }
};


class TCDefDictWidget : public QTreeWidget{

    Q_OBJECT

public:
    explicit TCDefDictWidget(QWidget *parent = 0){
        setColumnCount(2);
        setHeaderHidden(true);
        setAnimated(true);
        setIndentation(15);
        setRootIsDecorated(false);

        setWordWrap(true);
        //setUniformRowHeights(true);

        header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        header()->setSectionResizeMode(1, QHeaderView::Stretch);
    }

signals:

public slots:
    void setDefDictionary(const QList<GTDefDict>& defDicts){
        clean();
        int index = 0;
        foreach ( const GTDefDict& dict, defDicts ) {
            TCDefDictItem *defDictItem = new TCDefDictItem(dict);
            insertTopLevelItem(index++, defDictItem);
        }
    }

};
