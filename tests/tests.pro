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

message(===============================)
message(============ Tests ============)
message(===============================)

CONFIG +=   \
    console \
    thread

# The used Qt components
QT += core        \
      gui         \
      charts      \
      serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Compiler flags
# --coverage option is synonym for: -fprofile-arcs -ftest-coverage -lgcov
QMAKE_CXXFLAGS += -std=c++17 --coverage
# Linker flags
QMAKE_LFLAGS += --coverage

# Source files of the target
SOURCES +=                                                  \
    ../application/sources/configuration.cpp                \
    ../application/sources/diagram_container.cpp            \
    ../application/sources/measurement_data_protocol.cpp    \
    sources/test_main.cpp                                   \
    sources/test_data_point.cpp                             \
    sources/test_data_line.cpp                              \
    sources/test_diagram.cpp                                \
    sources/test_configuration.cpp                          \
    sources/test_diagram_container.cpp                      \
    sources/test_measurement_data_protocol.cpp              \
    sources/test_serial_port.cpp                            \
    sources/test_backend.cpp

HEADERS +=                                                  \
    ../application/sources/diagram_container.hpp             

DISTFILES +=                                        \
    gtest_dendency.pri                              \
    test_files/TEST_1C_0E_MDP.mdp                   \
    test_files/TEST_1C_1E_MDP_HeadlineError.mdp     \
    test_files/TEST_1C_2E_MDP_DatalineError.mdp     \
    test_files/TEST_2C_0E_MDP.mdp

TARGET = RDB_Diplomaterv_Monitor_Unit_Tests

# Copying the test files next to the binary
copydata.commands = $(COPY_DIR) $$PWD/test_files $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
