#===============================================================================#
#                                                                               #
#    RDB Monitor                                                                #
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



message(STATUS "===============================")
message(STATUS "===       RDB Monitor       ===")
message(STATUS "===============================")
message(STATUS "")

cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(RDB_Monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt related part
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Widgets Charts SerialPort REQUIRED)
set(USED_QT_COMPONENTS
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::SerialPort
)

# Application
add_subdirectory(application)

# Tests
if(DEFINED BUILD_TESTS)
    message(STATUS "Tests will be built!")
    add_subdirectory(tests)
    # This line is to supress the unused variable warning for BUILD_TESTS
    set(ignoreMe "${BUILD_TESTS}")
elseif()
    message(STATUS "Tests will not be built, if you need them please define the \"BUILD_TESTS\" variable!")
    message(STATUS "Example: \$ qmake BUILD_TESTS=\"On\" .")
endif()

message(STATUS "")