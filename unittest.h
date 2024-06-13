#ifndef UNITTEST_H
#define UNITTEST_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QGeoCoordinate>
#include <QDebug>


class UnitTest
{
public:
    UnitTest();

    static QJsonArray getGeometryCoords (QJsonObject feature ) ;

    static void testOfTranslate(QJsonArray originalCoords , QJsonArray translatedCoords  , double translateNumber);

};

#endif // UNITTEST_H
