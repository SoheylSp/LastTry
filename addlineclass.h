#ifndef ADDLINECLASS_H
#define ADDLINECLASS_H

#include <QGeoCoordinate>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QStringList>
#include <QDebug>
class AddLineClass
{
public:
    AddLineClass();

    void addLine(QString& filepath ,QList<QGeoCoordinate> coordinates);
};

#endif // ADDLINECLASS_H
