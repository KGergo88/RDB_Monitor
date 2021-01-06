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

message(===============================)
message(========= Application =========)
message(===============================)

# The used Qt components
QT += core        \
      gui         \
      charts      \
      serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Selecting the C++ standard
# This is solved this way and not with the CONFIG variable since that is
# only available since Qt5.12 or later.
unix {
    # Compiler flags
    QMAKE_CXXFLAGS += -std=c++17
}
win32 {
    # Compiler flags
    QMAKE_CXXFLAGS += /std:c++17
}

CONFIG(debug, debug|release) {
    CONFIG += -O0
}

# Source files of the target
SOURCES +=                                          \
    sources/backend.cpp                             \
    sources/configuration.cpp                       \
    sources/add_connection_dialog.cpp               \
    sources/continous_measurement_data_protocol.cpp \
    sources/data_line.cpp                           \
    sources/data_point.cpp                          \
    sources/diagram.cpp                             \
    sources/diagram_container.cpp                   \
    sources/main.cpp                                \
    sources/main_window.cpp                         \
    sources/measurement_data_protocol.cpp           \
    sources/network_handler.cpp                     \
    sources/serial_port.cpp

# Header files of the target
HEADERS +=                                          \
    sources/backend.hpp                             \
    sources/configuration.hpp                       \
    sources/add_connection_dialog.hpp               \
    sources/continous_measurement_data_protocol.hpp \
    sources/data_connection_interface.hpp           \
    sources/data_line.hpp                           \
    sources/data_point.hpp                          \
    sources/diagram.hpp                             \
    sources/diagram_container.hpp                   \
    sources/global.hpp                              \
    sources/i_backend_signal.hpp                    \
    sources/i_gui_signal.hpp                        \
    sources/i_protocol.hpp                          \
    sources/i_network_connection.hpp                \
    sources/main_window.hpp                         \
    sources/measurement_data_protocol.hpp           \
    sources/network_handler.hpp                     \
    sources/ordered_dict.h                          \
    sources/serial_port.hpp

RESOURCES = ../resources.qrc

TARGET = RDB_Diplomaterv_Monitor
