#include "geometry.h"
#include <QGeoPolygon>
GeoGraphClass::GeoGraphClass():geojsonIO(GeojsonIOClass())
{
    
}

GeoGraphClass &GeoGraphClass::getInstance()
{
    static GeoGraphClass instance;
    return instance;
}

QJsonObject GeoGraphClass::createLineFeature(const QList<QGeoCoordinate> &myCoordinates)
{
    
    
    
    QJsonObject newFeature;
    newFeature["type"] = "Feature";
    QJsonObject geometry;
    geometry["type"] = "LineString";
    QJsonArray coordinates;
    QJsonObject properties ;
    
    for (const QGeoCoordinate &coordinate : myCoordinates) {
        QJsonArray coordinateArray;
        coordinateArray.append(coordinate.longitude()); // Longitude first in GeoJSON
        coordinateArray.append(coordinate.latitude()); // Latitude second
        coordinates.append(coordinateArray);
    }
    
    geometry["coordinates"] = coordinates;
    newFeature["geometry"] = geometry;
    newFeature["properties"] = properties;
    
    return newFeature;
    
}

QJsonObject GeoGraphClass::createCircleFearute(const QGeoCoordinate &center, double &radius)
{
    
    QJsonObject feature;
    QJsonObject properties;
    QJsonObject geometry;
    QJsonArray coordinates;
    
    feature["type"] = "Feature";
    properties["name"] = "Circle";
    feature["properties"] = properties;
    
    QJsonArray outerRing;
    for (int i = 0; i <= 360; i += 10) {
        QGeoCoordinate point = center.atDistanceAndAzimuth(radius, i);
        outerRing.append(QJsonArray{ point.longitude(), point.latitude() });
    }
    
    geometry["type"] = "Polygon";
    geometry["coordinates"] = QJsonArray({ outerRing });
    feature["geometry"] = geometry;
    
    return feature;
    
}

QJsonObject GeoGraphClass::createPolygonFeature(const QList<QGeoCoordinate> &pointsList)
{
    
    QJsonObject feature;
    QJsonObject properties;
    QJsonObject geometry;
    QJsonArray coordinates;
    
    feature["type"] = "Feature";
    feature["properties"] = properties;
    geometry["type"] = "Polygon";
    
    QJsonArray polygonCoordinates;
    for (const auto &point : pointsList) {
        polygonCoordinates.append(QJsonArray{point.longitude(), point.latitude()});
    }
    // Closing the polygon by adding the first point again
    polygonCoordinates.append(polygonCoordinates.first()); // Add the first point again to close the polygon
    
    coordinates.append(polygonCoordinates);
    geometry["coordinates"] = coordinates;
    feature["geometry"] = geometry;
    
    return feature;
}

QJsonObject GeoGraphClass::createEllipseFeature(const QGeoCoordinate center, double semiMajorAxis, double semiMinorAxis, int segments)
{
    QJsonObject newFeature ;
    newFeature["type"] = "Feature" ;
    QJsonObject geometry ;
    geometry["type"] = "Polygon" ;
    QJsonArray coordinates  ;
    QJsonArray outerRing ;
    QJsonObject properties ;
    
    //Generate coordinates for the ellipse
    
    double step = 5.0 ;//step size in degrees
    

}

QJsonObject GeoGraphClass::createGreatCircleFeature(const QGeoCoordinate& point1, const QGeoCoordinate& point2, int numPoints) {
    QJsonObject feature;
    feature["type"] = "FeatureCollection";
    QJsonArray geometries;
    
    double distance = point1.distanceTo(point2);
    double azimuth = point1.azimuthTo(point2);
    
    for (int i = 0; i <= numPoints; ++i) {
        double f = static_cast<double>(i) / numPoints;
        QGeoCoordinate intermediatePoint = point1.atDistanceAndAzimuth(distance * f, azimuth);
        QJsonObject geometry;
        geometry["type"] = "Point";
        QJsonArray coordinates;
        coordinates.append(intermediatePoint.longitude());
        coordinates.append(intermediatePoint.latitude());
        geometry["coordinates"] = coordinates;
        geometries.append(geometry);
    }
    
    feature["features"] = geometries;
    return feature;
}

