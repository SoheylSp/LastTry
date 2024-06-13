#include "unittest.h"

UnitTest::UnitTest()
{

}

QJsonArray UnitTest::getGeometryCoords(QJsonObject feature)
{

    QJsonObject testfeature = feature ;

    if(testfeature.isEmpty()){

        qCritical()<<"Feature object is empty .";
        return QJsonArray();
    }



    QJsonObject testGeometry = feature["geometry"].toObject();


    if(testGeometry.isEmpty()){

        qCritical()<<"Geometry object is empty";
        return QJsonArray();

    }



    QJsonArray testCoordinates = testGeometry["coordinates"].toArray() ;

    if(testCoordinates.isEmpty()){

        qCritical()<<"Coordinates array is empty.";
        return QJsonArray() ;
    }

    //Get the geometery type from the geometry





    qDebug()<<"Coordinates are : " << testCoordinates;

    return testCoordinates ;


}

void UnitTest::testOfTranslate(QJsonArray originalCoords , QJsonArray translatedCoords , double translatrNumber)
{

    QJsonArray orgCoords = originalCoords ;
    QJsonArray trsCoords = translatedCoords;

    qDebug()<<"orgCoords is :" << orgCoords <<"\n";

    qDebug()<<"trsCoords is :" << trsCoords <<"\n";



    QJsonArray orgPoint1 , orgPoint2 , trsPoint1 , trsPoint2 ;

    orgPoint1 = orgCoords[0].toArray();
    orgPoint2 = orgCoords[1].toArray();

    trsPoint1 = trsCoords[0].toArray();
    trsPoint2 = trsCoords[1].toArray();

    double orgPoint1Lat , orgPoint1Lon , orgPoint2Lat , orgPoint2Lon;

    orgPoint1Lat = orgPoint1[1].toDouble();
    orgPoint1Lon = orgPoint1[0].toDouble();

    orgPoint2Lat = orgPoint2[1].toDouble();
    orgPoint2Lon = orgPoint2[0].toDouble();

    double trsPoint1Lat , trsPoint2Lat , trsPoint1Lon , trsPoint2Lon ;

    trsPoint1Lat = trsPoint1[1].toDouble();
    trsPoint1Lon = trsPoint1[0].toDouble();
    trsPoint2Lat = trsPoint2[1].toDouble();
    trsPoint2Lon = trsPoint2[0].toDouble();

    if(orgPoint1Lat + translatrNumber != trsPoint1Lat){

        qCritical()<<" first point latitude IS NOT translated correctly \n ";
    }else {

        qDebug()<<"first point latitude is translated CORRECTLLY\n";
    }


    if(orgPoint1Lon + translatrNumber != trsPoint1Lon){

        qCritical()<<" first point longitude IS NOT translated correctly \n ";

    }else {

        qDebug()<<"first point longitude is translated CORRECTLLY\n";
    }

    if(orgPoint2Lat + translatrNumber != trsPoint2Lat){


        qCritical()<<" second point latitude IS NOT translated correctly \n ";
    }else {

        qDebug()<<"second point latitude is translated CORRECTLLY\n";
    }

    if(orgPoint2Lon + translatrNumber != trsPoint2Lon){

        qCritical()<<" second point longitude IS NOT translated correctly \n ";

    }else {

        qDebug()<<"second point longitude is translated CORRECTLLY\n";

    }


}
