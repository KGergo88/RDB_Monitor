#-------------------------------------------------
#
# Project created by QtCreator 2017-08-26T18:47:03
#
#-------------------------------------------------

QT       += core   \
            gui    \
            charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RDB_Diplomaterv_Monitor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14

#Linux
unix {
    message("Linux build was selected!")
    INCLUDEPATH += $$_BOOST_ROOT
    LIBS += -lboost_system
}

#Windows
win32 {
    message("Windows build was selected!")

    DEFINES += WIN32_LEAN_AND_MEAN

    #This is the folder that contains the header files of the boost library.
    _BOOST_INCLUDES = "C:/boost/boost_1_66_0_msvc64_14_1/"
    #This is the folder that contains the compiled library files.
    _BOOST_LIBRARIES = "C:/boost/boost_1_66_0_msvc64_14_1/lib64-msvc-14.1"

    INCLUDEPATH += $$_BOOST_INCLUDES
    LIBS += -L$$_BOOST_LIBRARIES
}

SOURCES += \
    sources/data_line.cpp \
    sources/data_point.cpp \
    sources/diagram.cpp \
    sources/main.cpp \
    sources/gui.cpp \
    sources/serial_port.cpp \
    sources/data_processor.cpp

HEADERS += \
    sources/data_line.hpp \
    sources/data_point.hpp \
    sources/diagram.hpp \
    sources/data_line.hpp \
    sources/data_point.hpp \
    sources/diagram.hpp \
    sources/gui.hpp \
    sources/global.hpp \
    sources/serial_port.hpp \
    sources/data_processor.hpp
