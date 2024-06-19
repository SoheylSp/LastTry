#ifndef MODIFIERCLASS_H
#define MODIFIERCLASS_H

#include <QJsonObject>
#include <QGeoCoordinate>
#include <QGeoPolygon>


class ModifierClass
{
public:
    ModifierClass();

    static QJsonObject rotateModifier(const QGeoCoordinate &rotatePoint, double rotateAngle, const QJsonObject &feature);

    static QJsonObject rotateModifier2(const QGeoCoordinate &rotateCenter , double rotateAngle , const QJsonObject &feature);

    static QJsonObject translatorModifier(double translateNumber,QJsonObject feature);

    static QJsonObject scaleModifier(QGeoCoordinate scalePoint, double scaleNumber, QJsonObject feature);

    static QGeoCoordinate findCenterOfGeometry(const QJsonObject& feature);

};

#endif // MODIFIERCLASS_H
