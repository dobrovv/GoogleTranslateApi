#include "GTApiTranslation.h"

#include <QDebug>

#include <QRegularExpression>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

static QString replaceUnicodeChars (QString text);

GTApiTranslation::GTApiTranslation(QNetworkReply *googleTranslateReply)
{
    QUrlQuery googleQuery (googleTranslateReply->request().url());
    querySourceLang  = googleQuery.queryItemValue("sl");
    queryTargetLang  = googleQuery.queryItemValue("tl");
    queryHlLang      = googleQuery.queryItemValue("hl");
    queryErrorString = googleTranslateReply->errorString();

    rawReply   =  QString::fromUtf8( googleTranslateReply->readAll() );

    if ( !rawReply.isEmpty() ){
        qDebug() << "[Parser] Starting";
        root = GTReplyObject::fromRawString(rawReply);
        qDebug() << "[Parser] Parsed without errors: " << (rawReply == root.toRawString());
    } else {
        qDebug() << Q_FUNC_INFO << "Empty Reply";
    }
}

QStringList GTApiTranslation::translation() const
{
    QStringList res;

    /* fetch sentences from Transation and Translit */
    foreach( const GTReplyObject &sentence, root[0] ){
        res << sentence[0].toString();
    }

    /* remove Translit if it exists */
    if (!res.isEmpty() && !translit().isEmpty())
        res.removeLast();

    return res;
}

QStringList GTApiTranslation::original() const
{
    QStringList res;

    /* fetch sentences from Transation and Translit */
    foreach( const GTReplyObject &sentence, root[0] )
        res << sentence[1].toString();

    /* remove Translit sentece if it exists */
    if (!res.isEmpty() && !translit().isEmpty())
        res.removeLast();

    return res;
}

QString GTApiTranslation::translit() const
{
    QString res;
    const GTReplyObject & translitObj = root[0][ root[0].size()-1 ];
    if (translitObj.type() != GTReplyObject::UNDEF && translitObj.size() >= 4 )
        res = translitObj[2].toString();

    return res;
}

QString GTApiTranslation::sourceTranslit() const
{
    QString res;
    const GTReplyObject & translitObj = root[0][ root[0].size()-1 ];
    if (translitObj.type() != GTReplyObject::UNDEF && translitObj.size() >= 4 )
        res = translitObj[3].toString();

    return res;
}

QString GTApiTranslation::detectedSourceLang() const
{
    return root[2].toString();
}

QList<GTLangDetect> GTApiTranslation::detectedSourceLanguages() const
{
    QList<GTLangDetect> res;
    const GTReplyObject & langDetect = root[8];
    QStringList detectedLangs = langDetect[0].toStringList();

    for (int i = 0; i < detectedLangs.size(); ++i) {
        res.push_back(GTLangDetect(detectedLangs[i], langDetect[2][i].toNum()));
    }

    return res;
}

QStringList GTApiTranslation::seeAlsoList() const
{
    return root[14][0].toStringList();
}

QString GTApiTranslation::spellChecked(bool formatted) const
{
    if ( formatted ){
        return replaceUnicodeChars(root[7][0].toString());
    } else {
        return root[7][1].toString();
    }
}

QList<GTPosDict> GTApiTranslation::getPosDictionary() const
{
    QList<GTPosDict> res;

    foreach ( const GTReplyObject gtDict, root[1] ) {

        GTPosDict dictPos;
        dictPos.posNameHl    = gtDict[0].toString();
        dictPos.posDictEnum  = (GTPosDictEnum)(static_cast<int>(gtDict[4].toNum()));
        dictPos.wordSourceBaseForm = gtDict[3].toString();

        foreach (const GTReplyObject gtDictEntry, gtDict[2]) {

            GTPosDictEntry dictPosEntry;
            dictPosEntry.wordTarget         = gtDictEntry[0].toString();
            dictPosEntry.wordRetranslations = gtDictEntry[1].toStringList();
            dictPosEntry.synsetIds  = gtDictEntry[2].toStringList();
            dictPosEntry.score      = gtDictEntry[3].toNum();
            dictPosEntry.previousWordTarge  = gtDictEntry[4].toString();

            dictPos.entries.append(dictPosEntry);
        }

        res.append(dictPos);
    }
    return res;
}

QList<GTSynDict> GTApiTranslation::getSynDictionary() const
{
    QList<GTSynDict> res;

    foreach ( const GTReplyObject gtDict, root[11] ) {

        GTSynDict dictSyn;
        dictSyn.posName    = gtDict[0].toString();
        dictSyn.wordBaseForm = gtDict[2].toString();

        foreach (const GTReplyObject & gtDictEntry, gtDict[1]) {

            GTSynDictEntry dictSynEntry;
            dictSynEntry.synonyms = gtDictEntry[0].toStringList();
            dictSynEntry.word_id  = gtDictEntry[1].toString();

            dictSyn.entries.append(dictSynEntry);
        }

        res.append(dictSyn);
    }
    return res;
}

QList<GTDefDict> GTApiTranslation::getDefDictionary() const
{
    QList<GTDefDict> res;

    foreach ( const GTReplyObject & gtDict, root[12] ) {

        GTDefDict dictDef;
        dictDef.posName         = gtDict[0].toString();
        dictDef.wordBaseForm    = gtDict[2].toString();

        foreach (const GTReplyObject & gtDictEntry, gtDict[1]) {

            GTDefDictEntry dictDefEntry;
            dictDefEntry.definition = gtDictEntry[0].toString();
            dictDefEntry.word_id    = gtDictEntry[1].toString();
            dictDefEntry.definitionExampleUsage = gtDictEntry[2].toString();

            dictDef.entries.append(dictDefEntry);
        }

        res.append(dictDef);
    }
    return res;
}

GTExampleDict GTApiTranslation::getExampleDictionary() const
{
    GTExampleDict dictExample;

    foreach (const GTReplyObject & gtDictEntry, root[13][0]) {

        GTExampleDictEntry dictExampleEntry;
        dictExampleEntry.exampleFormatted  = replaceUnicodeChars(gtDictEntry[0].toString());
        dictExampleEntry.word_id  = gtDictEntry[5].toString();

        dictExample.entries.push_back(dictExampleEntry);

    }
    return dictExample;
}

const GTReplyObject & GTApiTranslation::replyObjectRef() const
{
    return root;
}

// replce unicode chars \u003c and \u003e
static QString replaceUnicodeChars (QString text){
    text.replace(QRegularExpression("\\\\u003c"), "<");
    text.replace(QRegularExpression("\\\\u003e"), ">");
    qDebug() << text;
    return text;
}
