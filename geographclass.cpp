#include "geographclass.h"

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
    QJsonObject feature     ;
    QJsonObject properties  ;
    QJsonObject geometry    ;
    QJsonArray jsonCoordinates;

    feature["type"] = "Feature" ;
    properties["name"] = "line" ;
    feature["properties"] = properties ;
    geometry["type"] = "LineString";

    for(const QGeoCoordinate & myGeoCoordinate : myCoordinates){

        QJsonArray coordinate ;
        coordinate.append(myGeoCoordinate.longitude());
        coordinate.append(myGeoCoordinate.latitude());
        jsonCoordinates.append(coordinate);

    }

    geometry["coordinates"] = jsonCoordinates;
    feature["geometry"] = geometry ;

    return feature ;
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

QJsonObject GeoGraphClass::createEllipseObject(const QGeoCoordinate &center, double semiMajorAxis, double semiMinorAxis , int segments)
{

    QJsonObject feature;
    QJsonObject properties;
    QJsonObject geometry;
    QJsonArray coordinates;

    feature["type"] = "Feature";
    feature["properties"] = properties;
    geometry["type"] = "Polygon";

    QJsonArray outerRing;
    for (int i = 0; i < segments; ++i) {
        double angle = (2 * M_PI * i) / segments;
        double x = center.longitude() + (semiMajorAxis * cos(angle));
        double y = center.latitude() + (semiMinorAxis * sin(angle));
        outerRing.append(QJsonArray{x, y});
    }
    // Close the polygon by adding the first point again
    outerRing.append(outerRing.first());

    coordinates.append(outerRing);
    geometry["coordinates"] = coordinates;
    feature["geometry"] = geometry;

    return feature;

}

QJsonObject GeoGraphClass::createGreatCircleFeature(const QGeoCoordinate& point)
{
    QJsonObject feature;
    QJsonObject properties;
    QJsonObject geometry;
    QJsonArray coordinates;

    feature["type"] = "Feature";
    feature["properties"] = properties;
    geometry["type"] = "Point";
    coordinates.append(point.longitude());
    coordinates.append(point.latitude());

    geometry["coordinates"] = coordinates;
    feature["geometry"] = geometry;

    return feature;
}

QGeoCoordinate GeoGraphClass::calculatePolygonCenter(const QList<QGeoCoordinate> &points)
{
    double totalLat = 0.0 ;
    double totalLon = 0.0 ;

    for(const auto& point : points){

        totalLat += point.latitude();
        totalLon += point.longitude();

    }

    return QGeoCoordinate(totalLat / points.size() , totalLon / points.size());
}
