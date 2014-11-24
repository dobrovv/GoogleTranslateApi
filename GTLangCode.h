#ifndef GTLANGCODE_H
#define GTLANGCODE_H

#include <QString>
#include <QMap>

////////////////////////////////////////////////////////////////////////////////
// Impliments a table of ISO 639-1 language codes supported by Google Translate

namespace GTLangCode {

    static const QMap<QString, QString>& langMap();

    inline QString langName(const QString & iso639_1) {
        if (langMap().contains(iso639_1))
            return langMap()[iso639_1];
        return QString();
    }

    inline QString isoCode(const QString& languageName) {
        foreach(const QString& key, langMap().keys()){
            if (langMap()[key].compare(languageName, Qt::CaseInsensitive) == 0)
                return key;
        }
        return QString();
    }

    //////////////////////////////////////////////
    // Inizializes the langugages-code map
    // returns a reference to an initialized static map
    // created 2014/11/22

    static const QMap<QString, QString>& langMap() {

        static bool alreadyInitialized = false;
        static QMap<QString, QString> langMap;

        if (alreadyInitialized)
            return langMap;

        langMap["auto"]="Auto";
        langMap["af"]="Afrikaans";
        langMap["sq"]="Albanian";
        langMap["ar"]="Arabic";
        langMap["az"]="Azerbaijani";
        langMap["eu"]="Basque";
        langMap["bn"]="Bengali";
        langMap["be"]="Belarusian";
        langMap["bg"]="Bulgarian";
        langMap["ca"]="Catalan";
        langMap["zh-CN"]="Chinese Simplified";
        langMap["zh-TW"]="Chinese Traditional";
        langMap["hr"]="Croatian";
        langMap["cs"]="Czech";
        langMap["da"]="Danish";
        langMap["nl"]="Dutch";
        langMap["en"]="English";
        langMap["eo"]="Esperanto";
        langMap["et"]="Estonian";
        langMap["tl"]="Filipino";
        langMap["fi"]="Finnish";
        langMap["fr"]="French";
        langMap["gl"]="Galician";
        langMap["ka"]="Georgian";
        langMap["de"]="German";
        langMap["el"]="Greek";
        langMap["gu"]="Gujarati";
        langMap["ht"]="Haitian Creole";
        langMap["iw"]="Hebrew";
        langMap["hi"]="Hindi";
        langMap["hu"]="Hungarian";
        langMap["is"]="Icelandic";
        langMap["id"]="Indonesian";
        langMap["ga"]="Irish";
        langMap["it"]="Italian";
        langMap["ja"]="Japanese";
        langMap["kn"]="Kannada";
        langMap["ko"]="Korean";
        langMap["la"]="Latin";
        langMap["lv"]="Latvian";
        langMap["lt"]="Lithuanian";
        langMap["mk"]="Macedonian";
        langMap["ms"]="Malay";
        langMap["mt"]="Maltese";
        langMap["no"]="Norwegian";
        langMap["fa"]="Persian";
        langMap["pl"]="Polish";
        langMap["pt"]="Portuguese";
        langMap["ro"]="Romanian";
        langMap["ru"]="Russian";
        langMap["sr"]="Serbian";
        langMap["sk"]="Slovak";
        langMap["sl"]="Slovenian";
        langMap["es"]="Spanish";
        langMap["sw"]="Swahili";
        langMap["sv"]="Swedish";
        langMap["ta"]="Tamil";
        langMap["te"]="Telugu";
        langMap["th"]="Thai";
        langMap["tr"]="Turkish";
        langMap["uk"]="Ukrainian";
        langMap["ur"]="Urdu";
        langMap["vi"]="Vietnamese";
        langMap["cy"]="Welsh";
        langMap["yi"]="Yiddish";

        alreadyInitialized = true;
        return langMap;
    }
}

#endif // GTLANGCODE_H
