///////////////////////////////////////////////////////////////////////////
// This class provides an interface to get the desired information
//      from the Google Translate Web Gadget
// It's returned from GTApi::translate() function

#ifndef GTAPITRANSLATION_H
#define GTAPITRANSLATION_H

#include "GTReplyObject.h"

#include <QStringList>

struct GTPosDict;
struct GTPosDictEntry;
enum   GTPosDictEnum {
    NOUN=1, VERB=2, ADJECTIVE=3, PREPOSITION=5, ABBREVIATION=6,
    CONJUNCTION=7, PRONOUN=8, INTERJECTION=9, PHRASE=10
};

typedef QPair<QString, qreal> GTLangDetect;

struct GTSynDict;
struct GTSynDictEntry;

struct GTDefDict;
struct GTDefDictEntry;

struct GTExampleDict;
struct GTExampleDictEntry;

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
    QString sourceTranslit() const;
    QString detectedSourceLang() const;

    // returns a list holding the detected languages and reliability of the detection( from 0 to 1.0)
    QList<GTLangDetect> detectedSourceLanguages() const;
    QStringList seeAlsoList() const;

    // Dictionaries
    QList<GTPosDict> getPosDictionary() const;
    QList<GTSynDict> getSynDictionary() const;
    QList<GTDefDict> getDefDictionary() const;
    GTExampleDict getExampleDictionary() const;

    // returns a reference to raw object
    const GTReplyObject & replyObjectRef() const;

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

//////////////////////////
// Synonym Dictionary

struct GTSynDict{
    QString posName;
    QList<GTSynDictEntry> entries;
    QString wordBaseForm;
};

struct GTSynDictEntry {
    QStringList synonyms;
    QString word_id;
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

/////////////////////////
// Examples Dictionary

struct GTExampleDict{
    QList<GTExampleDictEntry> entries;
};

struct GTExampleDictEntry{
    QString example;
    QString word_id;
};

#endif // GTAPITRANSLATION_H
