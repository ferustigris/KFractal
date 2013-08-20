#include "chart.h"
#include <QPainter>
#include <QPaintEvent>

Chart::Chart(QWidget *parent) :
    QWidget(parent)
{
	xTitle = "x";
	yTitle = "y";
}
void Chart::paintEvent ( QPaintEvent * event )
{
	#warning: сложно больно
	QPainter painter(this);
	painter.setBrush(QBrush(Qt::white));
	painter.drawRect(event->rect());
	double minY = 1000000, minX = 1000000, maxX = 0, maxY = 0;
	double xOffset = this->width()*0.1;
	double yOffset = this->height()*0.1;
	foreach(Curve curve, curves)
	{
		foreach(QPointF point, curve.getPoints())
		{
			minX = qMin(point.x(), minX);
			minY = qMin(point.y(), minY);
			maxX = qMax(point.x(), maxX);
			maxY = qMax(point.y(), maxY);
		}
	}
	double scaleX = (this->width())/maxX * 0.8;
	double scaleY = (this->height())/maxY * 0.8;
	foreach(Curve curve, curves)
	{
		painter.setBrush(QBrush(curve.getColor()));
		foreach(QPointF point, curve.getPoints())
		{
			QPoint p(point.x()*scaleX + xOffset, (maxY-point.y())*scaleY + yOffset);
			painter.drawEllipse(p, 2, 2);
		}
	}

	QString maximumX(QString::number(maxX));
	QString maximumY(QString::number(maxY));
	QString maximumX2(QString::number(maxX/2));
	QString maximumY2(QString::number(maxY/2));

	maxX = qMax(0, (int)(maxX*scaleX));
	maxY = qMax(0, (int)(maxY*scaleY));
	minX = qMin(-0, (int)(minX*scaleX));
	minY = qMin(-0, (int)(minY*scaleY));

	painter.setBrush(QBrush(Qt::black));
	painter.drawLine(xOffset, maxY + yOffset, xOffset, minY + yOffset);
	painter.drawLine(maxX + xOffset, maxY + yOffset, minX + xOffset, maxY + yOffset);

	painter.drawText(maxX + xOffset, maxY + yOffset - 20, maximumX);
	painter.drawEllipse(maxX + xOffset, maxY + yOffset, 3, 3);
	painter.drawText(maxX + xOffset + 10, maxY + yOffset, xTitle);

	painter.drawText(xOffset, -10 + yOffset, yTitle);
	painter.drawText(xOffset + 20, yOffset + 10, maximumY);
	painter.drawEllipse(xOffset, yOffset, 3, 3);

	painter.drawText(maxX/2 + xOffset, maxY + yOffset - 20, maximumX2);
	painter.drawEllipse(maxX/2 + xOffset, maxY + yOffset, 3, 3);

	painter.drawText(xOffset + 20, maxY/2 + yOffset + 10, maximumY2);
	painter.drawEllipse(xOffset, maxY/2 + yOffset, 3, 3);

	painter.drawText(xOffset - 20, maxY + yOffset + 20, "(0,0)");

	int y = 20;
	foreach(Curve curve, curves)
	{
		painter.setBrush(QBrush(curve.getColor()));
		painter.drawRect(maxX, y, 10, 10);
		painter.setBrush(QBrush(Qt::black));
		painter.drawText(maxX + 15, y + 10, curve.getTitle());
		y += 20;
	}

}
void Chart::addCurve(const Curve& curve)
{
	curves.append(curve);
}
void Chart::setTitle(const QString& xTitle, const QString& yTitle)
{
	this->xTitle = xTitle;
	this->yTitle = yTitle;
}
