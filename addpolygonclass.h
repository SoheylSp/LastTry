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
#include "geojsonioclass.h"


class AddPolygonClass {
public:
    AddPolygonClass();

    void addPolygon(const QList<QGeoCoordinate> &pointsList, const QString& filePath);
    QJsonObject createPolygonFeature(const QList<QGeoCoordinate> &pointsList);
    GeojsonIOClass geojsonIO ;

};

#endif // ADDPOLYGONCLASS_H
