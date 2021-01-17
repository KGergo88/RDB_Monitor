#===============================================================================#
#                                                                               #
#    RDB Monitor                                                                #
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

CONFIG(debug, debug|release) {
    BUILD_TYPE = debug
} else {
    BUILD_TYPE = release
}

# The C++ standard selection is solved this way and not with the CONFIG variable since that is
# only available since Qt5.12 or later.
unix {
    # Compiler flags
    # --coverage option is synonym for: -fprofile-arcs -ftest-coverage -lgcov
    QMAKE_CXXFLAGS += -std=c++17 --coverage
    # Linker flags
    QMAKE_LFLAGS += --coverage
}
win32 {
    # Compiler flags
    QMAKE_CXXFLAGS += /std:c++17
}

# Source files of the target
SOURCES +=                                                          \
    ../application/sources/configuration.cpp                        \
    ../application/sources/measurement_data_protocol.cpp            \
    ../application/sources/continous_measurement_data_protocol.cpp  \
    ../application/sources/serial_port.cpp                          \
    sources/test_main.cpp                                           \
    sources/test_data_point.cpp                                     \
    sources/test_data_line.cpp                                      \
    sources/test_diagram.cpp                                        \
    sources/test_configuration.cpp                                  \
    sources/test_diagram_container.cpp                              \
    sources/test_measurement_data_protocol.cpp                      \
    sources/test_continous_measurement_data_protocol.cpp            \
    sources/test_ordered_dict.cpp                                   \
    sources/test_serial_port.cpp                                    \
    sources/test_backend.cpp

HEADERS +=                          \
    sources/test_protocol_common.h

DISTFILES +=                                    \
    gtest_dendency.pri                          \
    test_files/TEST_1C_0E_MDP.mdp               \
    test_files/TEST_1C_1E_MDP_HeadlineError.mdp \
    test_files/TEST_1C_2E_MDP_DatalineError.mdp \
    test_files/TEST_2C_0E_MDP.mdp               \
    test_files/TEST_1C_0E_CMDP.cmdp             \
    test_files/TEST_1C_0E_CMDP_NoTitles.cmdp    \
    test_files/TEST_1C_0E_CMDP_Simple.cmdp

TARGET = RDB_Monitor_Unit_Tests

# Copying the test files next to the binary
TEST_FILES_SOURCE_PATH = $$PWD/test_files
unix {
    TEST_FILES_TARGET_PATH = $$OUT_PWD/test_files
}
win32 {
    TEST_FILES_TARGET_PATH = $$OUT_PWD/$$BUILD_TYPE/test_files
}
message(Copying the test_files from: $$TEST_FILES_SOURCE_PATH)
message(Copying the test_files to: $$TEST_FILES_TARGET_PATH)
copydata.commands = $(COPY_DIR) $$system_path($$TEST_FILES_SOURCE_PATH) $$system_path($$TEST_FILES_TARGET_PATH)
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
