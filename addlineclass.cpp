#include "addlineclass.h"

AddLineClass::AddLineClass()
{

}

void AddLineClass::addLine(QString &filePath, QList<QGeoCoordinate> myCoordinates)
{

   QJsonObject geojson = createFeatureObject(myCoordinates);
   geojsonIO.writeFeatureToFile(geojson,filePath);
}

void AddLineClass::appendToLine(QString &filePath, QList<QGeoCoordinate> myCoordinates)
{

    QJsonObject geojson = createFeatureObject(myCoordinates) ;
    geojsonIO.writeFeatureToFile(geojson,filePath);

}


QJsonObject AddLineClass::createFeatureObject(const QList<QGeoCoordinate> &coordinates)
{
    QJsonObject feature ;
    QJsonObject geometry;
    QJsonObject properties;
    QJsonArray jsonCoordinates;

    feature["type"] = "Feature" ;
    properties["name"] = "Line" ;
    feature["properties"]=properties;
    geometry["type"] = "LineString" ;

    for (const QGeoCoordinate& myGeoCoordinates : coordinates){

        QJsonArray coordinate ;
        coordinate.append(myGeoCoordinates.longitude());
        coordinate.append(myGeoCoordinates.latitude()) ;
        jsonCoordinates.append(coordinate);

    }

    geometry["coordinates"] = jsonCoordinates ;
    feature["geometry"] = geometry ;

    return feature ;
}

