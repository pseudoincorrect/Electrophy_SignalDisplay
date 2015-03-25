#include <AudioInput.h>

const int BufferSize = 14096;
const int lenSize = 5000;

AudioInput::AudioInput() :
        a_Inputdevice(QAudioDeviceInfo::defaultInputDevice())
    ,   a_Outputdevice(QAudioDeviceInfo::defaultOutputDevice())
    ,   a_audioInput(0)
    ,   a_audioOutput(0)
    ,   a_input(0)
    ,   a_buffer(BufferSize,0)
{
    initializeAudio();
}


AudioInput::~AudioInput() {}


void AudioInput::initializeAudio()
{

    a_format.setSampleRate(80000); //set frequency to 40000
    a_format.setChannelCount(2); //set channels to stereo
    a_format.setSampleSize(16); //set sample size to 16 bit
    a_format.setSampleType(QAudioFormat::UnSignedInt); //Sample type as signed integer sample
    a_format.setByteOrder(QAudioFormat::LittleEndian); //Byte order
    a_format.setCodec("audio/pcm"); //set codec as simple audio/pcm

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());
    if (!infoIn.isFormatSupported(a_format))
    {
    //Default format not supported - trying to use nearest
    a_format = infoIn.nearestFormat(a_format);
    }

    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());

    if (!infoOut.isFormatSupported(a_format))
    {
       //Default format not supported - trying to use nearest
    a_format = infoOut.nearestFormat(a_format);
    }
    createAudioInput();
    createAudioOutput();
}

void AudioInput::createAudioOutput()
{
    a_audioOutput = new QAudioOutput(a_Outputdevice, a_format, this);
}

void AudioInput::createAudioInput()
{
  a_audioInput = new QAudioInput(a_Inputdevice, a_format, this);
}

void AudioInput::deviceInChanged(QAudioDeviceInfo deviceInfo)
{
    a_audioInput->stop();
    a_audioInput->disconnect(this);
    delete a_audioInput;
    a_Inputdevice = deviceInfo; //cb_inDeviceBox->itemData(index).value<QAudioDeviceInfo>();
    createAudioInput();
    qWarning() << "device in changed";
}

void AudioInput::deviceOutChanged(QAudioDeviceInfo deviceInfo)
{
    qWarning() << "device out changed";
    a_audioInput->stop();
    a_audioInput->disconnect(this);
    delete a_audioInput;

    a_Outputdevice = deviceInfo; // cb_outDeviceBox->itemData(index).value<QAudioDeviceInfo>();
    createAudioInput();
}

void AudioInput::record(QString name)
{
    qWarning() << "Record to : " << name;
    destinationFile.setFileName(name);

    destinationFile.open( QIODevice::WriteOnly | QIODevice::Truncate );
    a_audioInput->start(&destinationFile);
}

void AudioInput::readMore()
{
    //Return if audio input is null
    if(!a_audioInput){
        qWarning() << "no input";
        return;
    }

    //return the number of audio byte available
    qint64 len = a_audioInput->bytesReady();

    if(len > lenSize)
       len = lenSize;

    //write and return the number of audio byte written
    qint64 l = a_input->read(a_buffer.data(), len);

    //qWarning() << "l" << l;

    if(l > 0)
    {
            //Assign sound samples to short array
            resultingData = (u_int16_t*)a_buffer.data();
            emit dataReady(l,resultingData);
    }

}

void AudioInput::stopRecording()
{
    a_audioInput->stop();
    destinationFile.close();
    qWarning() << "...Recording stopped !";
}



void AudioInput::liveStream()
{
   a_input = a_audioInput->start();
   connect(a_input, SIGNAL(readyRead()), SLOT(readMore()));
}


