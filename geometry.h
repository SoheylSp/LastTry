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

    static QJsonObject createLineFeature(const QList<QGeoCoordinate>& myCoordinates);
    static QJsonObject createCircleFearute(const QGeoCoordinate& center , double& radius);
    static QJsonObject createPolygonFeature(const QList<QGeoCoordinate>& pointList);
    static QJsonObject createEllipseFeature(const QGeoCoordinate center , double semiMajorAxis , double semiMinorAxis , int segments ) ;
    static QJsonObject createGreatCircleFeature(const QGeoCoordinate& point1, const QGeoCoordinate& point2, int numPoints);
    static QJsonObject createRectangleFeature(const QGeoCoordinate &center , const double& width ,const double & height);
    static QJsonObject creatRectangleFeatureWithDeg(const QGeoCoordinate& center , const double& widthDeg , const double& heightDeg);
    static QGeoCoordinate calculatePolygonCenter(const QList<QGeoCoordinate>& points);

    
    GeojsonIOClass geojsonIO;


};

#endif // GEOGRAPHCLASS_H
