#include <QApplication>
#include "AudioInput.h"
#include "qcustomplot.h"
#include "MainWindow.h"
#include "PlotWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
