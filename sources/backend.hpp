//==============================================================================//
//                                                                              //
//    RDB Diplomaterv Monitor                                                   //
//    A monitor program for the RDB Diplomaterv project                         //
//    Copyright (C) 2018  András Gergő Kocsis                                   //
//                                                                              //
//    This program is free software: you can redistribute it and/or modify      //
//    it under the terms of the GNU General Public License as published by      //
//    the Free Software Foundation, either version 3 of the License, or         //
//    (at your option) any later version.                                       //
//                                                                              //
//    This program is distributed in the hope that it will be useful,           //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of            //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
//    GNU General Public License for more details.                              //
//                                                                              //
//    You should have received a copy of the GNU General Public License         //
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.    //
//                                                                              //
//==============================================================================//



#include <iostream>
#include <mutex>

#include <QApplication>

#include "global.hpp"
#include "diagram.hpp"



#ifndef GUI_FRAMEWORK_HPP
#define GUI_FRAMEWORK_HPP



class Backend final
{
private:
    QApplication qt_application;

    MainWindow main_window;

    SerialPort serial_port;
    DataProcessor data_processor;
    NetworkHandler serial_network_handler;

    std::mutex mutex_report_status;

    std::vector<DiagramSpecialized> diagram_container;

public:
    static constexpr std::size_t report_date_and_time_string_size = 10;

    Backend() = delete;
    Backend(int argc, char **argv);
    Backend(const Backend&  newQtFramework) = delete;
    Backend(Backend&& newQtFramework) = delete;

    ~Backend() = default;

    Backend& operator=(const Backend&  newQtFramework) = delete;
    Backend& operator=(Backend&& newQtFramework) = delete;

    static void Run(int argc, char **argv);
//    static bool IsRunning(void);
//    static void ReportStatus(const std::string& message);
};



#endif /* GUI_FRAMEWORK_HPP */
