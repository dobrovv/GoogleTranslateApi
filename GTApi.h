#ifndef GTAPI_H
#define GTAPI_H

#include "GTApiTranslation.h"

#include <QObject>

class QNetworkAccessManager;

class GTApi : public QObject
{
    Q_OBJECT
    QNetworkAccessManager *net;

public:
    explicit GTApi(QObject *parent = 0);
    ~GTApi();

signals:
    void translationReady( const GTApiTranslation & gtApiTr );

public slots:

    // Translates the text, parsed responce is returned with the translationReady signal
    void translate(const QString& text, const QString& targetLang,
                   const QString& sourceLang = "auto", const QString& interfaceLang = "en");

private slots:
    void onReplyFinished();
};

#endif // GTAPI_H
