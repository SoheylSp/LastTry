#include "modifier.h"
#include "geograph.h"
#include <cmath>

ModifierClass::ModifierClass()
{

}

QJsonObject ModifierClass::rotateModifier(const QGeoCoordinate &rotatePoint, double rotateAngle, const QJsonObject &feature)
{

    QJsonObject modifiedFeature = feature;
    QJsonObject geometry = modifiedFeature["geometry"].toObject();
    QString geometryType = geometry["type"].toString();

    // Find the center of the geometry
    QGeoCoordinate center = findCenterOfGeometry(feature);

    // Apply the rotation around the rotate point
    if (geometryType == "Point") {
        QJsonArray coordinates = geometry["coordinates"].toArray();
        double latitude = coordinates[1].toDouble();
        double longitude = coordinates[0].toDouble();

        // Rotate the point around the rotate point
        QGeoCoordinate rotatedPoint = rotatePoint.atDistanceAndAzimuth(
                    center.distanceTo(QGeoCoordinate(latitude, longitude)),
                    center.azimuthTo(QGeoCoordinate(latitude, longitude)) + rotateAngle
                    );

        // Update the coordinates
        coordinates[0] = rotatedPoint.longitude();
        coordinates[1] = rotatedPoint.latitude();
        geometry["coordinates"] = coordinates;
    } else if (geometryType == "LineString") {
        QJsonArray coordinates = geometry["coordinates"].toArray();
        for (int i = 0; i < coordinates.size(); i++) {
            QJsonArray coordinateArray = coordinates[i].toArray();
            double latitude = coordinateArray[1].toDouble();
            double longitude = coordinateArray[0].toDouble();

            // Rotate the point around the rotate point
            QGeoCoordinate rotatedPoint = rotatePoint.atDistanceAndAzimuth(
                        center.distanceTo(QGeoCoordinate(latitude, longitude)),
                        center.azimuthTo(QGeoCoordinate(latitude, longitude)) + rotateAngle
                        );

            // Update the coordinates
            coordinateArray[0] = rotatedPoint.longitude();
            coordinateArray[1] = rotatedPoint.latitude();
            coordinates[i] = coordinateArray;
        }
        geometry["coordinates"] = coordinates;
    } else if (geometryType == "Polygon") {
        QJsonArray coordinates = geometry["coordinates"].toArray();
        for (int i = 0; i < coordinates.size(); i++) {
            QJsonArray linearRingCoordinates = coordinates[i].toArray();
            for (int j = 0; j < linearRingCoordinates.size(); j++) {
                QJsonArray coordinateArray = linearRingCoordinates[j].toArray();
                double latitude = coordinateArray[1].toDouble();
                double longitude = coordinateArray[0].toDouble();

                // Rotate the point around the rotate point
                QGeoCoordinate rotatedPoint = rotatePoint.atDistanceAndAzimuth(
                            center.distanceTo(QGeoCoordinate(latitude, longitude)),
                            center.azimuthTo(QGeoCoordinate(latitude, longitude)) + rotateAngle
                            );

                // Update the coordinates
                coordinateArray[0] = rotatedPoint.longitude();
                coordinateArray[1] = rotatedPoint.latitude();
                linearRingCoordinates[j] = coordinateArray;
            }
            coordinates[i] = linearRingCoordinates;
        }
        geometry["coordinates"] = coordinates;
    }

    modifiedFeature["geometry"] = geometry;
    return modifiedFeature;

}




QJsonObject ModifierClass::translatorModifier(double translateNumber , QJsonObject feature)
{

    QJsonObject result;
    QJsonObject geometry = feature["geometry"].toObject();
    QJsonArray coordinates = geometry["coordinates"].toArray();

    // Translate the coordinates
    for (int i = 0; i < coordinates.size(); i++) {
        QJsonArray coord = coordinates[i].toArray();
        coord[0] = coord[0].toDouble() + translateNumber;
        coord[1] = coord[1].toDouble() + translateNumber;
        coordinates[i] = coord;
    }

    // Update the geometry object with the translated coordinates
    geometry["coordinates"] = coordinates;

    // Update the feature object with the modified geometry
    result["type"] = feature["type"];
    result["properties"] = feature["properties"];
    result["geometry"] = geometry;

    return result;


}

