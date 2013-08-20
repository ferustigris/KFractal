#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QList>
#include "curve.h"

class Chart : public QWidget
{
	Q_OBJECT
	QList<Curve> curves;
	QString xTitle;
	QString yTitle;
public:
	explicit Chart(QWidget *parent = 0);
	void addCurve(const Curve& curve);
	void setTitle(const QString& xTitle, const QString& yTitle);
	virtual void paintEvent ( QPaintEvent * event );
signals:

public slots:

};

#endif // CHART_H
