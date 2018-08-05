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



#include "backend.hpp"
#include "main_window.hpp"
#include "network_handler.hpp"
#include "serial_port.hpp"
#include "measurement_data_protocol.hpp"



Backend::Backend() : QObject(),
                     serial_network_handler(&serial_port, &measurement_data_protocol, std::bind(&Backend::StoreNewDiagram, this, std::placeholders::_1))

{

}

void Backend::StoreNewDiagram(DiagramSpecialized&& diagram)
{
    std::lock_guard<std::mutex> lock(mutex_diagram_container);

    // Adding the diagram to the container that stores them for the GUI
    diagram_container.push_back(diagram);

//    // Emitting a signal to add this diagram to the end of the diagram list
//    emit pMainWindow->signalAddToDiagramList(pMainWindow->diagram_container.size() - 1);
}

void Backend::ReportStatus(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex_report_status);

    time_t rawtime;
    tm timeinfo;
    std::string date_and_time_string;

    // Getting the current time
    time(&rawtime);
    localtime_r(&rawtime, &timeinfo);

    // Resizing the string containing the current time to an appropriate size
    date_and_time_string.resize(report_date_and_time_string_size);

    // Assembling the status message from the time and the input text
    strftime(&date_and_time_string[0], date_and_time_string.size(), "%T", &timeinfo);
    std::string complete_message = date_and_time_string + " - " + message;

//    // Emitting the signal
//    emit pMainWindow->signalReportStatus(complete_message);
}
