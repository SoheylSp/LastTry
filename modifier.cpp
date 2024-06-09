#include "modifier.h"
#include "geometry.h"
#include <cmath>

ModifierClass::ModifierClass()
{

}

QJsonObject ModifierClass::rotateModifier(const QGeoCoordinate &rotatePoint, double rotateAngle, const QJsonObject &feature)
{

    QJsonObject modifiedFeature = feature; // Copy the original feature
    QJsonObject geometry = modifiedFeature["geometry"].toObject(); // Get the geometry object
    QString geometryType = geometry["type"].toString(); // Get the geometry type

    // Check if the geometry is a LineString or Polygon
    if (geometryType == "LineString" || geometryType == "Polygon") {
        QJsonArray coordinates = geometry["coordinates"].toArray(); // Get the coordinates array
        for (int i = 0; i < coordinates.size(); i++) {
            QJsonArray coordinateArray = coordinates[i].toArray(); // Get the inner array of coordinates
            for (int j = 0; j < coordinateArray.size(); j++) {
                QJsonArray point = coordinateArray[j].toArray(); // Get the current point
                double latitude = point[1].toDouble(); // Extract the latitude
                double longitude = point[0].toDouble(); // Extract the longitude

                QGeoCoordinate currentPoint(latitude, longitude); // Create a QGeoCoordinate object

                // Calculate the distance and azimuth from the rotatePoint to the currentPoint
                double distance = rotatePoint.distanceTo(currentPoint);
                double azimuth = rotatePoint.azimuthTo(currentPoint);

                // Calculate the new position of the point using the rotateAngle
                QGeoCoordinate newPoint = rotatePoint.atDistanceAndAzimuth(distance, azimuth + rotateAngle);

                // Update the coordinates with the new longitude and latitude
                point[0] = newPoint.longitude();
                point[1] = newPoint.latitude();
                coordinateArray[j] = point; // Update the coordinate array
            }
            coordinates[i] = coordinateArray; // Update the coordinates array
        }
        geometry["coordinates"] = coordinates; // Update the geometry with the rotated coordinates
    }

    modifiedFeature["geometry"] = geometry; // Update the feature with the rotated geometry
    return modifiedFeature; // Return the modified feature

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
    QJsonObject result ;
    QJsonObject geometry = feature["geometry"].toObject();
    QString geometryType = geometry["type"].toString();

    qDebug()<<"Geometry Type is"<<geometryType;

    //Check if the geometry type is supported

    if(geometryType.isEmpty()|| (geometryType != "LineString" && geometryType != "Polygon")){

        qDebug() << "geometry type" << geometryType << "is not supproted" ;

        // if its not supported or missed , return the original feature

        result["type"] = feature["type"] ;
        result["properties"] = feature["properties"] ;
        result["geometry"] = geometry ;
        return result ;

    }

    //check if the geometry has valid coordinates

    QJsonValue coordinatesValue = geometry["coordinates"] ;

    if(! coordinatesValue.isArray()){

        qDebug()<<"Coordinates are not an array";

        //If the geometry has no coordinates or the coordinates are not an array , return the original feature

        result["type"] = feature["type"];
        result["properties"] = feature["properties"];
        result["geometry"] = geometry ;
        return result ;

    }

    QJsonArray coordinates = coordinatesValue.toArray();

    if(coordinates.isEmpty()){

        qDebug()<<"Coordinates array is Empty";

        //If the geometry has no coordinates return the original feature

        result["type"] = feature["type"];
        result["properties"] = feature["properties"];
        result["geometry"] = geometry;
        return result;

    }

    QJsonArray scaledCoordinates ;

    for(const QJsonValue& coordinatesValue : coordinates){

        if(geometryType == "LineString" ){

            QJsonArray coordinatePair = coordinatesValue.toArray();

            if(coordinatePair.size() == 2){

                double latitude = coordinatePair[1].toDouble();
                double longitude = coordinatePair[0].toDouble();

                //Scale the coordinate pair arround the scale point

                double scaledLongitude = scalePoint.longitude() + (longitude - scalePoint.longitude()) * scaleNumber ;
                double scaledLatitude = scalePoint.latitude() + (latitude - scalePoint.latitude()) * scaleNumber ;

                QJsonArray scaledCoordinatePair = {scaledLongitude ,  scaledLatitude} ;
                scaledCoordinates.append(scaledCoordinatePair);
            }else {

                qDebug()<<"Invalid coordinate pair size " << coordinatePair.size();
            }

        }else {

            //Polygon

            QJsonArray linearRing = coordinatesValue.toArray() ;
            QJsonArray scaledLinearRing ;

            for(const QJsonValue& coordinatePairValue : linearRing){

                QJsonArray coordinatePair = coordinatePairValue.toArray();

                if(coordinatePair.size() == 2){

                    double latitude = coordinatePair[1].toDouble();
                    double longitude = coordinatePair[0].toDouble();

                    //Scale the coordinate pair arround the scale point

                    double scaledLongitude = scalePoint.longitude() + (longitude - scalePoint.longitude()) * scaleNumber;
                    double scaledLatitude = scalePoint.latitude() + (latitude = scalePoint.latitude()) * scaleNumber ;

                    QJsonArray scaledCoordinatePair = {scaledLongitude,scaledLatitude} ;

                    scaledLinearRing.append(scaledCoordinatePair);




                }else {

                    qCritical()<<"Invalid coordinate pair size : " << coordinatePair.size();
                }

            }

            scaledCoordinates.append(scaledLinearRing) ;
        }

    }

    //Update the geometry with the scaled coordinates

    geometry["coordiantes"] = scaledCoordinates ;

    result["type"] = feature["type"] ;
    result["properties"] = feature["properties"] ;
    result["geometry"] = geometry ;

    return result ;

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
