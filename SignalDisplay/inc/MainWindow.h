#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AudioInput.h"
#include "PlotWindow.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QGroupBox>
#include <QComboBox>
#include <QProgressBar>
#include <QLineEdit>
#include <QSignalMapper>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private :
    AudioInput *m_audioInput;

    PlotWindow *m_plot;

    QPushButton *p_liveStream;
    QPushButton *p_display;
    QPushButton *p_quit;
    QPushButton *p_record;
    QPushButton *p_info;
    QPushButton *p_select;
    QPushButton *p_stopRecording;

    QLabel *l_directory;
    QLabel *l_cat;
    QLabel *l_device;

    QLineEdit *le_fileName;

    QSignalMapper *sm_signalMapper;
    QSlider *s_dataWindow;

    QVBoxLayout *vl_record;
    QGridLayout *gl_plot;
    QVBoxLayout *vl_quit;

    QGroupBox *gb_record;
    QGroupBox *gb_plot;

    QComboBox *cb_inDeviceBox;
    QComboBox *cb_outDeviceBox;

    QString *s_nameFile;

private slots:
    void record();
    void changeIn(int index);
    void changeOut(int index);
    void changeDataWindow();
    void displayLiveStream();
    void browseDirectory();
    void info();
};

#endif // MAINWINDOW_H
