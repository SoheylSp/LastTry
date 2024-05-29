#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QStringList>
#include <QGeoCoordinate>
#include <iostream>
#include "geojsonio.h"
#include "geograph.h"
#include "modifier.h"
#include<QRegularExpression>

QList<QGeoCoordinate> parseCoordinates(const QVariantMap& arguments);
QVariantMap readArguments(QStringList arguments) ;
void help();

int main(int argc , char *argv[])
{


    QCoreApplication a(argc,argv);
    GeojsonIOClass geojsonIO;
    GeoGraphClass geoGraph ;

    //Store command line arguments in a QVariantMap type variable named myArguments

    QVariantMap myArguments = readArguments(a.arguments());


    QString myFilePath = myArguments["filePath"].toString();
    bool append = myArguments.contains("append") ;
    bool fileExist = false;
    if(append == true && QFile::exists(myFilePath)){

        fileExist = true ;
    }

    //bool fileExist = QFile::exists(myFilePath);

    QJsonObject existingGeojson ;
    QJsonArray existingFeatures ;

    if(fileExist){

        //File Exist , read the existing geojson data
        existingGeojson = geojsonIO.readGeojsonFromFile(myFilePath);
        existingFeatures = existingGeojson["features"].toArray();
    }else {

        //File does not exist , create a new Geojson object

        existingGeojson = QJsonObject();
        existingGeojson["type"] = "FeatureCollection";
        existingFeatures = QJsonArray();
    }


    //Create a new feature based on user's command line arguments

    QJsonObject newFeature ;



    if(myArguments.contains("addLine")){


        if(myArguments.contains("coordinates")){


            QList<QGeoCoordinate> coordinates = parseCoordinates(myArguments);
            newFeature = geoGraph.createLineFeature(coordinates);

            //existingFeatures.append(newFeature);

        }


    }else if (myArguments.contains("addPolygon")) {

        if(myArguments.contains("coordinates")){

            QList<QGeoCoordinate> coordinates =parseCoordinates(myArguments);
            newFeature = geoGraph.createPolygonFeature(coordinates);

            //existingFeatures.append(newFeature);

        }

    }else if (myArguments.contains("addCircle")) {

        if(myArguments.contains("center")){

            QGeoCoordinate center  = geojsonIO.getCircleCenterFromArguments(myArguments);
            double radius = myArguments["radius"].toDouble();
            newFeature = geoGraph.createCircleFearute(center,radius);

           // existingFeatures.append(newFeature);
        }

    }else if (myArguments.contains("calculateGreatCircle")) {




        QGeoCoordinate point1 , point2 ;

        if(myArguments.contains("point1") && myArguments.contains("point2")){

            QStringList point1Coordinates = myArguments["point1"].toString().split(",");
            QStringList point2Coordinates = myArguments["point2"].toString().split(",");

            if(point1Coordinates.length() ==2 && point2Coordinates.length()==2){

                double lat1 = point1Coordinates[0].toDouble();
                double lon1 = point1Coordinates[1].toDouble();
                double lat2 = point2Coordinates[0].toDouble();
                double lon2 = point2Coordinates[1].toDouble();

                point1.setLatitude(lat1);
                point1.setLongitude(lon1);

                point2.setLatitude(lat2);
                point2.setLongitude(lon2);

            }else {

                qCritical()<<"Invalis point format for point1 and point2";
                return -1 ;
            }


        }else {

            qCritical()<<"Missing point1 or point2 arguments";
            return -1 ;
        }


        int numPoints = myArguments["numPoints"].toInt();
        newFeature = geoGraph.createGreatCircleFeature(point1,point2,numPoints);

    }else if (myArguments.contains("addEllipse")) {


        QGeoCoordinate center;
        if (myArguments.contains("center")) {
            QStringList centerParts = myArguments["center"].toString().split(",");
            if (centerParts.length() == 2) {
                double lat = centerParts[0].toDouble();
                double lon = centerParts[1].toDouble();
                center.setLatitude(lat);
                center.setLongitude(lon);
            } else {
                qCritical() << "Invalid center format";
                return -1;
            }
        }

        double semiMajorAxis = myArguments["semiMajorAxis"].toDouble();
        double semiMinorAxis = myArguments["semiMinorAxis"].toDouble();
        int segments = myArguments["segments"].toInt();

        newFeature = geoGraph.createEllipseFeature(center,semiMajorAxis,semiMinorAxis,segments);
    }else if(myArguments.contains("addRectangle")){

        if(!myArguments.contains("filePath") ||! myArguments.contains("center") || !myArguments.contains("width") || ! myArguments.contains("height")){

            qCritical()<<"Missing argument , filePath , center , width or height does not exist\n";

        }

        QGeoCoordinate center ;
        QStringList centerParts ;
        QString widthStr = myArguments["width"].toString();
        QString heightStr = myArguments["height"].toString();

        double width = 0.0 ;
        double height = 0.0;
        centerParts = myArguments["center"].toString().split(",");

        if(centerParts.length() == 2){

            double lat = centerParts[0].toDouble();
            double lon = centerParts[1].toDouble();

            center.setLatitude(lat);
            center.setLongitude(lon);

        }else {

            qCritical()<<"Invalidd Coordinates format";
        }


        //check the "width" is meter or kilometer or radian

        if(myArguments["width"].toString().endsWith("km") && myArguments["height"].toString().endsWith("km")){


            QRegularExpression replaceStr ("km");

            width = widthStr.replace(QString("km"),QString("")).toDouble();
            height = heightStr.replace(QString("km"),QString("")).toDouble();


        }else if(myArguments["width"].toString().endsWith("m") && myArguments["height"].toString().endsWith("m")){

            QRegularExpression replaceStr ("m") ;

            width = widthStr.replace(QString("m"),QString("")).toDouble();
            height = heightStr.replace(QString("m"),QString("")).toDouble();

        }else if (myArguments["width"].toString().endsWith("deg") && myArguments["height"].toString().endsWith("deg")) {


            //QRegularExpression replaceStr ("deg");

            width =widthStr.replace(QString("deg"),QString("")).toDouble();
            height = heightStr.replace(QString("deg"),QString("")).toDouble();


            newFeature = geoGraph.creatRectangleFeatureWithDeg(center,width,height);

        }



        newFeature = geoGraph.createRectangleFeature(center,width,height);
    }


    //Modify the newly created feature if needed

    if (myArguments.contains("rotate") && myArguments.contains("rotatePoint")) {

        if(!myArguments.contains("rotateAngle")){

            qCritical()<<"a rotateAngle needed\n";

        }

        QStringList rotatePointParts = myArguments["rotatePoint"].toString().split(",");
        QGeoCoordinate rotatePoint ;

        if(rotatePointParts.length() == 2){

            double rpLat = rotatePointParts[0].toDouble();
            double rpLon = rotatePointParts[1].toDouble();

            rotatePoint.setLatitude(rpLat);
            rotatePoint.setLongitude(rpLon);

        }






        double rotateAngle = myArguments["rotateAngle"].toDouble();

        newFeature = ModifierClass::rotateModifier(rotatePoint,rotateAngle,newFeature);

    }else if (myArguments.contains("rotate") && !myArguments.contains("rotatePoint")) {

        if(! myArguments.contains("rotateAngle")){
            qCritical()<<"rotateAngle needed";
        }

        QGeoCoordinate center = ModifierClass::findCenterOfGeometry(newFeature);
        double rotateAngle = myArguments["rotateAngle"].toDouble();
        newFeature = ModifierClass::rotateModifier(center,rotateAngle,newFeature);

    }
    if (myArguments.contains("translate")) {
        double translateNumber = myArguments["translateNumber"].toDouble();

        newFeature = ModifierClass::translatorModifier(translateNumber ,newFeature);
    }
    if (myArguments.contains("scale")) {

        if(myArguments.contains("scalePoint") && myArguments.contains("scaleNumber")){
            double scaleNumber = myArguments["scaleNumber"].toDouble();
            QGeoCoordinate scalePoint ;
            QStringList scalePointParts = myArguments["scalePoint"].toString().split(",");

            if(scalePointParts.length() == 2){

                double spLat = scalePointParts[0].toDouble();
                double spLon = scalePointParts[1].toDouble();

                scalePoint.setLatitude(spLat);
                scalePoint.setLongitude(spLon);

            }
            newFeature = ModifierClass::scaleModifier(scalePoint ,scaleNumber, newFeature);
        }else if (myArguments.contains("scaleNumber") && ! myArguments.contains("scalePoint")) {


            double scaleNumber = myArguments["scaleNumber"].toDouble();
            QGeoCoordinate scalePoint = ModifierClass::findCenterOfGeometry(newFeature);
            newFeature = ModifierClass::scaleModifier(scalePoint,scaleNumber,newFeature);
        }

    }


    // Add the newly created (and possibly modified) feature to the existing features
    existingFeatures.append(newFeature);

    // Write the updated features to the GeoJSON file
    existingGeojson["features"] = existingFeatures;
    geojsonIO.writeGeojsonToFile(existingGeojson, myFilePath);

}

