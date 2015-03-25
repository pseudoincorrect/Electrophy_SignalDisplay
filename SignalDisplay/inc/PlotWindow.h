#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H
#include <stdlib.h>
#include <math.h>
#include <qendian.h>

#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QDebug>
#include "qcustomplot.h"
#include "DataY.h"

class PlotWindow : public QWidget
{
    Q_OBJECT

    public :
        PlotWindow();
        void changeDataWindow(int plage, int maxPlage);
        void clearAll();

    public slots :
        void displayFile();
        void displayLiveStream(qint64 length, u_int16_t *resultingData);

    public :
        QVector<QCustomPlot *> customPlot;

    signals :
        void showSlider();

    private slots :
        void xAxisScaleChanged();

    private :
        QFile destinationFile;
        QFile inputFile;
        QByteArray fileData;
        u_int16_t* shortData;
        double numberSample;
        qint64 prev_len;

};

#endif // PLOTWINDOW_H
