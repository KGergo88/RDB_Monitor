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


#include "continous_measurement_data_protocol.hpp"
#include "data_point.hpp"


extern const std::string continous_measurement_data_protocol_name = "Continous Measurement Data Protocol - CMDP";


ContinousMeasurementDataProtocol::ContinousMeasurementDataProtocol()
{
    state = Constants::States::WaitingForHeaderMessageStart;
}

std::string ContinousMeasurementDataProtocol::GetProtocolName(void)
{
    return continous_measurement_data_protocol_name;
}

void ContinousMeasurementDataProtocol::ProcessNetworkData(std::istream& input_data)
{
    std::vector<DefaultDiagram> assembled_diagrams;
    std::string actual_line_std_string;

    while(std::getline(input_data, actual_line_std_string))
    {
        QRegularExpressionMatch match;
        QString actual_line = QString::fromStdString(actual_line_std_string);

        switch(state)
        {
            case Constants::States::WaitingForHeaderMessageStart:
                // If a header message start line was found
                match = regex_patterns.header_start.match(actual_line);
                if(match.hasMatch())
                {
                    state = Constants::States::ProcessingHeaderDiagramTitle;
                }
                break;

            case Constants::States::ProcessingHeaderDiagramTitle:
                state = Constants::States::ProcessingHeaderDataLines;
                // If a header message diagram title line was found
                match = regex_patterns.header_diagram_title.match(actual_line);
                if(match.hasMatch())
                {
                    auto diagram_title = match.captured("diagram_title");
                    // Then we create a diagram object with the title
                    actual_diagram = DefaultDiagram(diagram_title.toStdString());
                    // Switching to the next state with a break --> a new line will be fetched
                    break;
                }
                else
                {
                    // No title was found, we will generate a title from the current date and time and create a diagram object with it
                    auto current_date_and_time = std::time(nullptr);
                    std::string current_date_and_time_string = ctime(&current_date_and_time);
                    // The ctime adds an extra newline to the string, this needs to be removed
                    current_date_and_time_string.pop_back();
                    actual_diagram = DefaultDiagram(current_date_and_time_string);
                    // Switching to the next state without a break --> a new line will NOT be fetched, because this line is the headline
                }
                // In both cases the actual model index needs to be invalidated as we are starting a new diagram
                actual_model_index = QModelIndex();
                // The falltrough is not an error in this case, this behaviour needed because there was no diagram title found, the actual_line contains the headline

            [[fallthrough]];
            case Constants::States::ProcessingHeaderDataLines:
                match = regex_patterns.header_datalines.match(actual_line);
                if(match.hasMatch())
                {
                    // Extracting the title of the X axis
                    auto x_title = match.captured("x_title");
                    actual_diagram.SetAxisXTitle(x_title.toStdString());

                    // Extracting the titles of the Y axis
                    auto y_titles = match.captured("y_titles");
                    QRegularExpressionMatchIterator regex_iterator = regex_patterns.header_dataline_y_titles.globalMatch(y_titles);
                    while(regex_iterator.hasNext())
                    {
                        match = regex_iterator.next();
                        auto y_id = match.captured("id");
                        auto y_title = match.captured("title");

                        // Adding the new dataline to the diagram, if we fail (because of duplicate IDs) then we drop this diagram
                        if(!actual_diagram.AddNewDataLine(y_id.toStdString(), y_title.toStdString()))
                        {
                            state = Constants::States::WaitingForHeaderMessageStart;
                        }
                    }
                    state = Constants::States::WaitingForHeaderMessageEnd;
                }
                else
                {
                    state = Constants::States::WaitingForHeaderMessageStart;
                }
                break;

            case Constants::States::WaitingForHeaderMessageEnd:
                // If a header message end line was found
                match = regex_patterns.header_end.match(actual_line);
                if(match.hasMatch())
                {
                    state = Constants::States::WaitingForDataMessageStart;
                }
                else
                {
                    state = Constants::States::WaitingForHeaderMessageStart;
                }
                break;

            case Constants::States::WaitingForDataMessageStart:
                // If a header message end line was found
                match = regex_patterns.data_start.match(actual_line);
                if(match.hasMatch())
                {
                    state = Constants::States::ProcessingDataMessageContent;
                }
                match = regex_patterns.tail.match(actual_line);
                if(match.hasMatch())
                {
                    assembled_diagrams.push_back(actual_diagram);
                    state = Constants::States::WaitingForHeaderMessageStart;
                }
                break;

            case Constants::States::ProcessingDataMessageContent:
                match = regex_patterns.data_content.match(actual_line);
                if(match.hasMatch())
                {
                    auto x_value = match.captured("x_value");
                    auto y_values = match.captured("y_content");

                    QRegularExpressionMatchIterator regex_iterator = regex_patterns.data_y_content.globalMatch(y_values);
                    while(regex_iterator.hasNext())
                    {
                        match = regex_iterator.next();
                        auto y_id = match.captured("id");
                        auto y_value = match.captured("value");

                        // Checking whether the dataline existsm if not we are dropping this diagram
                        if(actual_diagram.HasDataLine(y_id.toStdString()))
                        {
                            actual_diagram.AddNewDataPoint(y_id.toStdString(), DefaultDiagram::DataPoint_t(x_value.toDouble(), y_value.toDouble()));
                        }
                        else
                        {
                            state = Constants::States::WaitingForHeaderMessageStart;
                        }
                    }
                }
                else
                {
                    match = regex_patterns.data_end.match(actual_line);
                    if(match.hasMatch())
                    {
                        state = Constants::States::WaitingForDataMessageStart;
                    }
                    else
                    {
                        state = Constants::States::WaitingForHeaderMessageStart;
                    }
                }
                break;

            default:
                state = Constants::States::WaitingForHeaderMessageStart;
                throw("The ContinousMeasurementDataProtocol::ProcessNetworkData's statemachine switched to an unexpected state: "
                      + std::to_string(static_cast<std::underlying_type<Constants::States>::type>(state)));
                break;
        }
    }

    // If the actual model index is valid then this is an update and not a new diagram
    if(actual_model_index.isValid())
    {
        // We should not have assembled diagrams here, only an update to the actual diagram
        if(!assembled_diagrams.empty())
        {
            throw("The ContinousMeasurementDataProtocol::ProcessNetworkData detected an error: the actual_model_index was valid but the assembled_diagrams was not empty!");
        }

        // Sending the updated diagram
        if(m_diagram_updater)
        {
            m_diagram_updater(actual_model_index, actual_diagram);
        }
    }
    else
    {
        // If the model index is not valid then we are sending new diagrams
        if(m_diagram_collector)
        {
            auto model_indexes = m_diagram_collector(assembled_diagrams);

            // If we have the waiting for header start state here then we either had a message error
            // or a tail message was received. In both cases the last diagram is finished. This means that the
            // model index does not need to be stored as no update will be sent anymore.
            if(state != Constants::States::WaitingForHeaderMessageStart)
            {
                // We need to only store the last model index,
                // because we are only going to be able to send updates to the last diagram.
                actual_model_index = model_indexes.back();
            }
        }
    }
}

std::string ContinousMeasurementDataProtocol::GetSupportedFileType(void)
{
    // The CMDP protocol does not support any file types
    return std::string();
}

bool ContinousMeasurementDataProtocol::CanThisFileBeImportedFrom(const std::string path_to_file)
{
    (void) path_to_file;
    // The CMDP protocol does not support file processing
    return false;
}

std::vector<DefaultDiagram> ContinousMeasurementDataProtocol::ImportFromFile(std::ifstream& file_stream)
{
    (void) file_stream;
    // The CMDP protocol does not support file processing
    return std::vector<DefaultDiagram>();
}

bool ContinousMeasurementDataProtocol::CanThisFileBeExportedInto(const std::string path_to_file)
{
    (void) path_to_file;
    // The CMDP protocol does not support storing into files
    return false;
}

std::stringstream ContinousMeasurementDataProtocol::ExportToFile(const std::vector<DefaultDiagram>& diagrams_to_export)
{
    (void) diagrams_to_export;
    throw("The Continous Measurement Protocol does not support exporting into files!");
}