QVariantMap readArguments(QStringList arguments) {

    QVariantMap argumentsMap;

    for (int i = 1; i < arguments.length(); ++i) {
        QString argument = arguments[i];

        if (argument == "--help") {
            help();
            return argumentsMap;
        } else if (argument.startsWith("--")) {
            QString key = argument.mid(2);
            if (i + 1 < arguments.length()) {
                QString value = arguments[i + 1];
                if (value.startsWith("--")) {
                    // If the next argument is another option, assume true for this option
                    argumentsMap[key] = true;
                } else {
                    // If the next argument is not another option, it's a value for this option
                    argumentsMap[key] = value;
                    i++; // Skip the next argument since we've used it as a value
                }
            } else {
                // If there is no next argument, assume true for this option
                argumentsMap[key] = true;
            }
        }
    }

    return argumentsMap;

}

void help(){

    std::cout << " \n\n" ;

    std::cout  <<"To generate a geojson to shape a line use this command:\n\n";
    std::cout <<"./executableName --filePath /path/to/output.geojson --addLine --coordinates 32.3,12.3 --coordinate 76.5,54.5\n\n";
    std::cout <<"NOTE : use you'r own executabeName , coordinates & pathes\n";
    std::cout <<"\n\n";


    std::cout <<"*********************************************************************************\n";

    std::cout <<"To generate a geojson file to shape a circle use this command : \n";
    std::cout <<"./executableName --filePath /path/to/output.geojson --addCircle --center 43.2,23.4 --radius 100000\n\n";
    std::cout <<"NOTE : use you'r own coordinates , pathes & radius\n\n";

    std::cout <<"*********************************************************************************\n\n";

    std::cout << "To generate geojson format file to shape a polygon use this command:\n\n";
    std::cout << "./executableName --filePath /path/to/output.geojson --addPolygon --coordinates \"lat1,lon1 lat2,lon2 lat3,lon3 ... latN,lonN\"\n\n";
    std::cout << "NOTE: You must provide at least three coordinate pairs for a polygon. Separate each pair with a space.\n";
    std::cout << "NOTE: Use your own coordinates and path/to/output.geojson.\n\n";



    std::cout<<"*********************************************************************************\n\n";

    std::cout<<"To generate geojson format file to calculate the great circle between two points use this command:\n\n";
    std::cout << "./executableName --filePath /path/to/output.geojson --calculateGreatCircle --point1 43.34,54.5 --point2 65.5,34.5 --numPoints 30 \n\n" ;
    std::cout<<"NOTE:Use your own executableName , your Own coordinates for point1 & point2 ,your own numPoints and your own path/to/output.geojson \n\n";

    std::cout<<"***************************************************************************************\n\n";

    std::cout<<"To generate the geojson format file to shape an ellipse use this command:\n\n";
    std::cout<<"./executableName --filePath /path/to/output.geojson --addEllipse --center latitude,longitude --semiMajorAxis value<in double type> --semiMinorAxis value<in double type>";

    std::cout<<"**********************************************************************************************************\n\n";

    std::cout << "To translate the features in a GeoJSON file, use this command:\n\n";
    std::cout << "./executableName --filePath /path/to/input.geojson --translate --additionalLat 0.5 --additionalLon 1.0\n\n";
    std::cout << "This will add 0.5 to the latitude and 1.0 to the longitude of all the coordinates in the GeoJSON file.\n\n";

}

