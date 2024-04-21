#ifndef CALCULATEGREATCIRCLECLASS_H
#define CALCULATEGREATCIRCLECLASS_H

#include <QGeoCoordinate>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

class CalculateGreatCircleClass
{
public:
    CalculateGreatCircleClass();

    void calculateGreatCircle(const QGeoCoordinate point1 , const QGeoCoordinate point2 ,int numPoints);

    void writeGeojsonToFile(QString& filePath);
private:

    QJsonArray features_ ;

};

#endif // CALCULATEGREATCIRCLECLASS_H
