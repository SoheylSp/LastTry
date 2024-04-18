#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QStringList>
#include <QGeoCoordinate>
#include "addlineclass.h"

QVariantMap readArguments(QStringList arguments){

    QVariantMap argumentsMap ;


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


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Store command line arguments in a QVariantMap type variable named myArguments

    QVariantMap myArguments = readArguments(a.arguments());

    QString myFilepath ;


    if(myArguments.contains("addLine")){
        if(!myArguments.contains("filePath")){
            qCritical() << "Missing file path argument";
            return -1;
        }

       myFilepath = myArguments["filepath"].toString();

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

}



    return a.exec();
}
