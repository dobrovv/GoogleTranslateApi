#include "gtapi.h"

#include <QtGui>
#include <QtWidgets>

////////////////////////////////////////////////
// Widget that tests if everything still works (:

class GTTestWidget: public QWidget {

    Q_OBJECT

    GTApi     *gtApi;

    QLineEdit     *lneInput;
    QLineEdit     *lneSrcLang;

    QTextEdit     *apiView;
    QTextEdit     *apiComment;

public:
     GTTestWidget(QWidget* parent=0) : QWidget(parent) {

        lneInput = new QLineEdit(this);
        connect(lneInput, SIGNAL(returnPressed()), SLOT(onInputReturnPressed()));

        gtApi = new GTApi(this);
        connect(gtApi, SIGNAL(translationReady(GTApiTranslation)), SLOT(onTranslationReady(GTApiTranslation)));

        lneSrcLang = new QLineEdit(this);
        lneSrcLang->setText("fr");
        lneSrcLang->setInputMask("AA");
        lneSrcLang->setFixedSize(lneSrcLang->minimumSizeHint());

        apiView = new QTextEdit;
        apiView->setTabStopWidth(15);
        apiView->setWordWrapMode(QTextOption::NoWrap);

        apiComment = new QTextEdit;
        apiComment->setWordWrapMode(QTextOption::NoWrap);

        QGridLayout *layout = new QGridLayout(this);
        layout->addWidget(lneInput,   0, 0);
        layout->addWidget(lneSrcLang, 0, 1);
        layout->addWidget(apiView,    1, 0);
        layout->addWidget(apiComment, 1, 1);

    }

public slots:
    void onInputReturnPressed(){
        gtApi->translate(lneInput->text(), lneSrcLang->text());
    }

    void onTranslationReady(const GTApiTranslation& gtApiTr) {

        apiView->clear();
        apiComment->clear();

        const GTReplyObject & ref = gtApiTr.replyObjectRef();

        QStringList colors;
        colors << "#33CC33" << "#66CCFF" << "#CC33FF" << "#3333CC"
               << "#669999" << "#FF6666" << "#FF3300" << "#CCCC00"
               << "#00CC00" << "#66AA99" << "#FF0066" << "#FFCC00"
               << "#CCBB00" << "#00CC55" << "#99AA99";

        for (int child =0; child < ref.size(); ++child ) {

            QString comment = "Unknown parameter\n";
            QString childDescription = "Unknown/Not-Usable";
            QColor textColor = colors.size() > child ? QColor(colors[child]) : QColor(0, 0, 0);

            if ( child == 0 ) {

                childDescription = "[<Translation>*, <Translit>]";
                comment = QString("Translation:\n\t%1 (%2)\n").arg(gtApiTr.translation().join(""), gtApiTr.translit());
                comment += QString("Original:\n\t%1 (%2)\n").arg(gtApiTr.original().join(""), gtApiTr.sourceTranslit());

            } else if (child == 2) {
                childDescription = "detected_source_lang";
                comment = QString("Detected Source Language: %1\n").arg(gtApiTr.detectedSourceLang());

            }else if (child == 7) {
                childDescription="<SpellChecker>";
                comment = QString("Spell Checker:\n\t%1\nFormatted:\n\t%2\n").arg(gtApiTr.spellChecked(), gtApiTr.spellChecked(true));

            } else if (child == 8) {
                childDescription = "<LangDetect>";
                comment = "Detected Languages:\n";

                QList<GTLangDetect> langsDetect = gtApiTr.detectedSourceLanguages();
                foreach (const GTLangDetect & langDetect, langsDetect ) {
                    comment += QString("\tlang: %1 | reliability: %2\n").arg(langDetect.first, QString::number(langDetect.second));
                }
            } else if (child == 14) {
                childDescription = "<SeeAlsoList>";
                comment = "See Also Suggestions:\n";
                comment += "\t" + gtApiTr.seeAlsoList().join(", ") + "\n";

            } else if (child == 1) {

                childDescription = "[<PosDict>*]";
                comment = "Part-of-speech dictionary:\n";

                QList<GTPosDict> dicts = gtApiTr.getPosDictionary();
                foreach ( const GTPosDict & dict, dicts) {
                    QString header = QString("%1 - %2(#%3)").arg(dict.wordSourceBaseForm, dict.posNameHl, QString::number(dict.posDictEnum));
                    comment += header + "\n";

                    foreach (const GTPosDictEntry & entry, dict.entries) {
                        QString entryStr = QString("\t%1 %2 - (%3)[%4]\n").arg(entry.previousWordTarge, entry.wordTarget, QString::number(entry.score), entry.synsetIds.join(""));
                        comment += entryStr;
                    }
                }

            } else if (child == 11) {
                childDescription = "[<SynonymDict>*]";
                comment = "Synonyms Dictionary:\n";

                QList<GTSynDict> dicts = gtApiTr.getSynDictionary();
                foreach ( const GTSynDict & dict, dicts) {
                    QString header = QString("%1 - %2").arg(dict.wordBaseForm, dict.posName);
                    comment += header + "\n";

                    foreach (const GTSynDictEntry & entry, dict.entries) {
                        comment += QString("\t[%1] :\n\t%2\n").arg(entry.word_id, entry.synonyms.join(','));
                    }
                }

            } else if (child == 12 ) {
                childDescription = "[<DefDict>*]";
                comment = "Definition Dictionary:\n";

                QList<GTDefDict> dicts = gtApiTr.getDefDictionary();
                foreach ( const GTDefDict & dict, dicts) {
                    QString header = QString("%1 - %2").arg(dict.wordBaseForm, dict.posName);
                    comment += header + "\n";

                    foreach (const GTDefDictEntry & entry, dict.entries) {
                        comment += QString("\t[%1] :\n\t%2\n\t%3\n").arg(entry.word_id, entry.definition, entry.definitionExampleUsage);
                    }
                }
            } else if (child == 13 ) {
                childDescription = "[<ExampleDict>*]";
                comment = "Example Dictionary:\n";

                GTExampleDict dict = gtApiTr.getExampleDictionary();
                foreach (const GTExampleDictEntry & entry, dict.entries) {
                    comment += QString("\t[%1] : %2\n").arg(entry.word_id, entry.exampleFormatted);
                }
            }


            apiView->setTextColor(textColor);
            QString childName = QString("Child #%1: %2\n").arg(QString::number(child), childDescription);
            apiView->append( childName + ref[child].toRawString(true)+ "\n");

            apiComment->setTextColor(textColor);
            apiComment->append(comment);
        }
    }
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GTTestWidget wgt;
    wgt.show();

    return app.exec();
}

#include "main.moc"
