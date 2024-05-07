#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QStringList>
#include <QGeoCoordinate>
#include <iostream>
#include "geojsonioclass.h"
#include "geographclass.h"

void help();

QVariantMap readArguments(QStringList arguments) ;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Store command line arguments in a QVariantMap type variable named myArguments
    QVariantMap myArguments = readArguments(a.arguments());

    GeoGraphClass &geoGraph = GeoGraphClass::getInstance();


    QString myFilePath;
    int numPoints{0}; // Initialize numPoints to 0

    GeojsonIOClass geojsonIO ;

    if (myArguments.contains("addLine")) {
        if (!myArguments.contains("filePath") || !myArguments.contains("coordinates")) {
            qCritical() << "Missing filePath or coordinates argument";
            return -1;
        }

        QString myFilePath = myArguments["filePath"].toString();
        QStringList coordinateStrings = myArguments["coordinates"].toStringList();

        if (coordinateStrings.length() >= 2) {
            QList<QGeoCoordinate> geoCoordinates;

            for (const QString& coordinateString : coordinateStrings) {
                QStringList coordinateParts = coordinateString.split(",");
                if (coordinateParts.length() == 2) {
                    double lat = coordinateParts[0].toDouble();
                    double lon = coordinateParts[1].toDouble();
                    QGeoCoordinate geoCoord(lat, lon);
                    if (geoCoord.isValid()) {
                        geoCoordinates.append(geoCoord);
                    } else {
                        qCritical() << "Provided coordinate is not valid:" << coordinateString;
                        return -1;
                    }
                } else {
                    qCritical() << "Invalid coordinate format:" << coordinateParts;
                    return -1;
                }
            }

            bool append = myArguments.value("append").toBool() ;

            geoGraph.addLine(myFilePath,geoCoordinates,append);


        } else {
            qCritical() << "Insufficient number of coordinates provided. At least two coordinates are required.";
            return -1;
        }
    } else if (myArguments.contains("addPolygon")) {
        if (!myArguments.contains("filePath") || !myArguments.contains("coordinates")) {
            qCritical() << "Missing filePath or coordinates argument";
            return -1;
        }

        QString myFilePath = myArguments["filePath"].toString();
        QStringList coordinateStrings = myArguments["coordinates"].toString().split(" ", QString::SkipEmptyParts);

        if (coordinateStrings.length() >= 3) { // A polygon requires at least 3 points
            QList<QGeoCoordinate> geoCoordinates;

            // Converting QStringList to QList<QGeoCoordinate>
            for (const QString &coordinateString : coordinateStrings) {
                QStringList coordinateParts = coordinateString.split(",");
                if (coordinateParts.length() == 2) {
                    double lat = coordinateParts[0].toDouble();
                    double lon = coordinateParts[1].toDouble();
                    QGeoCoordinate geoCoord(lat, lon);
                    if (geoCoord.isValid()) {
                        geoCoordinates.append(geoCoord);
                    } else {
                        qCritical() << "Provided coordinate is not valid:" << coordinateString;
                        return -1;
                    }
                } else {
                    qCritical() << "Invalid coordinate format:" << coordinateParts;
                    return -1;
                }
            }

            bool append = myArguments.value("append").toBool() ;


            geoGraph.addPolygon(geoCoordinates,myFilePath,append);


        } else {
            qCritical() << "Insufficient number of coordinates provided. A polygon requires at least 3 points.";
            return -1;
        }
    } else if (myArguments.contains("addCircle")) {
        if (!myArguments.contains("filePath") || !myArguments.contains("center") || !myArguments.contains("radius")) {
            qCritical() << "Missing filePath, center, or radius argument";
            return -1;
        }

        myFilePath = myArguments["filePath"].toString();

        QStringList centerParts = myArguments["center"].toString().split(",");

        if (centerParts.length() != 2) {
            qCritical() << "Invalid center format";
            return -1;
        }

        double lat = centerParts[0].toDouble();
        double lon = centerParts[1].toDouble();
        QGeoCoordinate center(lat, lon);

        double radius = myArguments["radius"].toDouble();

        bool append = myArguments.value("append").toBool() ;

        geoGraph.addCircle(myFilePath,center,radius,append);



    } else if (myArguments.contains("calculateGreatCircle")) {



        if (!myArguments.contains("filePath") || !myArguments.contains("point1") || !myArguments.contains("point2") || !myArguments.contains("numPoints")) {
            qCritical() << "Missing filePath, point1, point2, or numPoints argument";
            return -1;
        }

        myFilePath = myArguments["filePath"].toString();

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

        geoGraph.calculateGreatCircle(myFilePath,point1,point2,numPoints);


    } else if (myArguments.contains("addEllipse")) {
        if (!myArguments.contains("filePath") || !myArguments.contains("center") || !myArguments.contains("semiMajorAxis") || !myArguments.contains("semiMinorAxis") || !myArguments.contains("segments")) {
            qCritical() << "Missing filePath, center, semiMajorAxis, semiMinorAxis, or segments argument";
            return -1;
        }

        myFilePath = myArguments["filePath"].toString();

        QStringList centerParts = myArguments["center"].toString().split(",");

        if (centerParts.length() != 2) {
            qCritical() << "Invalid center format";
            return -1;
        }

        double lat = centerParts[0].toDouble();
        double lon = centerParts[1].toDouble();
        QGeoCoordinate center(lat, lon);

        double semiMajorAxis = myArguments["semiMajorAxis"].toDouble();
        double semiMinorAxis = myArguments["semiMinorAxis"].toDouble();
        int segments = myArguments["segments"].toInt();

        geoGraph.addEllipse(myFilePath,center,semiMajorAxis,semiMinorAxis,segments);
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
}

QVariantMap readArguments(QStringList arguments) {
    QVariantMap argumentsMap;
    bool append = false; // Default to false

    for (int i = 1; i < arguments.length(); ++i) {
        QString argument = arguments[i];

        if (argument == "--help") {
            help();
            return argumentsMap;
        } else if (argument == "--append") {
            append = true; // Set append to true if --append is present
        } else if (argument.startsWith("--")) {
            QString key = argument.mid(2);
            if (i + 1 < arguments.length()) {
                QString value = arguments[i + 1];
                if (value.startsWith("--")) {
                    // If the next argument is another option, assume true for this option
                    argumentsMap[key] = true;
                } else {
                    // If the next argument is not another option, it's a value for this option
                    if (key == "coordinates") {
                        // If the key already exists, append the new coordinate to the existing list
                        if (argumentsMap.contains(key)) {
                            QStringList coordinates = argumentsMap[key].toStringList();
                            coordinates.append(value);
                            argumentsMap[key] = coordinates;
                        } else {
                            // If the key doesn't exist, create a new list with the coordinate
                            QStringList coordinates;
                            coordinates.append(value);
                            argumentsMap[key] = coordinates;
                        }
                    } else {
                        argumentsMap[key] = value;
                    }
                    i++; // Skip the next argument since we've used it as a value
                }
            } else {
                // If there is no next argument, assume true for this option
                argumentsMap[key] = true;
            }
        }
    }

    // Add the append flag to the arguments map
    argumentsMap["append"] = QVariant::fromValue(append);

    qDebug() << "Parsed arguments:";
    for (auto it = argumentsMap.begin(); it != argumentsMap.end(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }

    return argumentsMap;
}
