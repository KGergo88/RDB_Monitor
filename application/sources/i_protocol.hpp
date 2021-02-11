//==============================================================================//
//                                                                              //
//    RDB Monitor                                                               //
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


#ifndef I_PROTOCOL_HPP
#define I_PROTOCOL_HPP


#include <string>
#include <vector>
#include <QModelIndex>

#include "diagram.hpp"


class I_Protocol
{
public:
    // Callback types
    // Called when a network diagram got processed
    using diagram_collector_t = std::function<std::vector<QModelIndex>(const std::vector<DefaultDiagram>&)>;
    // Called when an update is available for a diagram
    using diagram_updater_t = std::function<void(const QModelIndex&, const DefaultDiagram&)>;
    // Called when an error was detected
    using error_reporter_t = std::function<void(const std::string&)>;

    virtual ~I_Protocol() = default;

    virtual void RegisterCallbacks(diagram_collector_t&& diagram_collector,
                                   diagram_updater_t&& diagram_updater,
                                   error_reporter_t&& error_reporter)
    {
        m_diagram_collector = diagram_collector;
        m_diagram_updater = diagram_updater;
        m_error_reporter = error_reporter;
    }

    // Returns the name of the protocol
    virtual std::string GetProtocolName(void) = 0;

    // Processes the data received from a network connection and reports the findings trough the callbacks
    virtual void ProcessNetworkData(std::istream& input_data) = 0;

    // Returns the file extensions that can be imported
    virtual std::string GetSupportedFileType(void) = 0;

    // Checks whether the file can be imported
    virtual bool CanThisFileBeImportedFrom(const std::string path_to_file) = 0;

    // Processes the content of the file and returns the found diagrams
    virtual std::vector<DefaultDiagram> ImportFromFile(std::ifstream& file_stream) = 0;

    // Checks whether data can be exported into the file
    virtual bool CanThisFileBeExportedInto(const std::string path_to_file) = 0;

    // Processes the received diagrams and returns a stream that can be written to a file
    virtual std::stringstream ExportToFile(const std::vector<DefaultDiagram>& diagrams_to_export) = 0;

protected:
    diagram_collector_t m_diagram_collector;
    diagram_updater_t m_diagram_updater;
    error_reporter_t m_error_reporter;
};

#endif // I_PROTOCOL_HPP
