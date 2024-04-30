#ifndef ADDELLIPSECLASS_H
#define ADDELLIPSECLASS_H

#include <QGeoCoordinate>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <cmath>
#include "geojsonioclass.h"

class AddEllipseClass
{

public:

    AddEllipseClass();

    void addEllipse(const QString &filePath, const QGeoCoordinate &center, double semiMajorAxis, double semiMinorAxis, double rotation, int numPoints);
       QJsonObject createEllipseFeature(const QGeoCoordinate &center, double semiMajorAxis, double semiMinorAxis, double rotation, int numPoints);
       GeojsonIOClass geojsonIO;
};

#endif // ADDELLIPSECLASS_H
