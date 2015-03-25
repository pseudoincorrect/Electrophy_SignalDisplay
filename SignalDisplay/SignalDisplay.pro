TEMPLATE = app
TARGET = SignalDisplay

QT += printsupport
QT += multimedia widgets

INCLUDEPATH +=  inc/ \
                src/ \
                pic/

HEADERS += \
    inc/qcustomplot.h   \
    inc/AudioInput.h    \
    inc/MainWindow.h    \
    inc/PlotWindow.h \
    inc/DataY.h

SOURCES += \
    main.cpp            \
    src/qcustomplot.cpp \
    src/AudioInput.cpp  \
    src/MainWindow.cpp  \
    src/PlotWindow.cpp \
    src/DataY.cpp

