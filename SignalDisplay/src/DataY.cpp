#include "DataY.h"

DataY::DataY(u_int64_t sizeVec) :  sizeData(sizeVec)
{
    Vdata = new QVector<double>(sizeData);
    dataIter = Vdata->begin();
    filling = 0;
}

QVector<double> * DataY::getData(void)
{
    return Vdata;
}

void DataY::addData(double data)
{
    if (dataIter != Vdata->end())
    {
        *dataIter = data;
        dataIter++;
        filling++;
    }
}

Vect_DataY::Vect_DataY(u_int8_t channel, u_int64_t size)
{

}
