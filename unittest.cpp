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

void UnitTest::testOfRotation(QJsonArray originalCoords, QJsonArray rotatedCoords, QGeoCoordinate rotatePoint ,double rotateAngle)
{

    QJsonArray orgCoords = originalCoords;
    QJsonArray rotCoords = rotatedCoords;

    qDebug() << "Original Coordinates are: " << orgCoords << "\n";
    qDebug() << "Rotated Coordinates are: " << rotCoords << "\n";

    const double tolerance = 1e-6; // Define a tolerance level for floating-point comparison

    for (int i = 0; i < orgCoords.size(); ++i) {
        QJsonArray orgPoint = orgCoords[i].toArray();
        QJsonArray rotPoint = rotCoords[i].toArray();

        double orgLat = orgPoint[1].toDouble();
        double orgLon = orgPoint[0].toDouble();

        double rotLat = rotPoint[1].toDouble();
        double rotLon = rotPoint[0].toDouble();

        QGeoCoordinate originalPointCoords(orgLat, orgLon);
        QGeoCoordinate rotatedPointCoords(rotLat, rotLon);

        double azimuthOriginalToRotated = rotatePoint.azimuthTo(rotatedPointCoords);
        double azimuthRotatedToOriginal = rotatePoint.azimuthTo(originalPointCoords);

        double calculatedAngle = azimuthOriginalToRotated - azimuthRotatedToOriginal;

        // Normalize the calculated angle
        if (calculatedAngle > 180.0) {
            calculatedAngle -= 360.0;
        } else if (calculatedAngle < -180.0) {
            calculatedAngle += 360.0;
        }

        // Normalize the rotateAngle
        double normalizedRotationAngle = rotateAngle;
        if (normalizedRotationAngle > 180.0) {
            normalizedRotationAngle -= 360.0;
        } else if (normalizedRotationAngle < -180.0) {
            normalizedRotationAngle += 360.0;
        }

        // Use tolerance for comparison
        if (std::abs(calculatedAngle - normalizedRotationAngle) > tolerance) {
            qCritical() << "Point " << i << " IS NOT rotated correctly\n";
        } else {
            qDebug() << "Point " << i << " is rotated CORRECTLY\n";
        }
    }

}

void UnitTest::testOfRotation2(QJsonArray orgCoords, QJsonArray rotatedCoords, QGeoCoordinate rotatedPoint, double rotateAngle)
{

    const double tolerance = 1e-6 ;

    for(int i = 0 ; i < orgCoords.size() ; ++i)
    {

        QJsonArray orgPointArray = orgCoords[i].toArray();
        QJsonArray rotPointArray = rotatedCoords[i].toArray();

        qDebug()<< "Org Point Array Size Is : " << orgPointArray.size()<<"\n";

        qDebug()<< "Rot Point Array Size Is : " << rotPointArray.size()<<"\n";


        if(orgPointArray.size() != rotPointArray.size()){

            qCritical()<<"Invalid rotPointArray size" << "\n";
            return ;
        }

        double orgLat , orgLon , rotLat , rotLon ;

        orgLat = orgPointArray[1].toDouble();
        orgLon = orgPointArray[0].toDouble();

        rotLat = rotPointArray[1].toDouble() ;
        rotLon = rotPointArray[0].toDouble();

        QGeoCoordinate orgPoint , rotPoint ,rotatePoint;

        orgPoint.setLatitude(orgLat);
        orgPoint.setLongitude(orgLon);

        rotPoint.setLatitude(rotLat);
        rotPoint.setLongitude(rotLon);

        rotatePoint.setLatitude(rotatedPoint.latitude());
        rotatePoint.setLongitude(rotatedPoint.longitude());

        qDebug()<<"Orginal point is : " << orgPoint <<"\n";
        qDebug()<<"Rotated Point is : "<<rotPoint<<"\n";
        qDebug()<<"Center of rotatin is : " << rotatePoint<<"\n";

        double distance , azimuth , calAzimuth ;
        QGeoCoordinate expectedRotatePoint;

        distance = rotatePoint.distanceTo(orgPoint);
        azimuth = rotatePoint.azimuthTo(orgPoint);
        calAzimuth = azimuth + rotateAngle ;

        expectedRotatePoint = rotatePoint.atDistanceAndAzimuth(distance,calAzimuth);

        qDebug()<<"Expected Rotate Point is : " << expectedRotatePoint << "\n";

        if(std::abs(expectedRotatePoint.latitude()-rotPoint.latitude())>tolerance||std::abs(expectedRotatePoint.longitude() - rotPoint.longitude()) > tolerance){

            qCritical()<<"Point" << i << "IS NOT rotated correctly\n";

        }else {

            qDebug()<<"Point"<<i<<"roteted CORRECTLY\n";

        }

    }
}

void UnitTest::testOfScal(QJsonArray originalCoords, QJsonArray scaledCoords, QGeoCoordinate scalePoint, double scaleNumber)
{
    QJsonArray orgCoords = originalCoords ;
    QJsonArray sclCoords = scaledCoords ;

    qDebug()<<"Original Coords are : " <<  orgCoords << " \n";
    qDebug()<<"Scaled Coords are : "<< scaledCoords << "\n" ;

    for(int i = 0 ; i < orgCoords.size() ; ++i){

        QJsonArray orgPoint = orgCoords[i].toArray();
        QJsonArray sclPoint = sclCoords[i].toArray();

        double orgLat = orgPoint[1].toDouble();
        double orgLon = orgPoint[0].toDouble();

        double sclLat = sclPoint[1].toDouble() ;
        double sclLon = sclPoint[0].toDouble();

        //Calculate the expected scaled coordiantes

        double expectedScaledLon = scalePoint.longitude() + (orgLon - scalePoint.longitude()) * scaleNumber ;
        double expectedScaledLat = scalePoint.latitude()+ (orgLat - scalePoint.latitude()) * scaleNumber ;

        //Comparing

        if(expectedScaledLon != sclLon || expectedScaledLat != sclLat){

            qCritical()<<"Point" << i  << "IS NOT scaled correctly\n";

        }else {

            qDebug()<<"Point" << i << "is scaled CORRECTLY\n";
        }



    }
}
