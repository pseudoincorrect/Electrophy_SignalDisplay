#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QScopedPointer<QWidget> window(new QWidget);
    QScopedPointer<QHBoxLayout> hl_principal(new QHBoxLayout);

    m_audioInput = new AudioInput();

    m_plot = new PlotWindow();

    p_liveStream = new QPushButton("Live input stream", this);
        p_liveStream->setFont(QFont("Times",14));
        connect(p_liveStream, SIGNAL(clicked()),this,SLOT(displayLiveStream()));
    p_display = new QPushButton("Display a recorded file", this);
        p_display->setFont(QFont("Times",14));
        connect(p_display, SIGNAL(clicked()),m_plot,SLOT(displayFile()));
    p_quit = new QPushButton("Quit", this);
        p_quit->setFont(QFont("Times",14));
        connect(p_quit, SIGNAL(clicked()),qApp, SLOT(quit()));
    p_select = new QPushButton("Select repertory", this);
        p_select->setFont(QFont("Times",14));
        connect(p_select, SIGNAL(clicked()),this,SLOT(browseDirectory()));
    p_info = new QPushButton("Info", this);
        p_info->setFont(QFont("Times",14));
        connect(p_info, SIGNAL(clicked()),this,SLOT(info()));
    p_record = new QPushButton("Record", this);
        p_record->setFont(QFont("Times",14));
        connect(p_record,SIGNAL(clicked()),this,SLOT(record()));
    p_stopRecording = new QPushButton("Stop", this);
        p_stopRecording->setFont(QFont("Times",14));
        connect(p_stopRecording, SIGNAL(clicked()),m_audioInput,SLOT(stopRecording()));

    l_device = new QLabel("select bellow the audio device",this);
    l_device->setMaximumHeight(25);
    l_directory = new QLabel("../Recording",this);
    l_directory->setMaximumHeight(25);
    l_cat = new QLabel(this);
    QPixmap pm_Cat(QPixmap("../SignalDisplay/pic/cat.jpg"));
    l_cat->setPixmap(pm_Cat.scaled(QSize(250, 150)));

    le_fileName = new QLineEdit("recording1",this);

    s_dataWindow = new QSlider(Qt::Horizontal,this);
    s_dataWindow->setMinimum(0);
    s_dataWindow->setMaximum(10000);
    connect(s_dataWindow, SIGNAL(sliderMoved(int)),this, SLOT(changeDataWindow()));
    s_dataWindow->hide();
    connect(m_plot,SIGNAL(showSlider()),s_dataWindow,SLOT(show()));

    //hl_principal = new QHBoxLayout;
    vl_record = new QVBoxLayout;
    gl_plot = new QGridLayout;

    cb_inDeviceBox = new QComboBox;
    QList<QAudioDeviceInfo> inDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    for(int i = 0; i < inDevices.size(); ++i)
        cb_inDeviceBox->addItem(inDevices.at(i).deviceName(), qVariantFromValue(inDevices.at(i)));
    connect(cb_inDeviceBox, SIGNAL(activated(int)),this, SLOT(changeIn(int)));

    cb_outDeviceBox = new QComboBox;
    QList<QAudioDeviceInfo> outDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for(int i = 0; i < outDevices.size(); ++i)
        cb_outDeviceBox->addItem(outDevices.at(i).deviceName(), qVariantFromValue(outDevices.at(i)));
    connect(cb_outDeviceBox, SIGNAL(activated(int)),this, SLOT(changeOut(int)));

    gb_record = new QGroupBox(this);
    gb_plot= new QGroupBox(this);

    //Record implementation
    vl_record->addWidget(l_device);
    vl_record->addWidget(cb_inDeviceBox);
    //vl_record->addWidget(cb_outDeviceBox);
    vl_record->addWidget(p_liveStream);
    vl_record->addWidget(p_display);
    vl_record->addWidget(p_info);
    vl_record->addWidget(p_select);
    vl_record->addWidget(l_directory);
    vl_record->addWidget(le_fileName);
    vl_record->addWidget(p_record);
    vl_record->addWidget(p_stopRecording);
    vl_record->addWidget(l_cat);
    vl_record->addWidget(p_quit);

    gb_record->setLayout(vl_record);
    gb_record->setFixedWidth(250);

    gl_plot->addWidget(m_plot->customPlot[0],0,0);
    gl_plot->addWidget(m_plot->customPlot[1],0,1);
    gl_plot->addWidget(m_plot->customPlot[2],1,0);
    gl_plot->addWidget(m_plot->customPlot[3],1,1);
    gl_plot->addWidget(m_plot->customPlot[4],2,0);
    gl_plot->addWidget(m_plot->customPlot[5],2,1);
    gl_plot->addWidget(m_plot->customPlot[6],3,0);
    gl_plot->addWidget(m_plot->customPlot[7],3,1);
    gl_plot->addWidget(s_dataWindow,2,0,1,0);
    gb_plot->setLayout(gl_plot);

    //Principal Layout implementation
    hl_principal->addWidget(gb_record);
    hl_principal->addWidget(gb_plot);
    m_plot->customPlot[0]->setMinimumSize(400,150);
    m_plot->customPlot[1]->setMinimumSize(400,150);
    m_plot->customPlot[2]->setMinimumSize(400,150);
    m_plot->customPlot[3]->setMinimumSize(400,150);
    m_plot->customPlot[4]->setMinimumSize(400,150);
    m_plot->customPlot[5]->setMinimumSize(400,150);
    m_plot->customPlot[6]->setMinimumSize(400,150);
    m_plot->customPlot[7]->setMinimumSize(400,150);
    window->setLayout(hl_principal.data());
    hl_principal.take();

    setCentralWidget(window.data());
    QWidget *const windowPtr = window.take(); // ownership transferred

    windowPtr->show();

}

void MainWindow::record()
{
    qWarning() << "Recording...";
    s_nameFile = new QString(l_directory->text() +"/"+ le_fileName->text() + ".raw");
    m_audioInput->record(*s_nameFile);
}

void MainWindow::browseDirectory()
{
   QString directoryName = QFileDialog::getExistingDirectory(this);
   if(directoryName.isEmpty())
   {
       directoryName = "aucun repertoire selectionné";
   }
   l_directory->setText(directoryName);
}

void MainWindow::changeIn(int index)
{
    QAudioDeviceInfo deviceIn = cb_inDeviceBox->itemData(index).value<QAudioDeviceInfo>();
    m_audioInput->deviceInChanged(deviceIn);
    cb_inDeviceBox->setCurrentIndex(index);
}

void MainWindow::changeOut(int index)
{
    QAudioDeviceInfo deviceOut = cb_outDeviceBox->itemData(index).value<QAudioDeviceInfo>();
    m_audioInput->deviceOutChanged(deviceOut);
    cb_outDeviceBox->setCurrentIndex(index);
}

void MainWindow::changeDataWindow()
{
    m_plot->changeDataWindow(this->s_dataWindow->value(), this->s_dataWindow->maximum());
}

void MainWindow::displayLiveStream()
{
    m_plot->clearAll();
    disconnect(m_audioInput, SIGNAL(dataReady(qint64, u_int16_t*)),m_plot,SLOT(displayLiveStream(qint64, u_int16_t*)));
    m_audioInput->liveStream();
    connect(m_audioInput, SIGNAL(dataReady(qint64, u_int16_t*)),m_plot,SLOT(displayLiveStream(qint64, u_int16_t*)));
}

void MainWindow::info()
{
    changeIn(4);
}
















