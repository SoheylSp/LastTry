#ifndef MODIFIERCLASS_H
#define MODIFIERCLASS_H

#include <QJsonObject>
#include <QGeoCoordinate>
#include <QGeoPolygon>


class ModifierClass
{
public:
    ModifierClass();

    static QJsonObject rotateModifier(double rotateAngle , const QJsonObject &feature);

private:

    static QGeoCoordinate findCenterOfGeometry(const QJsonObject& feature);

};

#endif // MODIFIERCLASS_H