QJsonObject ModifierClass::scaleModifier(QGeoCoordinate scalePoint, double scaleNumber, QJsonObject feature)
{
    QJsonObject result;
    QJsonObject geometry = feature["geometry"].toObject();
    QString geometryType = geometry["type"].toString();

    // Find the center of the geometry
    QGeoCoordinate center = findCenterOfGeometry(feature);

    // Apply the scaling around the scale point
    if (geometryType == "Point") {
        // Handle point geometry
        QJsonArray coordinates = geometry["coordinates"].toArray();
        double latitude = coordinates[1].toDouble();
        double longitude = coordinates[0].toDouble();

        // Scale the point around the scale point
        QGeoCoordinate scaledPoint = scalePoint.atDistanceAndAzimuth(scalePoint.distanceTo(QGeoCoordinate(latitude, longitude)) * scaleNumber, scalePoint.azimuthTo(QGeoCoordinate(latitude, longitude)));

        // Update the coordinates
        coordinates[0] = scaledPoint.longitude();
        coordinates[1] = scaledPoint.latitude();

        geometry["coordinates"] = coordinates;
    } else if (geometryType == "LineString" || geometryType == "Polygon") {
        // Handle LineString and Polygon geometries
        QJsonArray coordinates = geometry["coordinates"].toArray();

        for (int i = 0; i < coordinates.size(); ++i) {
            QJsonArray linearRing = coordinates[i].toArray();
            for (int j = 0; j < linearRing.size(); ++j) {
                QJsonArray coordinatePair = linearRing[j].toArray();
                double latitude = coordinatePair[1].toDouble();
                double longitude = coordinatePair[0].toDouble();

                // Scale the point around the scale point
                QGeoCoordinate scaledPoint = scalePoint.atDistanceAndAzimuth(scalePoint.distanceTo(QGeoCoordinate(latitude, longitude)) * scaleNumber, scalePoint.azimuthTo(QGeoCoordinate(latitude, longitude)));

                // Update the coordinates
                coordinatePair[0] = scaledPoint.longitude();
                coordinatePair[1] = scaledPoint.latitude();
                linearRing[j] = coordinatePair;
            }
            coordinates[i] = linearRing;
        }
        geometry["coordinates"] = coordinates;
    }

    result["type"] = feature["type"];
    result["properties"] = feature["properties"];
    result["geometry"] = geometry;

    return result;

}




QGeoCoordinate ModifierClass::findCenterOfGeometry(const QJsonObject &feature)
{

    QJsonObject geometry = feature["geometry"].toObject();
    QString geometryType = geometry["type"].toString();

    if(geometryType == "LineString"){

        QJsonArray coordinates = geometry["coordinates"].toArray();

        double totalLat = 0.0 ;
        double totalLon = 0.0 ;

        for(const auto coordinate : coordinates){

            QJsonArray point = coordinate.toArray();
            totalLat += point[1].toDouble();
            totalLon += point[0].toDouble();
        }

        return QGeoCoordinate(totalLat / coordinates.size() ,totalLon / coordinates.size());

    }else if (geometryType == "Polygon") {

        QJsonArray coordinates = geometry["coordinates"].toArray();
        QGeoPolygon myPolygon ;

        for(const auto ring : coordinates){

            for(const auto point : ring.toArray()){
                QJsonArray coords = point.toArray();
                myPolygon.addCoordinate(QGeoCoordinate(coords[1].toDouble(),coords[0].toDouble()));
            }
        }

        return myPolygon.center();

    }else if (geometryType == "Point") {

        QJsonArray coordinates = geometry["coordinates"].toArray();
        return QGeoCoordinate(coordinates[1].toDouble(),coordinates[0].toDouble());
    }

    return QGeoCoordinate();
}
