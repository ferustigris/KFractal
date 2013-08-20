#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "chart.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	QLabel result;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_actionLoad_Image_triggered();

	void on_actionKohe_triggered();

	void on_actionSerpinsky_triggered();

	void on_actionCube_triggered();
	void on_calc(const QString& fileName);

	void on_actionLine_triggered();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
