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
#include "continous_measurement_data_protocol.hpp"



Backend::Backend() : QObject(), gui_signal_interface(nullptr)
{
    available_connection_handlers.append(QString(serial_port_connection_name));

    available_protocol_handlers.append(QString(measurement_data_protocol_name));
    // Deactivate the CMDP before merging as this Protocol is not fully implemented yet!
    //available_protocol_handlers.append(QString(continous_measurement_data_protocol_name));

    file_handlers.push_back(std::make_shared<MeasurementDataProtocol>());
}

void Backend::RegisterGuiSignalInterface(I_GuiSignal* new_gui_signal_interface)
{
    if(new_gui_signal_interface)
    {
        gui_signal_interface = new_gui_signal_interface;

        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface),  SIGNAL(OpenNetworkConnection(const ConnectionRequestData&)),
                         this,                                          SLOT(OpenNetworkConnection(const ConnectionRequestData&)));
        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface),  SIGNAL(CloseNetworkConnection(const QString&)),
                         this,                                          SLOT(CloseNetworkConnection(const QString&)));
        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface),  SIGNAL(RequestForDiagram(const QModelIndex&)),
                         this,                                          SLOT(RequestForDiagram(const QModelIndex&)));
        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface),  SIGNAL(ImportFile(const std::string&)),
                         this,                                          SLOT(ImportFile(const std::string&)));
        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface),  SIGNAL(ExportFileShowCheckBoxes(void)),
                         this,                                          SLOT(ExportFileShowCheckBoxes(void)));
        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface),  SIGNAL(ExportFileHideCheckBoxes(void)),
                         this,                                          SLOT(ExportFileHideCheckBoxes(void)));
        QObject::connect(dynamic_cast<QObject*>(gui_signal_interface),  SIGNAL(ExportFileStoreCheckedDiagrams(const std::string&)),
                         this,                                          SLOT(ExportFileStoreCheckedDiagrams(const std::string&)));
    }
    else
    {
        std::string errorMessage = "There was no gui_signal_interface set in Backend::RegisterGuiSignalInterface!";
        throw errorMessage;
    }
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

void Backend::StoreNetworkDiagrams(const QString& connection_name, std::vector<DiagramSpecialized>& new_diagrams)
{
    StoreDiagrams(new_diagrams,
        [&](const DiagramSpecialized& diagram_to_add) -> QModelIndex
        {
            return diagram_container.AddDiagramFromNetwork(connection_name.toStdString(), diagram_to_add);
        });
}

void Backend::StoreFileDiagrams(const std::string& file_name, const std::string& file_path, std::vector<DiagramSpecialized>& new_diagrams)
{
    StoreDiagrams(new_diagrams,
        [&](const DiagramSpecialized& diagram_to_add) -> QModelIndex
        {
            return diagram_container.AddDiagramFromFile(file_name, file_path, diagram_to_add);
        });
}

std::vector<std::string> Backend::GetSupportedFileExtensions(void)
{
    std::vector<std::string> result;

    for(auto const& protocol_handler : file_handlers)
    {
        std::string supported_file_type = protocol_handler->GetSupportedFileType();
        if(!supported_file_type.empty())
        {
            result.push_back(supported_file_type);
        }
    }

    return result;
}

QStringList Backend::GetAvailableConnections(void)
{
    return available_connection_handlers;
}

QStringList Backend::GetAvailableProtocols(void)
{
    return available_protocol_handlers;
}

void Backend::OpenNetworkConnection(const ConnectionRequestData& request_data)
{
    auto unique_user_defined_name = makeUserDefinedConnectionNameUnique(request_data.user_defined_name);
    if(request_data.user_defined_name != unique_user_defined_name)
    {
        std::string status_message = "The connection name was updated: \"" + request_data.user_defined_name.toStdString() + "\" --> \"" + unique_user_defined_name.toStdString() + "\"";
        emit ReportStatus(status_message);
    }

    auto connection = ConnectionFactory::make(request_data.connection);
    auto connection_settings = request_data.connection_settings;
    auto protocol = ProtocolFactory::make(request_data.protocol);
    network_handlers[unique_user_defined_name] = std::make_shared<NetworkHandler>(unique_user_defined_name,
                                                                                  connection,
                                                                                  connection_settings,
                                                                                  protocol,
                                                                                  std::bind(&Backend::StoreNetworkDiagrams, this, std::placeholders::_1, std::placeholders::_2),
                                                                                  std::bind(&Backend::ReportStatus, this, std::placeholders::_1));

    std::string status_message = "The connection \"" + unique_user_defined_name.toStdString() + "\" was successfully opened!";
    ReportStatus(status_message);

    auto active_connections = getActiveConnections();
    emit ListOfActiveConnectionsChanged(active_connections);
}

