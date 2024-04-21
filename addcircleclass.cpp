#include "addcircleclass.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <cmath>

AddCircleClass::AddCircleClass()
{

}

void AddCircleClass::addCircle(const QString &filePath, const QGeoCoordinate &center, double radius)
{

    QJsonObject circleFeature = createCircleFeature(center,radius);
    writeGeojsonToFile(circleFeature,filePath);

}

QJsonObject AddCircleClass::createCircleFeature(const QGeoCoordinate &center, double radius)
{
    QJsonObject feature ;
    QJsonObject properties ;
    QJsonObject geometry ;
    QJsonArray coordinates ;

    feature["type"] = "Feature" ;
    feature["properties"] = properties ;
    geometry["type"] = "Polygon" ;

    for(int i = 0 ; i < 360 ; i+=10){

        QGeoCoordinate point = center.atDistanceAndAzimuth(radius,i);
        coordinates.append(QJsonArray{point.longitude(),point.latitude()}) ;

    }

    //Close the circle by adding the first point again

    coordinates.append(coordinates.at(0));

    geometry["coordinates"] = QJsonArray({coordinates});
    feature["geometry"]= geometry ;

    return feature ;
}

void AddCircleClass::writeGeojsonToFile(const QJsonObject &geojson, const QString &filePath)
{
    QFile file(filePath);
    if(! file.open(QIODevice::ReadWrite | QIODevice::Text)){

        qCritical()<<"faild to write file : " << file.errorString();
        return ;


    }

    QJsonDocument doc(geojson);
    qint64 bytesWritten = file.write(doc.toJson()) ;
    if(bytesWritten == -1){
        qCritical()<<"Faild to write to file";
    }else {

        qDebug()<<"wrote"<<bytesWritten<<"bytes to file";
}

    file.close() ;
}


