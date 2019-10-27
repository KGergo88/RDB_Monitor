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



include(GoogleTest.pri)



TEMPLATE = app

CONFIG += console
CONFIG += thread

# --coverage option is synonym for: -fprofile-arcs -ftest-coverage -lgcov
QMAKE_CXXFLAGS += -std=c++17 --coverage
QMAKE_LFLAGS += --coverage

#Linux
unix {
    message("Linux build was selected!")

    LIBS += -lstdc++fs
}

#Windows
win32 {
    message("Windows build was selected!")

    DEFINES += WIN32_LEAN_AND_MEAN
}

SOURCES +=                                      \
    ../application/sources/configuration.cpp    \
    sources/test_main.cpp                       \
    sources/test_data_point.cpp                 \
    sources/test_data_line.cpp                  \
    sources/test_diagram.cpp                    \
    sources/test_configuration.cpp

DISTFILES += gtest_dendency.pri

TARGET = RDB_Diplomaterv_Monitor_Unit_Tests
