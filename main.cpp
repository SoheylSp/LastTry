#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QStringList>
#include <QGeoCoordinate>
#include <iostream>
#include "addlineclass.h"
#include "addpolygonclass.h"
#include "addcircleclass.h"
#include "calculategreatcircleclass.h"

void help();

QVariantMap readArguments(QStringList arguments) ;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Store command line arguments in a QVariantMap type variable named myArguments

    QVariantMap myArguments = readArguments(a.arguments());

    QString myFilepath ;
    int numPoints{0};//Initialize numPoints to 0


    if(myArguments.contains("addLine")){
        if(!myArguments.contains("filePath")){
            qCritical() << "Missing file path argument";
            return -1;
        }

        myFilepath = myArguments["filePath"].toString();

        if(myArguments.contains("coordinates")){

            QVariantList coordinates = myArguments["coordinates"].toList();

            if(coordinates.length() >=2){

                QList<QGeoCoordinate> geoCoordinates ;

                //Converting QVariantList to to QList<QGeoCoordinate>

                for(QVariant coordinateString:coordinates){

                    QStringList coordinateParts = coordinateString.toString().split(",");
                    if(coordinateParts.length() == 2){

                        double lat = coordinateParts[0].toDouble();
                        double lon = coordinateParts[1].toDouble();

                        QGeoCoordinate geoCoord(lat,lon);
                        if(geoCoord.isValid()){

                            geoCoordinates.append(geoCoord) ;

                        }else {

                            qCritical()<<"Provided coordinate is not valid"<<coordinateString ;
                            return -1 ;
                        }
                    }else{

                        qCritical() << "Invalid coordinate format" << coordinateParts ;
                        return -1 ;


                    }

                }



                AddLineClass addLineObject ;
                addLineObject.addLine(myFilepath,geoCoordinates);

            }

        }

    }else if (myArguments.contains("addPolygon")) {


        if(! myArguments.contains("filePath")){

            qCritical()<<"Missing filePath argments";
            return -1 ;
        }

        myFilepath = myArguments["filePath"].toString();

        if(myArguments.contains("numPoints")){

            numPoints=myArguments["numPoints"].toInt() ;//Get the number of points for the polygon

        }else {

            qCritical()<<"Missing numPoints argument";
            return -1 ;
        }

        if(myArguments.contains("coordinates")){

            QVariantList coordinates = myArguments["coordinates"].toList() ;

            if(coordinates.length() >= numPoints){

                QList<QGeoCoordinate> geoCoordinates ;

                //Converting QVariantList to QList<QGeoCoordinate>

                for(QVariant coordinateString:coordinates){

                    QStringList coordinateParts = coordinateString.toString().split(",");
                    if(coordinateParts.length() == 2){

                        double lat = coordinateParts[0].toDouble() ;
                        double lon = coordinateParts[1].toDouble() ;

                        QGeoCoordinate geoCoord(lat,lon);
                        if(geoCoord.isValid()){
                            geoCoordinates.append(geoCoord) ;
                        }else {

                            qCritical()<<"Provided coordinatesis not valid " << coordinateString;
                            return -1 ;
                        }

                    }else {

                        qCritical()<<"Invalid coordinate format"<<coordinateParts;
                    }

                }

                AddPolyGonClass addPolygonObject ;
                addPolygonObject.addPolygon(numPoints,geoCoordinates,myFilepath) ;

            }else {

                qCritical()<<"Insufficient number of coordinates provided" ;
                return -1 ;
            }

        }


    }else if (myArguments.contains("addCircle")) {

        if(!myArguments.contains("filePath") || !myArguments.contains("center")){

            qCritical()<<"Missing filePath , center or radius argument" ;
            return -1 ;
        }

        myFilepath = myArguments["filePath"].toString() ;

        QStringList centerParts = myArguments["center"].toString().split(",") ;

        if(centerParts.length() != 2){

            qCritical()<<"Invalid center format";
            return -1 ;

        }

        double lat = centerParts[0].toDouble() ;
        double lon = centerParts[1].toDouble() ;

        QGeoCoordinate center(lat,lon);

        double radius = myArguments["radius"].toDouble() ;

        AddCircleClass addCircleObject ;
        addCircleObject.addCircle(myFilepath,center,radius) ;


    }else if (myArguments.contains("calculateGreatCircle")) {
        if (!myArguments.contains("filePath") || !myArguments.contains("point1") || !myArguments.contains("point2") || !myArguments.contains("numPoints")) {
            qCritical() << "Missing filePath, point1, point2, or numPoints argument";
            return -1;
        }

        myFilepath = myArguments["filePath"].toString();

        QGeoCoordinate point1, point2;

        QStringList point1Coordinates = myArguments["point1"].toString().split(",");
        QStringList point2Coordinates = myArguments["point2"].toString().split(",");

        if (point1Coordinates.length() != 2 || point2Coordinates.length() != 2) {
            qCritical() << "Invalid point format for point1 or point2";
            return -1;
        }

        double lat1 = point1Coordinates[0].toDouble();
        double lon1 = point1Coordinates[1].toDouble();
        point1.setLatitude(lat1);
        point1.setLongitude(lon1);

        double lat2 = point2Coordinates[0].toDouble();
        double lon2 = point2Coordinates[1].toDouble();
        point2.setLatitude(lat2);
        point2.setLongitude(lon2);

        numPoints = myArguments["numPoints"].toInt();

        CalculateGreatCircleClass greatCircleObj;
        greatCircleObj.calculateGreatCircle(point1, point2, numPoints);
        greatCircleObj.writeGeojsonToFile(myFilepath);

    }




    return a.exec();
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

    std::cout <<"To generate geojson format file to shape a polygon use this command:\n\n";
    std::cout <<"./executableName --filePath /path/to/output.geojson --addPolygon --numPoint n <an integer> --coordinates 12.3,34.4<Coordinates of point1> ... --coordinates87.5,23.4 <Coordinates of point n> \n\n";
    std::cout <<"NOTE : you must add --coordinates as much as numPoints numbers ,eg if the numPoints is = 5 , you must add 5 --coordinates with 5 pair of double numbers .\n\n ";
    std::cout <<"NOTE : Use your Own numPoints & coordinates \n\n";


    std::cout<<"*********************************************************************************\n\n";

    std::cout<<"To generate geojson format file to calculate the great circle between two points use this command:\n\n";
    std::cout << "./executableName --filePath /path/to/output.geojson --calculateGreatCircle --point1 43.34,54.5 --point2 65.5,34.5 --numPoints 30 \n\n" ;
    std::cout<<"NOTE:Use your own executableName , your Own coordinates for point1 & point2 ,your own numPoints and your own path/to/output.geojson \n\n";

}

QVariantMap readArguments(QStringList arguments){

    QVariantMap argumentsMap ;

    if(arguments.contains("--help")){

        help();

        return argumentsMap ;


    }


    for(int i = 1 ; i < arguments.length();++i){

        QString argument = arguments[i] ;

        if(argument.startsWith("--")){

            auto key = argument.mid(2) ;
            auto valueIndex = i + 1 ;

            if(argument.length()>valueIndex){
                auto value = arguments[valueIndex] ;
                if(value.startsWith("--")){

                    argumentsMap[key] = true ;

                }else {

                    i++ ;
                    if(argumentsMap.contains(key)){

                        auto list = argumentsMap[key].toList();
                        if(list.isEmpty()){
                            list.append(argumentsMap[key]);
                        }
                        list.append(value) ;
                        argumentsMap[key] = list ;


                    }else {

                        argumentsMap[key] = value ;
                    }
                }

            }

        }

    }

    return argumentsMap ;

};
