#include "addlineclass.h"

AddLineClass::AddLineClass()
{

}

void AddLineClass::addLine(QString &filepath, QList<QGeoCoordinate> myCoordinates)
{

    QFile file(filepath) ;
    if(! file.open(QIODevice::ReadWrite|QIODevice::Text)){
        qCritical() << "Faild to Open file" ;
    }

    QJsonObject feature     ;
    QJsonObject geometry    ;
    QJsonObject properties  ;
    QJsonArray coordinates  ;

    feature["type"]  = "Feature"        ;
    feature["properties"] = properties  ;
    geometry["type"] = "LineString"     ;

    for(const QGeoCoordinate& myGeoCoordinates : myCoordinates){

        QJsonArray coordinate ;
        coordinate.append(myGeoCoordinates.longitude()) ;
        coordinate.append(myGeoCoordinates.latitude())  ;
        coordinates.append(coordinate) ;


    }

    geometry["coordinates"] = coordinates ;
    feature["geometry"] = geometry        ;

}
