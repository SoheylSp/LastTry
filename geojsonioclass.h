#ifndef GEOJSONIOCLASS_H
#define GEOJSONIOCLASS_H

#include <QGeoCoordinate>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QStringList>
#include <QDebug>

class GeojsonIOClass
{
public:
    GeojsonIOClass();


    void writeFeaturesToFile(const QJsonArray &features , const QString& filePath , bool append = false);
    void writeGeojsonToFile(const QJsonObject &geojson , const QString& filePath );
    QJsonObject readGeojsonFromFile(const QString& filePath);



};

#endif // GEOJSONIOCLASS_H
