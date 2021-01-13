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
message(STATUS "============ Tests ============")
message(STATUS "===============================")
message(STATUS "")

function(checkIfPathExists PATH_TO_CHECK)
    if(NOT EXISTS ${${PATH_TO_CHECK}})
        message(FATAL " The directory ${${PATH_TO_CHECK}} does not exists!")
    endif()
endfunction()


# Google Test related part
find_package(Threads REQUIRED)
if(NOT DEFINED GOOGLETEST_DIR)
    if(DEFINED ENV{GOOGLETEST_DIR})
        set(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
    else()
        message(WARNING "The variable GOOGLETEST_DIR was not defined, using the default location!")
        # The default is to clone the Google Test repository next to the main projects folder.
        set(GOOGLETEST_DIR ${CMAKE_CURRENT_LIST_DIR}/../../googletest)
    endif()
endif()
checkIfPathExists(GOOGLETEST_DIR)

set(GTEST_SRCDIR ${GOOGLETEST_DIR}/googletest)
checkIfPathExists(GTEST_SRCDIR)

set(GMOCK_SRCDIR ${GOOGLETEST_DIR}/googlemock)
checkIfPathExists(GMOCK_SRCDIR)

add_definitions(-DGTEST_LANG_CXX11)

set(GOOGLETEST_INCLUDES
    ${GTEST_SRCDIR}
    ${GTEST_SRCDIR}/include
    ${GMOCK_SRCDIR}
    ${GMOCK_SRCDIR}/include
)

set(GOOGLETEST_SOURCES
    ${GTEST_SRCDIR}/src/gtest-all.cc
    ${GMOCK_SRCDIR}/src/gmock-all.cc
)

# Source files of the tests
set(TEST_SOURCES
    sources/test_main.cpp
    sources/test_data_point.cpp
    sources/test_data_line.cpp
    sources/test_diagram.cpp
    sources/test_configuration.cpp
    sources/test_diagram_container.cpp
    sources/test_measurement_data_protocol.cpp
    sources/test_continous_measurement_data_protocol.cpp
    sources/test_ordered_dict.cpp
    sources/test_serial_port.cpp
    sources/test_backend.cpp
)

# Source files of the application
set(APPLICATION_SOURCES
    ../application/sources/configuration.cpp
    ../application/sources/measurement_data_protocol.cpp
    ../application/sources/continous_measurement_data_protocol.cpp
)

set(EXECUTABLE_NAME ${PROJECT_NAME}_Unit_Tests)

add_executable(${EXECUTABLE_NAME}
    ${TEST_SOURCES}
    ${APPLICATION_SOURCES}
    ${GOOGLETEST_SOURCES}
)

target_include_directories(
    ${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${GOOGLETEST_INCLUDES}
)

target_link_libraries(
    ${EXECUTABLE_NAME} PRIVATE
    ${USED_QT_COMPONENTS}
    Threads::Threads
)

# Copying the test files next to the binary
add_custom_command(TARGET ${EXECUTABLE_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/test_files/ $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/test_files)

message(STATUS "")
