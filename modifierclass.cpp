#include "modifierclass.h"
#include "geographclass.h"

ModifierClass::ModifierClass()
{

}

QJsonObject ModifierClass::rotateModifier(double rotateAngle, const QJsonObject &feature)
{

    QJsonObject modifiedFeature = feature;
    QJsonObject geometry = feature["geometry"].toObject();
    QString geometryType = geometry["type"].toString();

    QGeoCoordinate center = findCenterOfGeometry(feature);

    if (geometryType == "LineString") {
        QJsonArray coordinates = geometry["coordinates"].toArray();
        for (auto coordinate : coordinates) {
            QJsonArray point = coordinate.toArray();
            double longitude = point[0].toDouble();
            double latitude = point[1].toDouble();
            QGeoCoordinate geoCoord(latitude, longitude);
            geoCoord = geoCoord.atDistanceAndAzimuth(geoCoord.distanceTo(center), geoCoord.azimuthTo(center) + rotateAngle);
            point[0] = geoCoord.longitude();
            point[1] = geoCoord.latitude();
        }
        geometry["coordinates"] = coordinates;
    } else if (geometryType == "Polygon") {
        QJsonArray coordinates = geometry["coordinates"].toArray();
        for (auto ring : coordinates) {
            QJsonArray points = ring.toArray();
            for (auto point : points) {
                QJsonArray coords = point.toArray();
                double longitude = coords[0].toDouble();
                double latitude = coords[1].toDouble();
                QGeoCoordinate geoCoord(latitude, longitude);
                geoCoord = geoCoord.atDistanceAndAzimuth(geoCoord.distanceTo(center), geoCoord.azimuthTo(center) + rotateAngle);
                coords[0] = geoCoord.longitude();
                coords[1] = geoCoord.latitude();
            }
        }
        geometry["coordinates"] = coordinates;
    } else if (geometryType == "Point") {
        QJsonArray coordinates = geometry["coordinates"].toArray();
        double longitude = coordinates[0].toDouble();
        double latitude = coordinates[1].toDouble();
        QGeoCoordinate geoCoord(latitude, longitude);
        geoCoord = geoCoord.atDistanceAndAzimuth(geoCoord.distanceTo(center), geoCoord.azimuthTo(center) + rotateAngle);
        coordinates[0] = geoCoord.longitude();
        coordinates[1] = geoCoord.latitude();
        geometry["coordinates"] = coordinates;
    }

    modifiedFeature["geometry"] = geometry;
    return modifiedFeature;
}

QGeoCoordinate ModifierClass::findCenterOfGeometry(const QJsonObject &feature)
{
    QJsonObject geometry= feature["geometry"].toObject();
    QString geometryType = geometry["type"].toString();

    if(geometryType == "LineString" ){

        QJsonArray coordinates = geometry["coordinates"].toArray();
        double totalLat = 0.0 ;
        double totalLon = 0.0;

        for(const auto coordinate:coordinates){

            QJsonArray point = coordinate.toArray();
            totalLat+= point[1].toDouble();
            totalLon+=point[0].toDouble();


        }

        return QGeoCoordinate(totalLat/coordinates.size(),totalLon/coordinates.size());

    }else if (geometryType == "Polygon" ) {

        QJsonArray coordinates = geometry["coordinates"].toArray();
        QList<QGeoCoordinate> points ;
        for(const auto ring : coordinates){

            for(const auto point:ring.toArray()){

                QJsonArray coords = point.toArray();
                points.append(QGeoCoordinate(coords[1].toDouble(),coords[0].toDouble()));
            }

        }
        return GeoGraphClass::getInstance().calculatePolygonCenter(points);

    }else if(geometryType == "Point"){

        QJsonArray coordinates = geometry["coordinates"].toArray();
        return QGeoCoordinate(coordinates[1].toDouble(),coordinates[0].toDouble());

    }

    return QGeoCoordinate();
}