QList<QGeoCoordinate> parseCoordinates(const QVariantMap& arguments)
{

    QList<QGeoCoordinate> coordinates;

     if (arguments.contains("coordinates")) {
         QVariant coordinatesVar = arguments["coordinates"];
         if (coordinatesVar.type() == QVariant::String) {
             // The coordinates are provided as a single string with multiple coordinate pairs separated by spaces
             QString coordinateStr = coordinatesVar.toString();
             QStringList coordinatePairs = coordinateStr.split(" "); // Split by space
             for (const QString& pair : coordinatePairs) {
                 QStringList parts = pair.split(","); // Split by comma
                 if (parts.length() == 2) {
                     bool latOk, lonOk;
                     double lat = parts[0].toDouble(&latOk);
                     double lon = parts[1].toDouble(&lonOk);
                     if (latOk && lonOk) {
                         coordinates.append(QGeoCoordinate(lat, lon));
                     } else {
                         qCritical() << "Invalid coordinate value:" << pair;
                     }
                 } else {
                     qCritical() << "Unexpected number of parts in coordinate pair:" << pair;
                 }
             }
         } else if (coordinatesVar.canConvert<QVariantList>()) {
             // The coordinates are provided as a list of coordinate pairs
             QVariantList coordinatesList = coordinatesVar.toList();
             for (const QVariant& coordinateVar : coordinatesList) {
                 if (coordinateVar.canConvert<QVariantList>()) {
                     // The coordinate is provided as a list of latitude and longitude
                     QVariantList coordinateParts = coordinateVar.toList();
                     if (coordinateParts.length() == 2) {
                         bool latOk, lonOk;
                         double lat = coordinateParts[0].toDouble(&latOk);
                         double lon = coordinateParts[1].toDouble(&lonOk);
                         if (latOk && lonOk) {
                             coordinates.append(QGeoCoordinate(lat, lon));
                         } else {
                             qCritical() << "Invalid coordinate value in coordinates list:" << coordinateVar;
                         }
                     } else {
                         qCritical() << "Unexpected number of parts in coordinate list:" << coordinateVar;
                     }
                 } else {
                     qCritical() << "Coordinate value is not a list:" << coordinateVar;
                 }
             }
         } else {
             qCritical() << "Coordinates value is not a list or string:" << coordinatesVar;
         }
     } else {
         qCritical() << "Coordinates key not found in arguments map.";
     }

     return coordinates;

}
