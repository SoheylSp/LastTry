#ifndef ADDLINECLASS_H
#define ADDLINECLASS_H

#include <QGeoCoordinate>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include "geojsonioclass.h"

class AddLineClass
{
public:
    AddLineClass();

    void addLine( QString& filepath ,QList<QGeoCoordinate> myCoordinates);
    void appendToLine( QString& filePath , QList<QGeoCoordinate>myCoordinates);

private:



    QJsonObject createFeatureObject(const QList<QGeoCoordinate> &coordinates) ;

   GeojsonIOClass geojsonIO ;

};

#endif // ADDLINECLASS_H
