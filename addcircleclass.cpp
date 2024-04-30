#include "addcircleclass.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <cmath>

AddCircleClass::AddCircleClass()
{

}

void AddCircleClass::addCircle(const QString &filePath, const QGeoCoordinate &center, double radius)
{

  QJsonObject circleFeature = createCircleFeature(center,radius);
  geojsonIO.writeFeatureToFile(circleFeature,filePath);

}

QJsonObject AddCircleClass::createCircleFeature(const QGeoCoordinate &center, double radius)
{
    QJsonObject feature;
        QJsonObject properties;
        QJsonObject geometry;
        QJsonArray coordinates;

        feature["type"] = "Feature";
        properties["name"] = "Circle";
        feature["properties"] = properties;

        QJsonArray outerRing;
        for (int i = 0; i <= 360; i += 10) {
            QGeoCoordinate point = center.atDistanceAndAzimuth(radius, i);
            outerRing.append(QJsonArray{ point.longitude(), point.latitude() });
        }

        geometry["type"] = "Polygon";
        geometry["coordinates"] = QJsonArray({ outerRing });
        feature["geometry"] = geometry;

        return feature;
}




