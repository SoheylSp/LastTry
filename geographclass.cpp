#include "geographclass.h"

GeoGraphClass::GeoGraphClass():geojsonIO(GeojsonIOClass())
{

}

GeoGraphClass &GeoGraphClass::getInstance()
{
    static GeoGraphClass instance;
    return instance;
}

void GeoGraphClass::addLine(const QString &filePath, const QList<QGeoCoordinate> &myCoordinates, bool append)
{

    QJsonObject lineFeature = createLineFeature(myCoordinates);
    QJsonArray featureArray;
    featureArray.append(lineFeature);
    geojsonIO.writeFeaturesToFile(featureArray, filePath, append);

}

void GeoGraphClass::addCircle(const QString &filePath, const QGeoCoordinate &center, double radius, bool append)
{

    QJsonObject circleFeature = createCircleFearute(center, radius);
    QJsonArray featureArray;
    featureArray.append(circleFeature);
    geojsonIO.writeFeaturesToFile(featureArray,filePath, append);

}

void GeoGraphClass::addPolygon(const QList<QGeoCoordinate> &pointsList, const QString &filePath, bool append)
{

    QJsonObject polygonFeature = createPolygonFeature(pointsList);
    QJsonArray featureArray;
    featureArray.append(polygonFeature);
    geojsonIO.writeFeaturesToFile(featureArray, filePath, append);

}

void GeoGraphClass::addEllipse(const QString &filePath, const QGeoCoordinate &center, double semiMajorAxis, double semiMinorAxis , int segments)
{
    QJsonObject ellipseFeature = createEllipseObject(center, semiMajorAxis, semiMinorAxis, segments);
    QJsonArray featureArray;
    featureArray.append(ellipseFeature);
    geojsonIO.writeFeaturesToFile(featureArray, filePath, true); // Assuming we always append for ellipses
}

void GeoGraphClass::calculateGreatCircle(const QString &filePath , const QGeoCoordinate point1, const QGeoCoordinate point2, int numPoints)
{
    QJsonArray features;
    for (int i = 0; i <= numPoints; ++i) {
        double f = static_cast<double>(i) / numPoints;
        double distance = point1.distanceTo(point2);
        double azimuth = point1.azimuthTo(point2);
        QGeoCoordinate interMediatePoint = point1.atDistanceAndAzimuth(distance * f, azimuth);
        QJsonObject feature = createGreatCircleFeature(interMediatePoint);
        features.append(feature);
    }
    geojsonIO.writeFeaturesToFile(features, filePath, true); // Assuming we always append for great circles

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
