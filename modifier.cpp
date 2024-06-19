#include "modifier.h"
#include "geometry.h"
#include <cmath>

ModifierClass::ModifierClass()
{

}

QJsonObject ModifierClass::rotateModifier(const QGeoCoordinate &rotatePoint, double rotateAngle, const QJsonObject &feature)
{

    QJsonObject rotatedFeature = feature;
     if (!feature.contains("geometry")) {
         return rotatedFeature; // Return the original feature if it doesn't contain geometry
     }

     QJsonObject geometry = feature["geometry"].toObject();
     QString geometryType = geometry["type"].toString();

     // Rotate geometries
     if (geometryType == "Polygon" || geometryType == "LineString") {
         QJsonArray coordinates;

         if (geometryType == "Polygon") {
             // For the polygon we need to iterate over the first level of coordinates
             QJsonArray polygonCoordinates = geometry["coordinates"].toArray();
             QJsonArray newPolygonCoordinates;

             for (const QJsonValue &polyCoordinates : polygonCoordinates) {
                 QJsonArray newCoordinates;
                 coordinates = polyCoordinates.toArray();

                 for (const QJsonValue &coord : coordinates) {
                     QJsonArray pointCoordinates = coord.toArray();
                     double longitude = pointCoordinates[0].toDouble();
                     double latitude = pointCoordinates[1].toDouble();

                     // Calculate the distance and azimuth from the rotate point to the coordinate
                     QGeoCoordinate originalCoord;
                     originalCoord.setLatitude(latitude);
                     originalCoord.setLongitude(longitude);
                     double distance = rotatePoint.distanceTo(originalCoord);
                     double azimuth = rotatePoint.azimuthTo(originalCoord);

                     // Add the rotateAngle to the azimuth
                     double rotatedAzimuth = azimuth + rotateAngle ;

                     // Calculate the new coordinate
                     QGeoCoordinate rotatedCoord = rotatePoint.atDistanceAndAzimuth(distance,rotatedAzimuth);

                     // Update the coordinate
                     newCoordinates.append(rotatedCoord.latitude()); // Corrected order
                     newCoordinates.append(rotatedCoord.longitude()); // Corrected order
                 }
                 newPolygonCoordinates.append(newCoordinates);
             }
             QJsonObject newGeometry;
             newGeometry["type"] = "Polygon";
             newGeometry["coordinates"] = newPolygonCoordinates;
             rotatedFeature["geometry"] = newGeometry;

         } else if (geometryType == "LineString") {
             // For LineString we directly access the coordinates
             coordinates = geometry["coordinates"].toArray();
             QJsonArray newCoordinates;

             for (const QJsonValue &coord : coordinates) {
                 QJsonArray pointCoordinates = coord.toArray();
                 double longitude = pointCoordinates[0].toDouble();
                 double latitude = pointCoordinates[1].toDouble();

                 // Calculate the distance and initial azimuth from the rotate Point to the coordinate
                 QGeoCoordinate originalCoord(latitude, longitude);
                 double distance = rotatePoint.distanceTo(originalCoord);
                 double azimuth = rotatePoint.azimuthTo(originalCoord);

                 // Add the rotation angle to the azimuth
                 double rotatedAzimuth = azimuth +  rotateAngle;

                 // Calculate the new coordinate
                 QGeoCoordinate rotatedCoord = rotatePoint.atDistanceAndAzimuth(distance, rotatedAzimuth);

                 // Update the coordinates
                 newCoordinates.append(rotatedCoord.latitude()); // Corrected order
                 newCoordinates.append(rotatedCoord.longitude()); // Corrected order
             }

             QJsonObject newGeometry;
             newGeometry["type"] = "LineString";
             newGeometry["coordinates"] = newCoordinates;
             rotatedFeature["geometry"] = newGeometry;
         }
     }

     return rotatedFeature;

}

