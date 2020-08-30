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



#include "measurement_data_protocol.hpp"



MeasurementDataProtocol::MeasurementDataProtocol()
{
    state = Constants::States::WaitingForStartLine;
}

std::string MeasurementDataProtocol::GetProtocolName(void)
{
    return std::string(Constants::protocol_name);
}

std::vector<DiagramSpecialized> MeasurementDataProtocol::ProcessData(std::istream& input_data)
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
                case Constants::States::WaitingForStartLine:
                    // If a start line was found...
                    if(std::regex_match(actual_line, std::regex(Constants::Regex::start_line)))
                    {
                        state = Constants::States::ProcessingTitleLine;
                    }
                    break;
                case Constants::States::ProcessingTitleLine:
                    // In any case, we will switch to the next state
                    state = Constants::States::ProcessingHeadline;
                    // If this is a diagram title line
                    if(std::regex_search(actual_line, match_results, std::regex(Constants::Regex::title_line)))
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
                case Constants::States::ProcessingHeadline:
                    // If this is a headline but not a dataline
                    // (this is needed because with regex it is difficult to define the differences between the data and headlines)
                    if((std::regex_match(actual_line, std::regex(Constants::Regex::headline))) &&
                       (!std::regex_match(actual_line, std::regex(Constants::Regex::data_line))))
                    {
                        std::string headline = actual_line;
                        DataIndexType column_index = 0;

                        // Collecting the labels from the headline
                        while(std::regex_search(headline, match_results, std::regex(Constants::Regex::headline_analyzer)))
                        {
                            if(0 == column_index)
                            {
                                actual_diagram.SetAxisXTitle(match_results[1]);
                            }
                            else
                            {
                                actual_diagram.AddNewDataLine(match_results[1]);
                            }

                            ++column_index;
                            headline = match_results.suffix().str();
                        }

                        state = Constants::States::ProcessingDataLines;
                    }
                    else
                    {
                        state = Constants::States::WaitingForStartLine;
                    }
                    break;
                case Constants::States::ProcessingDataLines:
                    if(std::regex_match(actual_line, std::regex(Constants::Regex::data_line)))
                    {
                        std::string data_line = actual_line;
                        DataIndexType column_index = 0;
                        DataPointType data_point_x_value = 0;

                        // Collecting the data from the dataline
                        while(std::regex_search(data_line, match_results, std::regex(Constants::Regex::data_line_analyzer)))
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
                                    state = Constants::States::WaitingForStartLine;
                                    break;
                                }
                            }

                            ++column_index;
                            data_line = match_results.suffix().str();
                        }
                        if((column_index - 1) != actual_diagram.GetTheNumberOfDataLines())
                        {
                            state = Constants::States::WaitingForStartLine;
                        }
                    }
                    else
                    {
                        if(std::regex_match(actual_line, std::regex(Constants::Regex::end_line)))
                        {
                            assembled_diagrams.push_back(actual_diagram);
                        }
                        state = Constants::States::WaitingForStartLine;
                    }
                    break;
                default:
                    state = Constants::States::WaitingForStartLine;
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

bool MeasurementDataProtocol::CanThisFileBeProcessed(const std::string path_to_file)
{
    bool bResult = false;

    std::string file_extension = QFileInfo(QString::fromStdString(path_to_file)).completeSuffix().toStdString();

    if(std::string(Constants::native_file_extension) == file_extension)
    {
        bResult = true;
    }

    return bResult;
}

std::stringstream MeasurementDataProtocol::ExportData(const std::vector<DiagramSpecialized>& diagrams_to_export)
{
    std::stringstream exported_data;

    for(auto const& diagram : diagrams_to_export)
    {
        exported_data << Constants::Export::start_line << std::endl;

        exported_data << Constants::Export::diagram_title_start << diagram.GetTitle() << Constants::Export::diagram_title_end << std::endl;

        auto number_of_data_lines = diagram.GetTheNumberOfDataLines();
        if(0 < number_of_data_lines)
        {
            exported_data << diagram.GetAxisXTitle() << Constants::Export::element_separator;
            for(std::size_t data_line_index = 0; data_line_index < number_of_data_lines; data_line_index++)
            {
                exported_data << diagram.GetDataLineTitle(data_line_index) << Constants::Export::element_separator;
            }
            exported_data << std::endl;

            auto number_of_data_points = diagram.GetTheNumberOfDataPoints(0);
            for(std::size_t data_point_index = 0; data_point_index < number_of_data_points; data_point_index++)
            {
                exported_data << diagram.GetDataPoint(0, data_point_index).GetX() << Constants::Export::element_separator;
                for(std::size_t data_line_index = 0; data_line_index < number_of_data_lines; data_line_index++)
                {
                    exported_data << diagram.GetDataPoint(data_line_index, data_point_index).GetY() << Constants::Export::element_separator;
                }
                exported_data << std::endl;
            }
        }

        exported_data << Constants::Export::end_line << std::endl << std::endl;
    }

    return exported_data;
}

bool MeasurementDataProtocol::CanThisFileBeExportedInto(const std::string path_to_file)
{
    bool bResult = false;

    std::string file_extension = QFileInfo(QString::fromStdString(path_to_file)).completeSuffix().toStdString();

    if(std::string(Constants::native_file_extension) == file_extension)
    {
        bResult = true;
    }

    return bResult;
}
