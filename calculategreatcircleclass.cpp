#include "calculategreatcircleclass.h"

CalculateGreatCircleClass::CalculateGreatCircleClass()
{

features_=QJsonArray();

}

void CalculateGreatCircleClass::calculateGreatCircle(const QGeoCoordinate point1, const QGeoCoordinate point2, int numPoints)
{

    //Clearin features_ array before populationg it with new intermediate points

    features_= QJsonArray() ;

    for(int i = 1 ; i <= numPoints; ++i){

        double f = static_cast<double>(i)/numPoints ;
        double distance = point1.distanceTo(point2) ;
        double azimuth = point1.azimuthTo(point2)   ;

        QGeoCoordinate interMediatePoint = point1.atDistanceAndAzimuth(distance * f,azimuth) ;

        QJsonObject feature ;
        QJsonObject properties ;
        QJsonObject geometry ;
        QJsonArray coordinates ;

        feature["type"] = "Feature" ;
        feature["properties"] = properties ;
        geometry["type"] = "Point" ;
        coordinates.append(interMediatePoint.longitude())   ;
        coordinates.append(interMediatePoint.latitude())    ;

        geometry["coordinates"] = coordinates ;
        feature["geometry"] = geometry ;

        features_.append(feature) ;

    }

}

void CalculateGreatCircleClass::writeGeojsonToFile(QString &filePath)
{

    geojsonIO.writeFeaturesToFile(features_,filePath) ;

}


