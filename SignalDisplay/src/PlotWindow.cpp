#include "PlotWindow.h"
#include "math.h"
#include "QTime"

const u_int32_t sizeDataWindow = 100000;
const u_int8_t NB_CHANNEL = 8;

PlotWindow::PlotWindow()
{
    prev_len =0;
    shortData = new u_int16_t;

    QVector<double> vectX;
    vectX.resize(1000);
    QVector<double> vectY;
    vectY.resize(1000);

    for(u_int8_t channel=0; channel< NB_CHANNEL; channel++){

        customPlot.push_back(new QCustomPlot);
        customPlot.resize(channel+1);
        // give the axes some labels:
        customPlot[channel]->xAxis->setLabel(QString("sample"));
        customPlot[channel]->yAxis->setLabel(QString("channel %1").arg(channel));
        // set axes ranges, so we see all data:
        customPlot[channel]->xAxis->setRange(-10, 1010);
        customPlot[channel]->yAxis->setRange(-2000, 2000);
        customPlot[channel]->axisRect()->setupFullAxesBox(true);
        customPlot[channel]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

        for(double i=0; i<1000; i++)
        {
            if (channel<4)
                vectY[i]= pow((i-500),(double) channel);
            else
                vectY[i]= -(pow( (i-500) , (double) (channel-4) ) );
            vectX[i]= i;
        }

        customPlot[channel]->addGraph();
        customPlot[channel]->graph(0)->setData(vectX, vectY);
        customPlot[channel]->xAxis->setRange(490, 510);
        customPlot[channel]->yAxis->setRange(-100, 100);
        customPlot[channel]->replot();

    }

    for(u_int8_t channel=0; channel< NB_CHANNEL; channel++)
        connect(customPlot[channel]->xAxis,SIGNAL(rangeChanged(QCPRange)),SLOT(xAxisScaleChanged()));
}


void PlotWindow::displayLiveStream(qint64 length, u_int16_t* resultingData)
{
    QTime t;
    t.start();
    double decal=0;
    QVector<double> vectX(length / NB_CHANNEL + 1);
    QVectorDataY

    for(u_int8_t channel=0; channel < NB_CHANNEL; channel++)
        vectY[channel].resize(length / NB_CHANNEL + 1);


    for(int32_t i=0, j=0, channel=0; i<length; i++)
    {
        if ((resultingData[i] & 0xFF00))//&& ((resultingData[i] & 0x000F) < 9) )
        {
            channel = (resultingData[i] & 0x000F)-1;

            if((channel < NB_CHANNEL) && (j < (length / NB_CHANNEL)) )
            {
                if (!channel)
                    vectX[j]= (prev_len + decal) / NB_CHANNEL;

                vectY[channel][j] = (double) (resultingData[i] >> 8);

                decal++;

                if (channel == 7)
                    j++;
                //if ((i % NB_CHANNEL) == NB_CHANNEL-1) j++; // k += i % 8 == 7
            }
        }
    }

    if (prev_len >= 50000)
    {
        prev_len = 0;
        clearAll();
    }

    for(u_int8_t channel=0; channel < NB_CHANNEL; channel++)
    {
        customPlot[channel]->graph(0)->addData(vectX, vectY[channel]);
        customPlot[channel]->replot();
    }
    prev_len += decal;

    qDebug("Time elapsed: %d ms", t.elapsed());
}

void PlotWindow::displayFile()
{
    QString nameInFile = QFileDialog::getOpenFileName(this, "select a file","/home/max/Documents",tr("all (*)"));
    inputFile.setFileName(nameInFile);
    inputFile.open(QIODevice::ReadOnly);
    fileData = inputFile.readAll(); //QByteArray
    inputFile.close();
    numberSample = fileData.size();
    shortData = (u_int16_t*) fileData.data();

    QVector<QVector<double> > vectY( NB_CHANNEL );
    QVector<double> vectX(sizeDataWindow);

    for(int channel=0; channel< NB_CHANNEL; channel++)
        vectY[channel].resize(sizeDataWindow / NB_CHANNEL);

    for(int64_t i=0, j=0, channel=0; i<sizeDataWindow; i++){

        channel=(shortData[i] & 0x000F)-1;

        if (!channel)
            vectX[j]= j / NB_CHANNEL;

            vectY[channel][j] = (double) (shortData[i]);

        if ((i % NB_CHANNEL) == NB_CHANNEL - 1) j++; // k += i % 8 == 7
    }

    for(u_int8_t channel=0; channel < NB_CHANNEL; channel++){
        //create graph and assign data to it:
        customPlot[channel]->graph(0)->setData(vectX, vectY[channel]);
        // give the axes some labels:
        customPlot[channel]->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,3));
        // set axes ranges, so we see all data:
        customPlot[channel]->xAxis->setRange(150, 200);
        customPlot[channel]->yAxis->setRange(-10, 300);
        customPlot[channel]->axisRect()->setupFullAxesBox(true);
        customPlot[channel]->replot();
    }
    emit showSlider();
}



void PlotWindow::changeDataWindow(int plage, int maxPlage)
{
    u_int64_t midle = plage * numberSample / maxPlage;

    QVector<double> vectX(sizeDataWindow);
    QVector<QVector<double> > vectY(4);

    for(u_int8_t channel=0; channel<4; channel++)
        vectY[channel].resize(sizeDataWindow/4);

    for(u_int64_t i=0, j=0, channel=0; i<sizeDataWindow; i++){

        channel= (shortData[midle+i] & 0x000F)-1;

        if(!channel)
            vectX[j]=(midle+i)/4;

        vectY[channel][j] = (double) (shortData[midle+i]);


        if ((i % NB_CHANNEL) == NB_CHANNEL-1) j++; // k += i % 8 == 7
    }

    for(u_int8_t channel=0; channel < NB_CHANNEL; channel++)
    {
        customPlot[channel]->graph(0)->clearData();
        //create graph and assign data to it:
        customPlot[channel]->graph(0)->setData(vectX, vectY[channel]);
        // give the axes some labels:
        //customPlot[channel]->graph(0)->setLineStyle(QCPGraph::lsNone);
        customPlot[channel]->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,3));
        // set axes ranges, so we see all data:
        customPlot[channel]->yAxis->setRange(-10, 300);
        customPlot[channel]->xAxis->setRange(midle+3400, midle+3550);
        customPlot[channel]->axisRect()->setupFullAxesBox(true);
        customPlot[channel]->replot();
    }
}

void PlotWindow::clearAll()
{
    for(int channel=0; channel < NB_CHANNEL; channel++)
    {
        customPlot[channel]->graph(0)->clearData();
        customPlot[channel]->xAxis->setRange(-100, 6000);
        //customPlot[i]->graph(0)->setLineStyle(QCPGraph::lsNone);
        customPlot[channel]->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,3));
        prev_len = 0;
    }
}

void PlotWindow::xAxisScaleChanged()
{
    for(u_int8_t channel=0; channel < NB_CHANNEL; channel++)
        customPlot[channel]->yAxis->setRange(-10,200);
}

