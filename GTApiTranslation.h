#ifndef GTAPITRANSLATION_H
#define GTAPITRANSLATION_H

#include "GTReplyObject.h"

#include <QStringList>

struct GTPosDict;
struct GTPosDictEntry;
enum   GTPosDictEnum {
    NOUN=1, VERB=2, ADJECTIVE=3, PREPOSITION=5, ABBREVIATION=6, CONJUNCTION=7, PRONOUN=8, INTERJECTION=9, PHRASE=10
};

struct GTDefDict;
struct GTDefDictEntry;

class GTApiTranslation
{
    GTReplyObject root;

    QString queryRawReply;
    QString queryTargetLang;
    QString querySourceLang;
    QString queryHlLang;
    QString queryErrorString;

public:
    GTApiTranslation( const GTReplyObject &);

    QStringList translation() const;
    QStringList original() const;
    QString translit() const;
    QString srcTranslit() const;
    QString srcLang() const;

    const GTReplyObject & replyObjectRef() const;

    // Dictionaries
    QList<GTPosDict> getPosDictionary() const;
    QList<GTDefDict> getDefDictionary() const;

};

/////////////////////////////
// Part-of-speech Dictionary

struct GTPosDict{
    QString posNameHl;
    /*QStringList wordTarget; // are in entries too*/
    QList<GTPosDictEntry> entries;
    QString wordSourceBaseForm;
    GTPosDictEnum posDictEnum;
};

struct GTPosDictEntry{
    QString wordTarget;
    QStringList wordRetranslations;
    QStringList synsetIds;
    qreal score;
    QString previousWordTarge;
};

/////////////////////////
// Definition Dictionary

struct GTDefDict{
    QString posName;
    QList<GTDefDictEntry> entries;
    QString wordBaseForm;
};

struct GTDefDictEntry{
    QString definition;
    QString word_id;
    QString definitionExampleUsage;
};

#endif // GTAPITRANSLATION_H
