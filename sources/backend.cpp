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
                     serial_port(),
                     measurement_data_protocol(),
                     serial_network_handler(&serial_port,
                                            &measurement_data_protocol,
                                            std::bind(&Backend::StoreNewDiagrams, this, std::placeholders::_1),
                                            std::bind(&Backend::ReportStatus, this, std::placeholders::_1)),
                     gui_signal_interface(nullptr)
{

}

void Backend::RegisterGuiSignalInterface(GuiSignalInterface* new_gui_signal_interface)
{
    if(new_gui_signal_interface)
    {
        gui_signal_interface = new_gui_signal_interface;

        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface), SIGNAL(OpenNetworkConnection(const std::string&)),  this, SLOT(OpenNetwokConnection(const std::string&)));
        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface), SIGNAL(CloseNetworkConnection(const std::string&)), this, SLOT(CloseNetworkConnection(const std::string&)));
        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface), SIGNAL(RequestForDiagram(const DataIndexType&)),    this, SLOT(RequestForDiagram(const DataIndexType&)));
        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface), SIGNAL(OpenFile(const std::string&)),               this, SLOT(OpenFile(const std::string&)));
    }
    else
    {
        std::string errorMessage = "There was no gui_signal_interface set in Backend::RegisterGuiSignalInterface!";
        throw errorMessage;
    }
}

void Backend::StoreNewDiagrams(std::vector<std::shared_ptr<DiagramSpecialized> >& new_diagrams)
{
    auto this_is_the_first_diagram = diagram_container.empty();

    // Adding the diagrams to the diagram_container
    for(const auto& i : new_diagrams)
    {
        diagram_container.push_back(*i);
    }

    ReportStatus(std::to_string(new_diagrams.size()) + " new diagram was added to the list.");
    NotifyAboutDiagramContainerChange();

    if(this_is_the_first_diagram)
    {
        emit ShowThisDiagram(*diagram_container.begin());
    }
}

void Backend::NotifyAboutDiagramContainerChange(void)
{
    std::vector<std::string> diagram_titles;

    for(const auto& i : diagram_container)
    {
        diagram_titles.push_back(i.GetTitle());
    }

    emit DiagramListHasChanged(diagram_titles);
}

void Backend::ReportStatus(const std::string& message)
{
    // The format string needs to be defined as a string literal so that the compiler can check it's content
    #define STATUS_REPORT_FORMAT_STRING ("%02d:%02d:%02d - %s")
    // This is the pre-calculated length of the status report, this can be used for checks
    std::size_t status_report_length = sizeof("HH:MM:SS - ") + message.size();

    // Getting the current time
    // Under Windows, the warning might arise that the localtime() is deprecated and the localtime_s() should be used instead.
    // This warning will be ignored because GCC does not support it and for keeping the code simple.
    time_t raw_time;
    struct tm* local_time_values;
    raw_time = time(nullptr);
    local_time_values = localtime(&raw_time);

    // Assembling the status message from the time and the input text
    auto report_message = std::make_unique<char[]>(status_report_length);
    auto snprintf_result = snprintf(report_message.get(), status_report_length, STATUS_REPORT_FORMAT_STRING,
                                    local_time_values->tm_hour,
                                    local_time_values->tm_min,
                                    local_time_values->tm_sec,
                                    message.c_str());

    // If everything was ok with the snprintf call
    // We can not check it with the report_message because of the coding,
    // the sizes will not necessarily match with the return value of the snprintf()
    if(0 < snprintf_result)
    {
        emit NewStatusMessage(std::string(report_message.get()));
    }
    else
    {
        throw("ERROR! The snprintf() has reported a format error in Backend::ReportStatus()!");
    }
}

void Backend::OpenNetwokConnection(const std::string& port_name)
{
    bool result = false;

    if(serial_network_handler.Run(port_name))
    {
        result = true;
        ReportStatus("The connection \"" + port_name + "\" was successfully opened!");
    }
    else
    {
        ReportStatus("The connection \"" + port_name + "\" could not be opened...maybe wrong name?");
    }
    emit NetworkOperationFinished(port_name, result);
}

void Backend::CloseNetworkConnection(const std::string& port_name)
{
    serial_network_handler.Stop();

    ReportStatus("The connection \"" + port_name + "\" was successfully closed!");
    NetworkOperationFinished(port_name, true);
}

void Backend::RequestForDiagram(const DataIndexType& diagram_index)
{
    if(diagram_index < diagram_container.size())
    {
        emit ShowThisDiagram(diagram_container[diagram_index]);
    }
    else
    {
        std::string errorMessage = "ERROR! The requested diagram (index " + std::to_string(diagram_index )+ " ) does not exist!";
        throw errorMessage;
    }
}

void Backend::OpenFile(const std::string& path_to_file)
{
    #warning "Solve the std::filesystem linking error..."
    if(true)//std::filesystem::exists(std::filesystem::path(path_to_file)))
    {
        #warning "This later on needs to be solved with the protocol container method..."
        if(measurement_data_protocol.CanThisFileBeProcessed(path_to_file))
        {
            std::ifstream file_stream(path_to_file);
            #warning "Solve the std::filesystem linking error..."
            std::string file_name = "file_name";   //std::filesystem::path(path_to_file).filename();
            auto diagrams_from_file = measurement_data_protocol.ProcessData(file_name, file_stream);
            StoreNewDiagrams(diagrams_from_file);
        }
        else
        {
            #warning "handle this case..."
        }
    }
    else
    {
        #warning "handle this case..."
    }
}
