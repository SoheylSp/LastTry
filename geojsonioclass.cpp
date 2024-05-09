#include "geojsonioclass.h"

GeojsonIOClass::GeojsonIOClass()
{
}

void GeojsonIOClass::writeFeaturesToFile(const QJsonArray& features, const QString& filePath, bool append)
{

    QFile file(filePath);
    QIODevice::OpenMode openMode = append ? QIODevice::ReadWrite | QIODevice::Text : QIODevice::WriteOnly | QIODevice::Text;

    if (!file.open(openMode)) {
        qCritical() << "Failed to open file:" << filePath << ", error:" << file.errorString();
        return;
    }

    QJsonDocument doc;
    if (append && file.size() > 0) {
        // Read the existing GeoJSON data and append the new features
        QByteArray existingData = file.readAll();
        doc = QJsonDocument::fromJson(existingData);
        QJsonObject geojson = doc.object();
        QJsonArray existingFeatures = geojson["features"].toArray();
        for (const QJsonValue &feature : features) {
            existingFeatures.append(feature);
        }
        geojson["features"] = existingFeatures;
        doc.setObject(geojson);
    } else {
        // Create a new GeoJSON object with the features
        QJsonObject geojson;
        geojson["type"] = "FeatureCollection";
        geojson["features"] = features;
        doc.setObject(geojson);

    }

    // Move the file pointer to the beginning if appending
    if (append) {
        file.seek(0);
    }

    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

    qint64 bytesWritten = file.write(jsonData);
    if (bytesWritten == -1) {
        qCritical() << "Failed to write file:" << filePath << ", error:" << file.errorString();
    } else {
        qDebug() << "Wrote" << bytesWritten << "bytes to file:" << filePath;
    }

    file.close();


}

void GeojsonIOClass::writeGeojsonToFile(const QJsonObject &geojson, const QString &filePath)
{

    QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qCritical() << "Failed to open file:" << filePath << ", error:" << file.errorString();
            return;
        }

        QJsonDocument jsonDoc(geojson);
        file.write(jsonDoc.toJson());
        file.flush();
        file.close();
}

QJsonObject GeojsonIOClass::readGeojsonFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file:" << filePath << ", error:" << file.errorString();
        return QJsonObject();
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    if (jsonDoc.isObject()) {
        return jsonDoc.object();
    } else {
        qCritical() << "File at" << filePath << "does not contain valid GeoJSON data.";
        return QJsonObject();
    }
}



