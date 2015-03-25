#ifndef DATAY_H
#define DATAY_H

#include <stdlib.h>
#include <math.h>
#include <QApplication>
#include <QWidget>

class DataY
{
    public :

    DataY(u_int64_t sizeVec);
    ~DataY();

    QVector<double> * getData(void);
    void addData(double data);

    private :

    QVector<double> * Vdata;
    QVector<double>::iterator dataIter;
    const u_int64_t sizeData;
    u_int64_t filling;
};

class Vect_DataY
{
    public :
    Vect_DataY(u_int8_t channel, u_int64_t size);

    private
    QVector<Datay> channel_Data;

};

#endif // DATAY_H