void Backend::CloseNetworkConnection(const QString& user_defined_name)
{
    network_handlers[user_defined_name]->Stop();
    network_handlers.erase(user_defined_name);

    std::string status_message = "The connection \"" + user_defined_name.toStdString() + "\" was successfully closed!";
    ReportStatus(status_message);

    auto active_connections = getActiveConnections();
    emit ListOfActiveConnectionsChanged(active_connections);
}

void Backend::RequestForDiagram(const QModelIndex& model_index)
{
    DiagramSpecialized* first_diagram = diagram_container.GetDiagram(model_index);
    if(first_diagram)
    {
        emit ShowThisDiagram(*first_diagram);
    }
}

void Backend::ImportFile(const std::string& path_to_file)
{
    QFileInfo file_info(QString::fromStdString(path_to_file));
    if(file_info.exists())
    {
        std::string file_name = file_info.fileName().toStdString();
        if(!diagram_container.IsThisFileAlreadyStored(file_name, path_to_file))
        {
            bool the_file_was_processed = false;
            for(auto const& file_handler : file_handlers)
            {
                if(file_handler->CanThisFileBeProcessed(path_to_file))
                {
                    std::ifstream file_stream(path_to_file);
                    auto diagrams_from_file = file_handler->ProcessData(file_stream);
                    StoreFileDiagrams(file_name, path_to_file, diagrams_from_file);

                    // Updating the configuration with the folder of the file that was imported
                    configuration.ImportFolder(file_info.absoluteDir().absolutePath().toStdString());

                    ReportStatus("The file \"" + path_to_file + "\" was successfully opened!");
                    the_file_was_processed = true;
                    // We have imported the diagrams with this protocol, we can break out of the loop
                    break;
                }
            }

            if(!the_file_was_processed)
            {
                ReportStatus("ERROR! The MeasurementDataProtocol cannot process the file: \"" + path_to_file + "\" because it has a wrong extension!");
            }
        }
        else
        {
            ReportStatus("The file \"" + path_to_file + "\" was already imported and will not be imported again!");
        }
    }
    else
    {
        ReportStatus("ERROR! The path \"" + path_to_file + "\" does not exist!");
    }
}

void Backend::ExportFileShowCheckBoxes(void)
{
    diagram_container.ShowCheckBoxes();
}

void Backend::ExportFileHideCheckBoxes(void)
{
    diagram_container.HideCheckBoxes();
}

void Backend::ExportFileStoreCheckedDiagrams(const std::string& path_to_file)
{
    for(auto const& protocol_handler : file_handlers)
    {
        if(protocol_handler->CanThisFileBeExportedInto(path_to_file))
        {
            auto checked_diagrams = diagram_container.GetCheckedDiagrams();
            if(checked_diagrams.size())
            {
                auto exported_data = protocol_handler->ExportData(checked_diagrams);

                std::ofstream output_file_stream(path_to_file, (std::ofstream::out | std::ofstream::trunc));
                output_file_stream << exported_data.rdbuf();

                // Updating the configuration with the folder of the file that was exported
                configuration.ExportFolder(QFileInfo(QString::fromStdString(path_to_file)).absoluteDir().absolutePath().toStdString());

                ReportStatus("The selected diagrams were successfully written to \"" + path_to_file + "\"!");
                // We have stored the diagrams with this data_exporter, we can break out of the loop
                break;
            }
            else
            {
                ReportStatus("No diagram was selected! Nothing was exported!");
            }
        }
        else
        {
            ReportStatus("ERROR! The MeasurementDataProtocol cannot save diagrams into the file: \"" + path_to_file + "\" because it has a wrong extension!");
        }
    }
}

void Backend::StoreDiagrams(std::vector<DiagramSpecialized>& new_diagrams, const std::function<QModelIndex(const DiagramSpecialized&)> storage_logic)
{
    auto container_is_empty = (0 == diagram_container.GetNumberOfDiagrams());

    // Adding the diagrams to the diagram_container
    for(const auto& i : new_diagrams)
    {
        // Calling the logic that does the storage for a single diagram, this is provided by the caller
        auto recently_added_diagram = storage_logic(i);

        // Displaying the diagram if this was the first
        if(container_is_empty)
        {
            container_is_empty = false;

            DiagramSpecialized* first_diagram = diagram_container.GetDiagram(recently_added_diagram);
            if(first_diagram)
            {
                emit ShowThisDiagram(*first_diagram);
            }
        }
    }

    ReportStatus(std::to_string(new_diagrams.size()) + " new diagram was added to the list.");
}

QStringList Backend::getActiveConnections(void)
{
    QStringList active_connections;

    for(const auto& i : network_handlers)
    {
        active_connections.append(i.first);
    }

    return active_connections;
}

QString Backend::makeUserDefinedConnectionNameUnique(const QString& user_defined_name)
{
    QString result = user_defined_name;

    if(network_handlers.count(user_defined_name))
    {
        // The name already exists, it needs to be updated!
        // We need to go into recursion here because the updated name may already exist as well!
        result = makeUserDefinedConnectionNameUnique(user_defined_name + "_copy");
    }

    return result;
}
