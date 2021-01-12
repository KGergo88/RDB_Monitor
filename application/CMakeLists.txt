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



message(STATUS "===============================")
message(STATUS "========= Application =========")
message(STATUS "===============================")
message(STATUS "")

# Source files of the target
set(SOURCES
    sources/backend.cpp
    sources/configuration.cpp
    sources/add_connection_dialog.cpp
    sources/continous_measurement_data_protocol.cpp
    sources/data_line.cpp
    sources/data_point.cpp
    sources/diagram.cpp
    sources/diagram_container.cpp
    sources/main.cpp
    sources/main_window.cpp
    sources/measurement_data_protocol.cpp
    sources/network_handler.cpp
    sources/serial_port.cpp
    sources/serial_port_settings_editor.cpp
)

set(EXECUTABLE_NAME ${PROJECT_NAME})

add_executable(${EXECUTABLE_NAME}
    ${SOURCES}
    ../resources.qrc
)

target_include_directories(
    ${EXECUTABLE_NAME} PRIVATE
    sources
)

target_link_libraries(
    ${EXECUTABLE_NAME} PRIVATE
    ${USED_QT_COMPONENTS}
)

message(STATUS "")
