#ifndef GTREPLYOBJECT_H
#define GTREPLYOBJECT_H

#include <QString>
#include <QList>

class GTReplyObject
{

public:
    GTReplyObject();

    /* Object type checkers */
    bool    isNull() const;
    bool    isBool() const;
    bool    isNum()  const;
    bool    isString() const;
    bool    isObject() const;

    /* Getters */
    bool          toBool() const;
    qreal         toNum()  const;
    QString       toString() const;
    QStringList   toStringList() const;
    int           size() const;

    const GTReplyObject & child(int which) const;
    const GTReplyObject & operator[](int i) const;

    /* Iterators */
    typedef QList<GTReplyObject>::const_iterator const_iterator;

    const_iterator begin() const{
        return obj_childs.begin();
    }
    const_iterator end() const{
        return obj_childs.end();
    }

    static GTReplyObject fromRawString (const QString& rawReply);
    QString toRawString() const;

    enum GTReplyObjectType {UNDEF=0, BOOL, NUM, STRING, ARRAY};

private:
    GTReplyObjectType       obj_type;
    QList<GTReplyObject>    obj_childs;
    QString                 obj_string;

    friend uint readObjectParam(const QString& raw, uint ofst, GTReplyObject& outParam);
    friend uint readObjectArray(const QString& raw, uint ofst, GTReplyObject& outObject);

};

#endif // GTREPLYOBJECT_H
