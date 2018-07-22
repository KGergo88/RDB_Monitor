#===============================================================================#
#                                                                               #
#    RDB Diplomaterv Monitor                                                    #
#       A monitor program for the RDB Diplomaterv project                       #
#    Copyright (C) 2018  András Gergő Kocsis                                    #
#                                                                               #
#    This program is free software: you can redistribute it and/or modify       #
#    it under the terms of the GNU General Public License as published by       #
#    the Free Software Foundation, either version 3 of the License, or          #
#    (at your option) any later version.                                        #
#                                                                               #
#    This program is distributed in the hope that it will be useful,            #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#    GNU General Public License for more details.                               #
#                                                                               #
#    You should have received a copy of the GNU General Public License          #
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.     #
#                                                                               #
#===============================================================================#



QT       += core       \
            gui        \
            charts     \
            serialport \

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
}

#Windows
win32 {
    message("Windows build was selected!")

    DEFINES += WIN32_LEAN_AND_MEAN
}

SOURCES += \
    sources/data_line.cpp \
    sources/data_point.cpp \
    sources/diagram.cpp \
    sources/main.cpp \
    sources/serial_port.cpp \
    sources/data_processor.cpp \
    sources/data_processor_interface.cpp \
    sources/network_handler.cpp \
    sources/main_window.cpp \
    sources/qt_framework.cpp

HEADERS += \
    sources/data_line.hpp \
    sources/data_point.hpp \
    sources/diagram.hpp \
    sources/data_line.hpp \
    sources/data_point.hpp \
    sources/diagram.hpp \
    sources/global.hpp \
    sources/serial_port.hpp \
    sources/data_processor.hpp \
    sources/network_connection_interface.hpp \
    sources/data_processor_interface.hpp \
    sources/network_handler.hpp \
    sources/main_window.hpp \
    sources/qt_framework.hpp

DISTFILES += \
    README
