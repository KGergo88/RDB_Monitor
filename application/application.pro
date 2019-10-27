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



TEMPLATE = app

QT       += core        \
            gui         \
            charts      \
            serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += -std=c++17

#Linux
unix {
    message("Linux build was selected!")

    # Compiler requirement: We need g++8 or newer.
    LIBS += -lstdc++fs
}

#Windows
win32 {
    message("Windows build was selected!")

    DEFINES += WIN32_LEAN_AND_MEAN
}

SOURCES +=                                  \
    sources/backend.cpp                     \
    sources/configuration.cpp               \
    sources/data_line.cpp                   \
    sources/data_point.cpp                  \
    sources/diagram.cpp                     \
    sources/diagram_container.cpp           \
    sources/main.cpp                        \
    sources/main_window.cpp                 \
    sources/measurement_data_protocol.cpp   \
    sources/network_handler.cpp             \
    sources/serial_port.cpp

HEADERS +=                                      \
    sources/backend.hpp                         \
    sources/backend_signal_interface.hpp        \
    sources/configuration.hpp                   \
    sources/data_connection_interface.hpp       \
    sources/data_line.hpp                       \
    sources/data_point.hpp                      \
    sources/data_processing_interface.hpp       \
    sources/diagram.hpp                         \
    sources/diagram_container.hpp               \
    sources/global.hpp                          \
    sources/gui_signal_interface.hpp            \
    sources/main_window.hpp                     \
    sources/measurement_data_protocol.hpp       \
    sources/network_connection_interface.hpp    \
    sources/network_handler.hpp                 \
    sources/serial_port.hpp

TARGET = RDB_Diplomaterv_Monitor
