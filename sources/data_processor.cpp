#include "data_processor.hpp"

std::vector<std::unique_ptr<DiagramSpecialized> > DataProcessor::ProcessData(std::string data_source, std::istream& input_data)
{
    std::unique_ptr<DiagramSpecialized> actual_diagram;
    std::vector<std::unique_ptr<DiagramSpecialized> > assembled_diagrams;
    ProcessingStates processing_state = ProcessingStates::WaitingForStartLine;

    // Processing the input stream until there is data available in it
    while(!input_data.eof())
    {
        std::string actual_line;
        std::smatch match_results;

        std::getline(input_data, actual_line);

        try
        {
            switch(processing_state)
            {
                case ProcessingStates::WaitingForStartLine:
                    // If a start line was found...
                    if(std::regex_match(actual_line, std::regex(regex_start_line)))
                    {
                        processing_state = ProcessingStates::ProcessingHeadline;
                    }
                    break;
                case ProcessingStates::ProcessingHeadline:
                    // If this is a headline but and a dataline
                    // (this is needed because with regex it is difficult to define the differences between the data and headlines)
                    if((std::regex_match(actual_line, std::regex(regex_headline))) &&
                       (!std::regex_match(actual_line, std::regex(regex_data_line))))
                    {
                        std::string headline = actual_line;
                        DataIndexType column_index = 0;

                        // Collecting the labels from the headline
                        while(std::regex_search(headline, match_results, std::regex(regex_headline_analyzer)))
                        {
                            if(0 == column_index)
                            {
                                auto current_date_and_time = std::time(0);
                                actual_diagram = std::make_unique<DiagramSpecialized>(data_source + " - " + std::string(ctime(&current_date_and_time)) , match_results[1]);
                            }
                            else
                            {
                                actual_diagram->AddNewDataLine(match_results[1]);
                            }

                            column_index++;
                            headline = match_results.suffix().str();
                        }

                        // A headline was found...
                        //actual_diagram = make_shared<DiagramSpecialized>(match_results.at());
                        processing_state = ProcessingStates::ProcessingDataLines;
                    }
                    else
                    {
                        processing_state = ProcessingStates::WaitingForStartLine;
                    }
                    break;
                case ProcessingStates::ProcessingDataLines:
                    if(std::regex_match(actual_line, std::regex(regex_data_line)))
                    {
                        std::string data_line = actual_line;
                        DataIndexType column_index = 0;
                        DataPointType data_point_x_value = 0;

                        // Collecting the labels from the headline
                        while(std::regex_search(data_line, match_results, std::regex(regex_data_line_analyzer)))
                        {
                            if(0 == column_index)
                            {
                                std::stringstream stringstream(match_results[1]);
                                stringstream >> data_point_x_value;
                            }
                            else
                            {
                                if((column_index - 1) < actual_diagram->GetTheNumberOfDataLines())
                                {
                                    std::stringstream stringstream(match_results[1]);
                                    DataPointType data_point_y_value;
                                    stringstream >> data_point_y_value;
                                    actual_diagram->AddNewDataPoint((column_index - 1), DataPointSpecialized(data_point_x_value, data_point_y_value));
                                }
                                else
                                {
                                    processing_state = ProcessingStates::WaitingForStartLine;
                                }
                            }

                            column_index++;
                            data_line = match_results.suffix().str();
                        }
                        if((column_index - 1) != actual_diagram->GetTheNumberOfDataLines())
                        {
                            processing_state = ProcessingStates::WaitingForStartLine;
                        }
                    }
                    else
                    {
                        if(std::regex_match(actual_line, std::regex(regex_end_line)))
                        {
                            assembled_diagrams.push_back(std::move(actual_diagram));
                        }
                        processing_state = ProcessingStates::WaitingForStartLine;
                    }
                    break;
                default:
                    #warning "This should never happen, throw an exception or terminate the program or something..."
                    break;
            }
        }
        catch(const std::regex_error& exception)
        {
            std::cerr << "A regex exception was caught: " << exception.code() << ": " << exception.what() << std::endl;
        }
    }

    return assembled_diagrams;
}