QJsonObject ModifierClass::rotateModifier2(const QGeoCoordinate &rotateCenter, double rotateAngle, const QJsonObject &feature)
{

    QJsonObject modifiedFeature = feature ;

   // qDebug()<<"FEATURE : : : " << feature ;

    QJsonObject modifiedGeometry = modifiedFeature["geometry"].toObject();

   // qDebug()<<"GEOMETRY : : : " << modifiedGeometry ;


    QJsonArray modifiedCoords = modifiedGeometry["coordinates"].toArray();



   for (int i = 0; i < modifiedCoords.size(); ++i)
      {
          QJsonArray actualPointArray = modifiedCoords[i].toArray();

          QGeoCoordinate actualPoint;
          actualPoint.setLatitude(actualPointArray[1].toDouble());

          actualPoint.setLongitude(actualPointArray[0].toDouble());

          double distance , azimuth , calAzimuth ;

          distance = rotateCenter.distanceTo(actualPoint);
          azimuth = rotateCenter.azimuthTo(actualPoint) ;
          calAzimuth = azimuth + rotateAngle ;

          QGeoCoordinate calculatedPoint = rotateCenter.atDistanceAndAzimuth(distance,calAzimuth);

          QJsonArray newPointArray ;
          newPointArray.append(calculatedPoint.longitude());
          newPointArray.append(calculatedPoint.latitude()) ;

          modifiedCoords[i] = newPointArray;

        //  qDebug()<<"ModifiedCoords Are  : L LL  :" << modifiedCoords;

      }

   modifiedGeometry["coordinates"] = modifiedCoords ;
   modifiedFeature["geometry"] = modifiedGeometry ;


   return modifiedFeature ;

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

    qDebug() << "Geometry Type is" << geometryType;

    // Check if the geometry type is supported
    if (geometryType.isEmpty() || (geometryType != "LineString" && geometryType != "Polygon")) {
        qDebug() << "geometry type" << geometryType << "is not supported";

        // If it's not supported or missed, return the original feature
        result["type"] = feature["type"];
        result["properties"] = feature["properties"];
        result["geometry"] = geometry;
        return result;
    }

    // Check if the geometry has valid coordinates
    QJsonValue coordinatesValue = geometry["coordinates"];

    if (!coordinatesValue.isArray()) {
        qDebug() << "Coordinates are not an array";

        // If the geometry has no coordinates or the coordinates are not an array, return the original feature
        result["type"] = feature["type"];
        result["properties"] = feature["properties"];
        result["geometry"] = geometry;
        return result;
    }

    QJsonArray coordinates = coordinatesValue.toArray();

    if (coordinates.isEmpty()) {
        qDebug() << "Coordinates array is Empty";

        // If the geometry has no coordinates return the original feature
        result["type"] = feature["type"];
        result["properties"] = feature["properties"];
        result["geometry"] = geometry;
        return result;
    }

    QJsonArray scaledCoordinates;

    for (const QJsonValue& coordinatesValue : coordinates) {
        if (geometryType == "LineString") {
            QJsonArray coordinatePair = coordinatesValue.toArray();

            if (coordinatePair.size() == 2) {
                double latitude = coordinatePair[1].toDouble();
                double longitude = coordinatePair[0].toDouble();

                // Scale the coordinate pair around the scale point
                double scaledLongitude = scalePoint.longitude() + (longitude - scalePoint.longitude()) * scaleNumber;
                double scaledLatitude = scalePoint.latitude() + (latitude - scalePoint.latitude()) * scaleNumber;

                QJsonArray scaledCoordinatePair = {scaledLongitude, scaledLatitude};
                scaledCoordinates.append(scaledCoordinatePair);
            } else {
                qDebug() << "Invalid coordinate pair size " << coordinatePair.size();
            }
        } else {
            // Polygon
            QJsonArray linearRing = coordinatesValue.toArray();
            QJsonArray scaledLinearRing;

            for (const QJsonValue& coordinatePairValue : linearRing) {
                QJsonArray coordinatePair = coordinatePairValue.toArray();

                if (coordinatePair.size() == 2) {
                    double latitude = coordinatePair[1].toDouble();
                    double longitude = coordinatePair[0].toDouble();

                    // Scale the coordinate pair around the scale point
                    double scaledLongitude = scalePoint.longitude() + (longitude - scalePoint.longitude()) * scaleNumber;
                    double scaledLatitude = scalePoint.latitude() + (latitude - scalePoint.latitude()) * scaleNumber;

                    QJsonArray scaledCoordinatePair = {scaledLongitude, scaledLatitude};
                    scaledLinearRing.append(scaledCoordinatePair);
                } else {
                    qCritical() << "Invalid coordinate pair size: " << coordinatePair.size();
                }
            }

            scaledCoordinates.append(scaledLinearRing);
        }
    }

    // Update the geometry with the scaled coordinates
    geometry["coordinates"] = scaledCoordinates;

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
