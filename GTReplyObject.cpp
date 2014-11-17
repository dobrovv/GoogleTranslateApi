#include "GTReplyObject.h"

#include <QDebug>
#include <QRegularExpression>

// Object to return on index-out-of-range access
static GTReplyObject invalidObject;

GTReplyObject::GTReplyObject()
{
    obj_type = UNDEF;
}

////////////////////////
// Object type checkers

bool GTReplyObject::isNull() const
{
    return obj_type == UNDEF;
}

bool GTReplyObject::isBool() const
{
    return obj_type == BOOL;
}

bool GTReplyObject::isNum() const
{
    return obj_type == NUM;
}

bool GTReplyObject::isString() const
{
    return obj_type == STRING;
}

bool GTReplyObject::isObject() const
{
    return obj_type == ARRAY;
}

GTReplyObject::GTReplyObjectType GTReplyObject::type() const
{
    return obj_type;
}

////////////
// Getters

bool GTReplyObject::toBool() const
{
    return obj_string == "true"? true : false;
}

qreal GTReplyObject::toNum()  const
{
    return obj_string.toDouble();
}

QString GTReplyObject::toString() const
{
    return obj_string;
}

QStringList GTReplyObject::toStringList() const
{
    QStringList res;
    for ( int i = 0; i < obj_childs.size(); ++i ) {
        res << child(i).toString();
    }
    return res;
}

int GTReplyObject::size() const
{
    return obj_childs.size();
}

const GTReplyObject &GTReplyObject::child(int which) const
{
    if ( which < 0 || which >= obj_childs.size() )
        return invalidObject;
    return obj_childs[which];
}

const GTReplyObject & GTReplyObject::operator[](int which) const
{
    if ( which < 0 || which >= obj_childs.size() )
        return invalidObject;
    return obj_childs[which];
}

///////////////////////////////////////////
// generate an GTReplyObject from a string

GTReplyObject GTReplyObject::fromRawString(const QString &rawReply)
{
    GTReplyObject obj;
    readObjectParam(rawReply, 0, obj);

    return obj;
}

///////////////////////////////////////////
// generate a string from an GTReplyObject

QString GTReplyObject::toRawString(bool prettyDecoded) const {
    QString res;

    if ( obj_type == GTReplyObject::ARRAY ) {
        QStringList childsStr;
        foreach ( GTReplyObject child, obj_childs )
            childsStr << child.toRawString(prettyDecoded);

        if (!prettyDecoded) {
            res = '[' + childsStr.join(',') + ']';
        } else {
            QString childJoin = "\n" + childsStr.join(",\n");
            childJoin.replace(QRegularExpression("\\n"), "\n\t");
            res = "[" + childJoin+ "\n]";
        }

    } else {
        res = obj_string;
        if (obj_type == GTReplyObject::STRING)
            res = "\"" + res + "\"";
    }

    return res;
}

/////////////////////////////////////////////////
// reads an object parameter from the raw string
// return the string length of the parametre

uint readObjectParam(const QString& raw, uint ofst_start, GTReplyObject& outParam) {

    uint ofst = ofst_start;

    // skip whitespaces in the beginning
    while ( raw[ofst].isSpace() )
        ofst++;

    // determine parameter's type
    // if it's an object read it and return
    if ( raw[ofst] == '[') {
        outParam.obj_type = GTReplyObject::ARRAY;
        return readObjectArray(raw, ofst, outParam);
    }

    uint len = 0;

    // determine the length of the raw parameter
    while ( ofst+len < (uint)raw.length() ) {
        if ( raw[ofst+len] == ',' || raw[ofst+len] == ']' ) {
            break;

            // skip over the string ("exam\"ple"),
        } else if ( raw[ofst+len] == '"' ) {
            for ( len+=1; ofst+len < (uint)raw.length(); ++len ) {
                // skip the escapeped charcater (ex. '\"')
                if (raw[ofst+len] == '\\') {
                    len += 1;
                } else if (raw[ofst+len] == '"') {
                    len += 1;
                    break;
                }
            }
        } else {
            ++len;
        }
    }

    // read and trim the raw parameter into string
    QString param = raw.mid(ofst, len).trimmed();

    // determine parameter's type
    // if it's empty (ex. [,,]) set as UNDEF
    if ( param.isEmpty() ) {
        outParam.obj_type = GTReplyObject::UNDEF;

        // it's a boolean
    } else if ( param == "true" ) {
        outParam.obj_type = GTReplyObject::BOOL;

    } else if ( param == "false" ) {
        outParam.obj_type = GTReplyObject::BOOL;

        // it's a string
    } else if ( param[0] == '\"' && param[param.size()-1] == '\"' ){
        outParam.obj_type = GTReplyObject::STRING;
        param = param.mid(1, param.size()-1-1);   // remove double quotes (") surrounding the param

        // it's a number
    } else {
        outParam.obj_type = GTReplyObject::NUM;
        bool isNumber;
        param.toDouble(&isNumber);

        if (!isNumber){
            outParam.obj_type = GTReplyObject::UNDEF;
            qDebug() << Q_FUNC_INFO << "[Parser] Error: String is not a valid parameter:" << param;
        }
    }

    // set object's parameter string
    outParam.obj_string = param;

    return ofst - ofst_start + len;
}

/////////////////////////////////////////
// reads an object from the raw stream
// return the string lenght of the object

uint readObjectArray(const QString& raw, uint ofst_start, GTReplyObject& outObject) {

    uint ofst = ofst_start;

    // check that raw stream is an array
    if (raw[ofst++] != '['){
        // PANIC!
        qDebug() << Q_FUNC_INFO << "Parser Error: Array doesn't start with '['";
        return uint(-1);
    }

    int len = 0;

    // read the children
    while ( ofst < (uint)raw.length() ) {
        // read a parameter
        GTReplyObject child;
        len = readObjectParam(raw, ofst, child);
        ofst += len;
        outObject.obj_childs.append(child);

        // check for stream end
        if ( ofst >= (uint)raw.length() ){
            break;
        }

        // jump over parameter delimiters (',' or ']') in the stream
        if ( raw[ofst] == ',' ){
            ++ofst;
            continue;
        } else if ( raw[ofst] == ']' ){
            ++ofst;
            break;
        } else {
            // PANIC
            qDebug() << Q_FUNC_INFO << "Parser Error: Parameter delimiter isn't ',' or ']'";
            break;
        }
    }

    return ofst - ofst_start;
}
