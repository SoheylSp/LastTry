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

    void writeFeatureToFile(const QJsonObject &feature , const QString& filePath , bool append = false);
    void writeFeaturesToFile(const QJsonArray &features , const QString& filePath , bool append = false);

private:

    void writeGeojsonToFile(const QJsonObject &geojson , const QString& filePath , bool append);

};

#endif // GEOJSONIOCLASS_H
