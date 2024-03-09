######################################################################
# Automatically generated by qmake (3.1) Sat Mar 9 12:17:21 2024
######################################################################

TEMPLATE = app
TARGET = cv_fractal
INCLUDEPATH += .
QT += widgets 
# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x060000 # disables all APIs deprecated in Qt 6.0.0 and earlier
LIBS += `pkg-config opencv4 --libs` 
QMAKE_CXXFLAGS += -O3 `pkg-config opencv4 --cflags` 
# Input
HEADERS += cv_fractal.hpp frac_window.hpp
SOURCES += frac_main.cpp frac_window.cpp 
