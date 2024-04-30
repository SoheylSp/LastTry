#include "addellipseclass.h"
#include <cmath>

AddEllipseClass::AddEllipseClass()
{
}

void AddEllipseClass::addEllipse(const QString &filePath, const QGeoCoordinate &center, double semiMajorAxis, double semiMinorAxis, double rotation, int numPoints)
{
    QJsonObject ellipseFeature = createEllipseFeature(center, semiMajorAxis, semiMinorAxis, rotation, numPoints);
    geojsonIO.writeFeatureToFile(ellipseFeature, filePath);
}

QJsonObject AddEllipseClass::createEllipseFeature(const QGeoCoordinate &center, double semiMajorAxis, double semiMinorAxis, double rotation, int numPoints)
{
    QJsonObject feature;
    QJsonObject properties;
    QJsonObject geometry;
    QJsonArray coordinates;

    feature["type"] = "Feature";
    properties["name"] = "Ellipse";
    feature["properties"] = properties;

    QJsonArray outerRing;
    for (int i = 0; i <= 360; i += 360 / numPoints) {
        double angle = i * M_PI / 180;
        double x = semiMajorAxis * cos(angle);
        double y = semiMinorAxis * sin(angle);

        // Apply rotation
        double rotatedX = x * cos(rotation * M_PI / 180) - y * sin(rotation * M_PI / 180);
        double rotatedY = x * sin(rotation * M_PI / 180) + y * cos(rotation * M_PI / 180);

        QGeoCoordinate point = center.atDistanceAndAzimuth(sqrt(rotatedX * rotatedX + rotatedY * rotatedY), atan2(rotatedY, rotatedX) * 180 / M_PI);
        outerRing.append(QJsonArray{ point.longitude(), point.latitude() });
    }

    geometry["type"] = "Polygon";
    geometry["coordinates"] = QJsonArray({ outerRing });
    feature["geometry"] = geometry;

    return feature;
}
