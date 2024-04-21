#ifndef ADDCIRCLECLASS_H
#define ADDCIRCLECLASS_H

#include <QGeoCoordinate>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>


class AddCircleClass
{
public:
    AddCircleClass();

    void addCircle(const QString &filePath , const QGeoCoordinate &center , double radius);

private:

    QJsonObject createCircleFeature(const QGeoCoordinate &center  , double radius);
    void writeGeojsonToFile(const QJsonObject &geojson , const QString &filePath);

};

#endif // ADDCIRCLECLASS_H
