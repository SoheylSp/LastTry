#ifndef ADDCIRCLECLASS_H
#define ADDCIRCLECLASS_H

#include <QGeoCoordinate>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include "geojsonioclass.h"

class AddCircleClass
{
public:
    AddCircleClass();

    void addCircle(const QString &filePath , const QGeoCoordinate &center , double radius);
    QJsonObject createCircleFeature(const QGeoCoordinate &center  , double radius);

    GeojsonIOClass geojsonIO ;

};

#endif // ADDCIRCLECLASS_H
