#ifndef CURVE_H
#define CURVE_H
#include <QColor>
#include <QList>
#include <QPoint>

class Curve
{
	QList<QPointF> points;
	QColor color;
	QString title;
public:
	Curve(QColor color, const QString&title);
	QColor getColor() const;
	QList<QPointF> getPoints();
	void addPoint(double x, double y);
	QString getTitle() const;
};

#endif // CURVE_H
