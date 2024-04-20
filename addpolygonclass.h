#ifndef ADDPOLYGONCLASS_H
#define ADDPOLYGONCLASS_H

#include <QString>
#include <QGeoCoordinate>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QStringList>
#include <QDebug>

class AddPolyGonClass
{
public:
    AddPolyGonClass();

    void addPolygon(int numPoints , QList<QGeoCoordinate> pointsList , const QString& filepath);

private:
    void writeGeojsonToFile(const QJsonObject &geojson , const QString& filepath);

};

#endif // ADDPOLYGONCLASS_H
