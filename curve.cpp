#include "curve.h"

Curve::Curve(QColor color, const QString&title)
{
	this->color = color;
	this->title = title;
}
QColor Curve::getColor() const
{
	return color;
}
QString Curve::getTitle() const
{
	return title;
}
QList<QPointF> Curve::getPoints()
{
	return points;
}

void Curve::addPoint(double x, double y)
{
	points.append(QPointF(x, y));
}