QJsonObject GeoGraphClass::createRectangleFeature(const QGeoCoordinate &center, const double &width, const double &height)
{
    
    
    QJsonObject feature ;
    feature["type"] = "Feature" ;
    QJsonObject properties;
    
    QJsonObject geometry ;
    geometry["type"] = "Polygon" ;
    
    QJsonArray coordinates ;
    QJsonArray outerRing ;
    
    //Calculate the coordinates of the rectangle
    
    
    double halfWidth = width/2 ;
    double halfHeight = height/2;
    
    //Use atDistanceAndAzimuth()to calculate the coordinates
    
    outerRing.append(QJsonArray{center.atDistanceAndAzimuth(halfWidth, 270).longitude(),
                                center.atDistanceAndAzimuth(halfHeight, 180).latitude()});
    
    outerRing.append(QJsonArray{center.atDistanceAndAzimuth(halfWidth, 90).longitude(),
                                center.atDistanceAndAzimuth(halfHeight, 180).latitude()});
    
    outerRing.append(QJsonArray{center.atDistanceAndAzimuth(halfWidth, 90).longitude(),
                                center.atDistanceAndAzimuth(halfHeight, 0).latitude()});
    
    outerRing.append(QJsonArray{center.atDistanceAndAzimuth(halfWidth, 270).longitude(),
                                center.atDistanceAndAzimuth(halfHeight, 0).latitude()});
    
    outerRing.append(QJsonArray{center.atDistanceAndAzimuth(halfWidth, 270).longitude(),
                                center.atDistanceAndAzimuth(halfHeight, 180).latitude()});
    
    coordinates.append(outerRing);
    geometry["coordinates"] = coordinates ;
    feature["geometry"] =geometry ;
    
    feature["properties"]= properties;
    
    return feature;
}

QJsonObject GeoGraphClass::creatRectangleFeatureWithDeg(const QGeoCoordinate &center, const double &widthDeg, const double &heightDeg)
{
    
    QJsonObject feature ;
    QJsonObject geometry ;
    QJsonObject properties;
    QJsonArray coordinates ;
    
    feature["type"] = "Feature" ;
    feature["properties"] = properties;
    
    // double halfWidth = widthDeg/2;
    // double halfHeight = heightDeg/2;
    
    QGeoCoordinate point1,point2,point3,point4;
    
    point1.setLatitude(center.latitude() - heightDeg/2);
    point1.setLongitude(center.longitude() - widthDeg/2);
    
    point2.setLatitude(center.latitude() -heightDeg/2);
    point2.setLongitude(center.longitude() + widthDeg/2);
    
    point3.setLatitude(center.latitude() + heightDeg/2);
    point3.setLongitude(center.longitude() -widthDeg/2);
    
    point4.setLatitude(center.latitude() + heightDeg/2);
    point4.setLongitude(center.longitude()+widthDeg/2);
    
    
    coordinates.append(QJsonArray{point1.longitude(),point1.latitude()});
    coordinates.append(QJsonArray{point2.longitude(),point2.latitude()});
    coordinates.append(QJsonArray{point3.longitude(),point3.latitude()});
    coordinates.append(QJsonArray{point4.longitude(),point4.latitude()});
    
    //Append the first coordinate again to close the polygon
    
    coordinates.append(QJsonArray{point1.longitude(),point1.latitude()});
    
    geometry["coordinates"] = coordinates ;
    geometry["type"] = "Polygon";
    feature["geometry"] = geometry ;
    
    return feature;
    
}

QGeoCoordinate GeoGraphClass::calculatePolygonCenter(const QList<QGeoCoordinate> &points)
{
    QGeoPolygon myPolygon ;
    
    for(const auto point : points){
        
        myPolygon.addCoordinate(point);
    }
    
    return myPolygon.center();
}
