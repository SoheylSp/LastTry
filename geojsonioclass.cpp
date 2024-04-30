#include "geojsonioclass.h"

GeojsonIOClass::GeojsonIOClass()
{

}

void GeojsonIOClass::writeFeatureToFile(const QJsonObject &feature, const QString &filePath, bool append)
{

    QJsonObject geojson ;
    geojson["type"] = "FeatureCollection" ;
    QJsonArray features ;
    features.append(feature) ;
    geojson["features"] = features;

    writeFeatureToFile(geojson,filePath,append) ;

}

void GeojsonIOClass::writeFeaturesToFile(const QJsonArray &features, const QString &filePath, bool append)
{

    QJsonObject geojson ;
    geojson["type"] = "FeatureCollection" ;
    geojson["features"] = features ;

    writeGeojsonToFile(geojson,filePath,append);

}

void GeojsonIOClass::writeGeojsonToFile(const QJsonObject &geojson, const QString &filePath, bool append)
{
    QFile file(filePath);
    QIODevice::OpenMode openMode = append ? QIODevice::ReadWrite : QIODevice::WriteOnly;

    if(! file.open(openMode|QIODevice::Text)){


        qCritical()<<"Faild to open file";
        return ;

    }

    QJsonDocument doc ;

    if(append && file.size() > 0){

        //Read the existing data

        QByteArray jsonData = file.readAll() ;
        doc = QJsonDocument::fromJson(jsonData) ;

        //Append the new feature(s) to the existing FeatureCollection

        QJsonObject existingGeojson = doc.object();
        QJsonArray existingFeatures = existingGeojson["features"].toArray();
        existingFeatures.append(geojson["features"].toArray()) ;
        existingGeojson["features"] = existingFeatures;
        doc.setObject(existingGeojson) ;

    }else {

        //Create a new featureCollection

        doc.setObject(geojson) ;
}

    //Write the updated data back to file

    file.resize(0) ;
    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

    qint64 bytesWritten = file.write(jsonData);
    if(bytesWritten == -1){
        qCritical() << "Faild to write to file :" << file.errorString() ;

    }else {

        qDebug()<<"Wrote " << bytesWritten << "bytes to file" ;
}

    file.close();

}
