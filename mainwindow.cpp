#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QtCore/qmath.h>
#include <QMessageBox>

QColor getNextColor()
{
    static int number = 0;
    number %= 6;
    switch(number++) {
    case 0: return Qt::blue;
    case 1: return Qt::yellow;
    case 2: return Qt::red;
    case 3: return Qt::green;
    case 4: return Qt::darkGreen;
    case 5: return Qt::darkRed;
    }
}

class Cube {
    QImage image;
    double bound;
public:
    Cube(const int bound, const QImage& image)
    {
        this->image = image;
        this->bound = bound;
    }
    double getAverageBright()
    {
        double summ = 0;
        for(int i = 0; i < image.width(); i++)
        {
            for(int k = 0; k < image.height(); k++)
            {
                summ += qGray(image.pixel(i, k));
                //qDebug()<<"qGray(image.pixel(i, k))="<<qGray(image.pixel(i, k));
            }
        }
        summ /= (image.width()*image.height());
        //qDebug()<<"sum="<<summ;
        return summ;
    }
    bool isBright()
    {
        return bound > getAverageBright();
    }
};
class ImageDimension : public QImage {
    QList<Cube> cubes;
    double e;
public:
    ImageDimension(const QPixmap file, const double e) : QImage(file.toImage())
    {
        this->e = e;
        int averageBright = Cube(0, copy()).getAverageBright();
        for(int i = 0; i < width(); i += e)
        {
            for(int k = 0; k < height(); k += e)
            {
                cubes.append(Cube(averageBright, copy(i, k, e, e)));
            }
        }
    }
    double getLogN()
    {
        int count = 0;
        foreach(Cube cube, cubes)
        {
            count += cube.isBright();
        }
        //qDebug()<<"count="<<count;
        if(count <= 0)throw(QString("count <= 0"));
        return qLn(count);
    }
    double getLogE()
    {
        if(e <= 0)throw(QString("e <= 0"));
        return qLn(e/qMin(width(), height()));
    }

};

class Dimension {
    double precision;
    QPixmap pix;
public:
    Dimension(const QPixmap& file, const double precision = 0.001) : pix(file)
    {
        this->precision = precision;
    }

    double getD(const QString& title, Chart* chart)
    {
        double averageD = 0;
        int count = 0;
        double lastD = 0;
        double lastLogN = 0;
        double lastLogE = 0;
        Curve curve(getNextColor(), title);
        bool firstTime = true;
        for(double e = qMin(pix.width(), pix.height())/10; e >= 1; e -= 1)
        {
            ImageDimension imageDimension(pix, e);
            try {
                double logN = imageDimension.getLogN();
                double logE = imageDimension.getLogE();
                if(lastLogE - logE == 0)throw ("lastLogE - logE == 0");
                //qWarning()<<"lastLogN="<<lastLogN<<"logN="<<logN;
                //qWarning()<<"lastLogE="<<lastLogE<<"logE="<<logE;
                //if(lastLogN > logN)continue;
                //double d = -((lastLogN - logN)/(lastLogE - logE));
                double d = -logN/logE;
                lastLogN = logN;
                lastLogE = logE;
                lastD = d;
                if(firstTime)
                {
                    firstTime = false;
                    continue;
                }
                averageD += d;
                count++;
                qWarning()<<"result="<<d<<",e="<<e<<",ln(N)="<<logN<<",ln(e)="<<logE;
                qApp->processEvents();
                curve.addPoint(qAbs(logN), qAbs(logE));
                //if(qAbs(lastD - d) < precision)break;
            } catch (QString e) {
                qCritical()<<e;
            }
        }
#warning: артефакты с размерностью >2 и отрицательной размерностью.
        if(count)
            averageD /= count;
        qWarning()<<"averageD="<<averageD;
        chart->addCurve(curve);
        chart->repaint();
        return averageD;
    }
};

//###########################################################################
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->addWidget(&result);
    ui->chart->setTitle("ln(N)", "ln(e)");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_calc(const QString& fileName)
{
    QPixmap file(fileName);
    qDebug()<<fileName;
/*
    QImage img = file.toImage();
    img = img.convertToFormat(QImage::Format_Mono);
    file = file.fromImage(img);
*/
    this->setEnabled(false);

    QLabel* l = new QLabel(fileName);
    ui->images->addWidget(l);
    l = new QLabel(fileName);
    l->setScaledContents(true);
    l->setMaximumSize(200, 200);
    l->setPixmap(file);
    ui->images->addWidget(l);

    Dimension dim(file);
    double d = dim.getD(fileName, ui->chart);
    qWarning()<<"finish result="<<d;
    result.setText(QString("d=%1").arg(d));

    this->setEnabled(true);
}

void MainWindow::on_actionLoad_Image_triggered()
{
    on_calc(QFileDialog::getOpenFileName(this, tr("Load image")));
}

void MainWindow::on_actionKohe_triggered()
{
    on_calc(":/fractals/koh");
}

void MainWindow::on_actionSerpinsky_triggered()
{
    on_calc(":/fractals/serp");
}

void MainWindow::on_actionCube_triggered()
{
    on_calc(":/fractals/cube");
}

void MainWindow::on_actionLine_triggered()
{
    on_calc(":/fractals/line");
}
