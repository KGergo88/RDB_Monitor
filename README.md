# RDB Monitor <!-- omit in toc -->
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

 A generic measurement display program originally developed for the [RDB Diplomaterv](https://github.com/KGergo88/RDB_Diplomaterv) project.

![alt text](images/RDB_Monitor_Screenshot.png "RDB_Monitor")


## Table of contents <!-- omit in toc -->
- [Introduction](#introduction)
- [Usage](#usage)
  - [Importing diagrams](#importing-diagrams)
  - [Exporting diagrams](#exporting-diagrams)
  - [Setting up a connection](#setting-up-a-connection)
  - [Removing a connection](#removing-a-connection)
- [Supported connections](#supported-connections)
  - [Serial port](#serial-port)
- [Supported protocols](#supported-protocols)
  - [Measurement Data Protocol (MDP)](#measurement-data-protocol-mdp)
- [Dependencies and portability](#dependencies-and-portability)
- [Building the project](#building-the-project)
  - [Building the project on Linux](#building-the-project-on-linux)
  - [Building the project on Windows](#building-the-project-on-windows)
- [CI system](#ci-system)
  - [CppCheck](#cppcheck)
  - [BuildAndTestOnLatestUbuntu](#buildandtestonlatestubuntu)
  - [BuildAndTestOnLatestWindows](#buildandtestonlatestwindows)
- [Release notes](#release-notes)
  - [v2.1.0 - File handling](#v210---file-handling)
  - [v2.0.0 - Architectural rework](#v200---architectural-rework)
  - [v1.0.2 - Shutdown failure patch](#v102---shutdown-failure-patch)
  - [v1.0.1 - Memory leak patch for the first release](#v101---memory-leak-patch-for-the-first-release)
  - [v1.0.0 - First release](#v100---first-release)
- [License](#license)


## Introduction

The RDB Monitor is a generic measurement result visualiser software. It was originally developed for the RDB Diplomaterv project.
The primary use case is to visualise data from the embedded controller of an electric drive in order to be able to setup the current, speed, position controllers. Besides this, the program can be used with any other software or device that produces output that follows one of the [supported protocols](#supported-protocols). The data can be received through one of the [supported connections](#supported-connections) or imported from a file. The created diagrams can be also exported to a file.


## Usage

### Importing diagrams

 - Click the **Import / Export Diagrams** menu in the menubar
 - Select **Import Diagrams**
 - Select the file you want to import
 - The imported files and their diagrams will apper under **Available diagrams**

### Exporting diagrams

 - Click the **Import / Export Diagrams** menu in the menubar
 - Select **Export Diagrams**
 - Select the diagrams you want to export under **Available diagrams**
 - Click the **Export** Button in the lower right corner

### Setting up a connection

- Click the **Add connection** button
- Select a connection from the **Available connections** trough the data is going to be received
- Select a protocol from the **Available protocols** that will process the received data
- Give a name for your connection that will appear in the connection manager
- Edit the **Connection settings** if needed
- Click the **Ok** button
- The connection should apper in the connection manager, if not please check the status messages

### Removing a connection

- Select the connection you want to remove
- Click the **Remove connection** button


## Supported connections

Connections are used for receiving raw data from the network. The following subchapters describing the available connections.

### Serial port

A simple connection typically used to receive data from embedded devices. It is implemented with the QSerialPort class. The following settings can be edited:

 - Port name
 - Baudrate
 - Data bits
 - Stop bits
 - Parity
 - Flow control


## Supported protocols

Protocols are used for processing the data received trough a connection and for importing and exporting files. The following subchapters describing the available protocols.

### Measurement Data Protocol (MDP)

This protocol is used for data reception trough the serial port and for data storage in the file system. It is a simple text based protocol and can be implemented on microcontrollers with limited resources.

- One data transmission is called a session, that contains data for one diagram
- A session is built up by the following parts:
    - Start pattern
    - Optionally the diagram title
    - The names of the measured quantities and the horizontal axis title
    - The measured values with the horizontal axis values
    - End pattern

The data needs to be formatted the following way in order to be processed by the program:
```
<<<START>>>
<DIAGRAM_TITLE>
"X_AXIS_TITLE","Y0_AXIS_TITLE",...,"Yn_AXIS_TITLE",
"X_AXIS_DATA0","Y0_AXIS_DATA0",...,"Yn_AXIS_DATA0",
.
.
.
"X_AXIS_DATAn","Y0_AXIS_DATAn",...,"Yn_AXIS_DATAn",
<<<END>>>
```

The following requirements have to be fulfilled:
  - The " symbols are not part of the format they are only marking the beginning and the end of an element
  - The measurement data is always starting with the pattern "<<<START>>>"
  - Then the optional diagram title is transmitted, which can be omitted if not needed, in which case a title will be generated for the diagram.
  - Then the headline is transmitted:
    - Every headline element is followed by a comma "," and there are no whitespaces before between and after the elements
    - The headline describes how many different signals were measured and their names
    - There has to be always at least two headline elements transmitted: the common X axis, which is the horizontal axis and at least one Y axis
    - The headline elements:
      - can only contain alphanumeric characters (0..9, A..Z, a..z)
  - Then the headline the measured data is transmitted:
    - Every dataline element is followed by a comma "," and there are no whitespaces before between and after the elements
    - There has to be as many elements in a data line as there was in the headline
    - The data line elements are the values that were measured at the same time
    - All the measured signal values in the data line belong to the first data line element, the common X axis value
    - The data line elements:
      - are integer values
      - can have an optional "+" or "-" sign as a prefix, for example "+25" or "-568"
      - without a sign prefix are considered positive
  - The measurement data is always ending with the pattern "<<<END>>>"
  - This data, starting with the start pattern until the end of the end pattern is called a session that contains all data needed to describe a diagram
  - If there was an error detected in one of the lines then the whole session is invalid

Examples for a session without diagram title and two measured signals:
```
<<<START>>>
u32TimeMs,u32SetValue,u32ActualValue,
0,4000,0,
10,4000,500,
20,4000,1000,
30,4000,1500,
40,4000,2000,
50,4000,2500,
60,4000,3000,
70,4000,3500,
80,4000,4000,
<<<END>>>
```

Examples for a session with diagram title and one measured signal:
```
<<<START>>>
<Living room daily temperature>
u32TimeHour,u8RoomTemperature,
0,15,
2,17,
4,19,
6,18,
8,20,
10,22,
12,24,
14,26,
16,24,
18,22,
20,20,
22,18,
<<<END>>>
```


## Dependencies and portability

The RDB Monitor was developed using C++17 and it depends on the following external components:
  - Qt5 v5.12.0 or newer
    - The used components are Core, Widgets, and Charts SerialPort
    - You can download it from the [Qt website](www.qt.io)

The project can be build and used on Linux and Windows as well. Please see the [CI system](#CI-system) chapter for more details on the supported OS versions.


## Building the project

The RDB Monitor is supports QMake and CMAke as well. The project is separated into two parts: the **application** and the **tests**. The **tests** part is not build by default, only if expicitly requested trough the **BUILD_TESTS** flag.

### Building the project on Linux

On Linux the project can be built either by using the QtCreator or trough the terminal.

For a build with QMake use the following commands inside the project folder:

``` bash
$ mkdir build
$ cd build
$ qmake BUILD_TESTS="On" .. # The BUILD_TESTS flag is optional
$ make
```
For a build with CMake use the following commands inside the project folder:

``` bash
$ mkdir build
$ cd build
$ cmake -DBUILD_TESTS=On .. # The BUILD_TESTS flag is optional
$ make
```

### Building the project on Windows

On Windows the it is suggested to build the project using the QtCreator. If you need to build from the CMD you can use CMake.

For a build with CMake use the following commands inside the project folder:

``` bat
mkdir build
cd build
REM The BUILD_TESTS flag is optional
REM Replace the Visual Studio 16 2019 with your compiler version if needed
REM Replace x64 if you intend to build for the x86 architecture
cmake .. -DBUILD_TESTS=On -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```


## CI system

The project is using CI to ensure that the pull requests are not merging commits that contain errors. The CI jobs are run with [GitHub Actions](https://github.com/features/actions). The job descriptions can be found [here](https://github.com/KGergo88/RDB_Monitor/tree/master/.github/workflows). The following subchapters describe the jobs that must succeed before a pull request can be merged.

### CppCheck

This job runs the [CppCheck](http://cppcheck.sourceforge.net/) static analysis tool on the codebase and reports the findings.

### BuildAndTestOnLatestUbuntu

This job builds the project in the [ubuntu-latest](https://docs.github.com/en/free-pro-team@latest/actions/reference/specifications-for-github-hosted-runners) virtual environment. Both the QMake and CMake builds tested with this job. After a successful build the unit tests are run and the unit test coverage report is created with the [test_coverage.py](https://github.com/KGergo88/RDB_Monitor/blob/master/tests/test_coverage.py) script. The coverage results are posted in the pull request as a comment with the [Lcov reporter action](https://github.com/romeovs/lcov-reporter-action).

### BuildAndTestOnLatestWindows

This job builds the project in the [windows-latest](https://docs.github.com/en/free-pro-team@latest/actions/reference/specifications-for-github-hosted-runners) virtual environment. Only the CMake build is tested with this job. After a successful build the unit tests are run.


## Release notes

### v2.1.0 - File handling
#### Details:<!-- omit in toc -->
  - Added diagram import/export from/to filesystem
  - Added tree view for the available diagrams
  - MDP protocol: new, optional diagram title feature
  - Reorganizing the diagram storage to use the model-view-controller design pattern
  - Added configuration JSON file handling for default folders and future settings
  - Using layouts to control the window organization
  - Added menu bar

### v2.0.0 - Architectural rework
#### Details:<!-- omit in toc -->
  - Improvement of the GUI especially the diagram view area
  - New status report messages were added so that the user has more info during runtime
  - The program now is a single-threaded application utiling an event based design
  - Implementation of the new design with signal interfaces
  - Further improvement of the data contained classes: DataPoint, DataLine and Diagram
  - Removal of the BOOST from the project
  - The SerialPort class is now implemented with the SerialPort library of the Qt framework

### v1.0.2 - Shutdown failure patch
#### Fixes:<!-- omit in toc -->
  - The shutdown problem that was described in the errata of the v1.0.1 was fixed

### v1.0.1 - Memory leak patch for the first release
#### Fixes:<!-- omit in toc -->
  - The memory that was described in the errata of the v1.0.0 was fixed

#### Errata:<!-- omit in toc -->
  - The worker thread will not join when quitting the program while the serial port is open

### v1.0.0 - First release
#### Functionality:<!-- omit in toc -->
  + Opening a serial port and listening to it
  + Processing and displaying the diagrams received on the serial port
  + Keeping the received diagrams in the list and allowing switching between them

#### Errata:<!-- omit in toc -->
  - A probable memory leak is present in the program
      + It can be seen when there are at least two diagrams listed in the program
      + When it is being switched between them, the memory usage of the program is increasing rapidly with every switch


## License

RDB Monitor
Copyright (C) 2018  András Gergő Kocsis

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program (see LICENSE file).  If not, see <https://www.gnu.org/licenses/>.
