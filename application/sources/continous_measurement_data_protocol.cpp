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



#include "continous_measurement_data_protocol.hpp"



ContinousMeasurementDataProtocol::ContinousMeasurementDataProtocol()
{
    state = Constants::States::WaitingForHeaderMessageStart;
}

std::string ContinousMeasurementDataProtocol::GetProtocolName(void)
{
    return std::string(Constants::protocol_name);
}

std::vector<DiagramSpecialized> ContinousMeasurementDataProtocol::ProcessData(std::istream& input_data)
{
    std::vector<DiagramSpecialized> assembled_diagrams;
    std::string received_data;
    std::string actual_line;

    while(std::getline(input_data, actual_line))
    {
        std::smatch match_results;

        // Removing the whitespaces from the actual line
        actual_line.erase(std::remove_if(actual_line.begin(), actual_line.end(), isspace), actual_line.end());

        try
        {
            switch(state)
            {
                case Constants::States::WaitingForHeaderMessageStart:
                    // If a header message start line was found
                    if(std::regex_match(actual_line, std::regex(Constants::Regex::header_start)))
                    {
                        state = Constants::States::ProcessingHeaderDiagramTitle;
                    }
                    break;
                case Constants::States::ProcessingHeaderDiagramTitle:
                    state = Constants::States::ProcessingHeaderDataLines;
                    // If a header message diagram title line was found
                    if(std::regex_search(actual_line, match_results, std::regex(Constants::Regex::header_diagram_title)))
                    {
                        // Then we create a diagram object with the title
                        actual_diagram = DiagramSpecialized(match_results[1]);
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
                        actual_diagram = DiagramSpecialized(current_date_and_time_string);
                        // Switching to the next state without a break --> a new line will NOT be fetched, because this line is the headline
                    }
                    // The falltrough is not an error in this case, this behaviour needed because there was no diagram title found, the actual_line contains the headline
                    [[fallthrough]];
                case Constants::States::ProcessingHeaderDataLines:
                    if(std::regex_match(actual_line, std::regex(Constants::Regex::header_datalines)))
                    {
                        std::string header_data_lines = actual_line;

                        // Extracting the title of the X axis
                        std::regex_search(header_data_lines, match_results, std::regex(Constants::Regex::header_dataline_x));
                        actual_diagram.SetAxisXTitle(match_results[1]);
                        header_data_lines = match_results.suffix().str();

                        // Extracting the title of the Y axis
                        while(std::regex_search(header_data_lines, match_results, std::regex(Constants::Regex::header_dataline_y)))
                        {
                            actual_diagram.AddNewDataLine(match_results[1], match_results[2]);
                            header_data_lines = match_results.suffix().str();
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
                    if(std::regex_match(actual_line, std::regex(Constants::Regex::header_end)))
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
                    if(std::regex_match(actual_line, std::regex(Constants::Regex::data_start)))
                    {
                        state = Constants::States::ProcessingDataMessageContent;
                    }
                    else if(std::regex_match(actual_line, std::regex(Constants::Regex::tail)))
                    {
                        assembled_diagrams.push_back(actual_diagram);
                        state = Constants::States::WaitingForHeaderMessageStart;
                    }
                    break;
                case Constants::States::ProcessingDataMessageContent:
                    if(std::regex_match(actual_line, std::regex(Constants::Regex::data_datalines)))
                    {
                        std::string data_line = actual_line;
                        DataIndexType column_index = 0;
                        DataPointType data_point_x_value = 0;

                        // Collecting the data from the dataline
                        while(std::regex_search(data_line, match_results, std::regex(Constants::Regex::data_datalines)))
                        {
                            if(0 == column_index)
                            {
                                std::stringstream stringstream(match_results[1]);
                                stringstream >> data_point_x_value;
                            }
                            else
                            {
                                if((column_index - 1) < actual_diagram.GetTheNumberOfDataLines())
                                {
                                    std::stringstream stringstream(match_results[1]);
                                    DataPointType data_point_y_value;
                                    stringstream >> data_point_y_value;
                                    actual_diagram.AddNewDataPoint((column_index - 1), DataPointSpecialized(data_point_x_value, data_point_y_value));
                                }
                                else
                                {
                                    state = Constants::States::WaitingForHeaderMessageStart;
                                    break;
                                }
                            }

                            ++column_index;
                            data_line = match_results.suffix().str();
                        }
                        if((column_index - 1) != actual_diagram.GetTheNumberOfDataLines())
                        {
                            state = Constants::States::WaitingForHeaderMessageStart;
                        }
                    }
                    else
                    {
                        if(std::regex_match(actual_line, std::regex(Constants::Regex::data_end)))
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
                    throw("The DataProcessor::ProcessData's statemachine switched to an unexpected state: " + std::to_string(static_cast<std::underlying_type<Constants::States>::type>(state)));
                    break;
            }
        }
        catch(const std::regex_error& exception)
        {
            throw("A regex exception was caught: " + std::to_string(exception.code()) + ": " + exception.what());
        }
    }

    return assembled_diagrams;
}

bool ContinousMeasurementDataProtocol::CanThisFileBeProcessed(const std::string path_to_file)
{
    bool bResult = false;

    std::string file_extension = QFileInfo(QString::fromStdString(path_to_file)).completeSuffix().toStdString();

    if(std::string(Constants::native_file_extension) == file_extension)
    {
        bResult = true;
    }

    return bResult;
}
