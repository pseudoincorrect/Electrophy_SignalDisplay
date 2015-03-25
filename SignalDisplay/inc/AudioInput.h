#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <stdlib.h>
#include <math.h>
#include <qendian.h>

#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QDebug>

#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QAudioInput>

#include <QByteArray>
#include <QBuffer>
#include <QVector>
#include <QDataStream>
#include "qcustomplot.h"

class AudioInput : public QWidget
{
    Q_OBJECT

    public:
        AudioInput();
        ~AudioInput();

    public slots :
        void stopRecording();
        void record(QString name);
        void deviceInChanged(QAudioDeviceInfo deviceInfo);
        void deviceOutChanged(QAudioDeviceInfo deviceInfo);
        void liveStream();

    public :
        u_int16_t*  resultingData;

    signals:
        void dataReady(qint64 length, u_int16_t *resultingData);

    private:
        void initializeAudio();
        void createAudioInput();
        void createAudioOutput();

    private slots :
        void readMore();

    private:
        QAudioDeviceInfo a_Inputdevice;
        QAudioDeviceInfo a_Outputdevice;
        QAudioFormat a_format;
        QAudioInput *a_audioInput;
        QAudioOutput *a_audioOutput;
        QIODevice *a_input;
        QIODevice *a_output;
        QByteArray a_buffer;
        QFile destinationFile;
        QString directoryName;
        QWidget *wdg;

};


#endif // AUDIOINPUT_H
