#include "addpolygonclass.h"

AddPolyGonClass::AddPolyGonClass()
{

}

void AddPolyGonClass::addPolygon(int numPoints, QList<QGeoCoordinate> pointsList, const QString &filepath)
{
    if (pointsList.size() != numPoints) {
        qCritical() << "Number of points does not match the specified numPoints";
        return;
    }

    QJsonObject feature;
    QJsonObject properties;
    QJsonObject geometry;
    QJsonArray coordinates;

    feature["type"] = "Feature";
    feature["properties"] = properties;
    geometry["type"] = "Polygon";

    QJsonArray polygonCoordinates;
    for (const auto &point : pointsList) {
        polygonCoordinates.append(QJsonArray{point.longitude(), point.latitude()});
    }
    // Closing the polygon by adding the first point again
    polygonCoordinates.append(polygonCoordinates.first()); // Add the first point again to close the polygon

    coordinates.append(polygonCoordinates);
    geometry["coordinates"] = coordinates;
    feature["geometry"] = geometry;

    QJsonObject geojson;
    geojson["type"] = "FeatureCollection";
    QJsonArray features;
    features.append(feature);
    geojson["features"] = features;

    // Write the geojson data to the file
    writeGeojsonToFile(geojson, filepath);
}

void AddPolyGonClass::writeGeojsonToFile(const QJsonObject &geojson, const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file for writing:" << file.errorString();
        return;
    }

    QJsonDocument doc(geojson);
    qint64 bytesWritten = file.write(doc.toJson());
    if (bytesWritten == -1) {
        qCritical() << "Failed to write to file:" << file.errorString();
    } else {
        qDebug() << "Wrote" << bytesWritten << "bytes to file";
    }

    file.close();
}
