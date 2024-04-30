#include "addpolygonclass.h"

AddPolygonClass::AddPolygonClass() {
    // Constructor implementation
}

void AddPolygonClass::addPolygon(const QList<QGeoCoordinate> &pointsList, const QString& filePath) {

    QJsonObject polygonFeature = createPolygonFeature(pointsList);
    geojsonIO.writeFeatureToFile(polygonFeature,filePath);

}

QJsonObject AddPolygonClass::createPolygonFeature(const QList<QGeoCoordinate> &pointsList) {

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

        return feature;
}
