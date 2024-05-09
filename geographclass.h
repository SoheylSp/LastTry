#ifndef GEOGRAPHCLASS_H
#define GEOGRAPHCLASS_H
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QGeoCoordinate>
#include "geojsonioclass.h"
#include <cmath>
class GeoGraphClass
{
public:

    GeoGraphClass();
    GeoGraphClass(const GeoGraphClass&)=delete ;
    static GeoGraphClass& getInstance();
    GeoGraphClass& operator=(const GeoGraphClass&) = delete ;
    QJsonObject createLineFeature(const QList<QGeoCoordinate>& myCoordinates);
    QJsonObject createCircleFearute(const QGeoCoordinate& center , double& radius);
    QJsonObject createPolygonFeature(const QList<QGeoCoordinate>& pointList);
    QJsonObject createEllipseObject(const QGeoCoordinate &center, double semiMajorAxis, double semiMinorAxis , int segments) ;
    QJsonObject createGreatCircleFeature(const QGeoCoordinate& point);

    QGeoCoordinate calculatePolygonCenter(const QList<QGeoCoordinate>& points);
    
    GeojsonIOClass geojsonIO;


};

#endif // GEOGRAPHCLASS_H
