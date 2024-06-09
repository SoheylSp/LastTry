#ifndef GEOGRAPHCLASS_H
#define GEOGRAPHCLASS_H
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QGeoCoordinate>
#include "geojsonio.h"
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
    QJsonObject createEllipseFeature(const QGeoCoordinate center , double semiMajorAxis , double semiMinorAxis , int segments ) ;
    QJsonObject createGreatCircleFeature(const QGeoCoordinate& point1, const QGeoCoordinate& point2, int numPoints);
    QJsonObject createRectangleFeature(const QGeoCoordinate &center , const double& width ,const double & height);
    QJsonObject creatRectangleFeatureWithDeg(const QGeoCoordinate& center , const double& widthDeg , const double& heightDeg);
    QGeoCoordinate calculatePolygonCenter(const QList<QGeoCoordinate>& points);

    
    GeojsonIOClass geojsonIO;


};

#endif // GEOGRAPHCLASS_H
