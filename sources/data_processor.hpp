#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cctype>

#include "global.hpp"
#include "diagram.hpp"

#ifndef DATA_PROCESSOR_HPP
#define DATA_PROCESSOR_HPP

class DataProcessor
{
public:
    static inline DataProcessor& GetInstance(void)
    {
        static DataProcessor Singleton;
        return Singleton;
    }

    DataProcessor(const DataProcessor&) = delete;
    DataProcessor(DataProcessor&&) = delete;

    DataProcessor& operator=(const DataProcessor&) = delete;
    DataProcessor& operator=(DataProcessor&&) = delete;

    std::shared_ptr<DiagramSpecialized> ProcessData(std::string data_source, std::istream& input_data)
    {
        bool data_start_line_was_found = false;
        bool data_end_line_was_found = false;

        auto current_date_and_time = std::time(0);
        auto diagram = std::make_shared<DiagramSpecialized>(data_source + " - " + std::string(ctime(&current_date_and_time)));

        std::string actual_line;
        while(std::getline(input_data, actual_line) && !input_data.eof())
        {
            if(std::string::npos != actual_line.find(DATA_START_LINE))
            {
                data_start_line_was_found = true;
                break;
            }
        }

        if(data_start_line_was_found)
        {
            std::getline(input_data, actual_line);
            auto columns = ProcessHeadLine(actual_line);
            if(columns)
            {
                for(const auto& i : *columns)
                {
                    diagram->AddNewDataLine(i);
                }

                DataPointType data_point_x_coordinate = 0;
                while(std::getline(input_data, actual_line) && !input_data.eof())
                {
                    if(std::string::npos != actual_line.find(DATA_END_LINE))
                    {
                        data_end_line_was_found = true;
                        break;
                    }

                    auto data_points = ProcessDataLine(actual_line);
                    if(data_points)
                    {
                        if(columns->size() != data_points->size())
                        {
                            break;
                        }

                        DataIndexType data_line_index = 0;
                        for(auto& i : *data_points)
                        {
                            diagram->AddNewDataPoint(data_line_index, DataPointSpecialized(data_point_x_coordinate, i));
                            data_line_index++;
                        }
                        data_point_x_coordinate++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        if(!(data_start_line_was_found && data_end_line_was_found))
        {
            std::cerr << "The input data could not be processed." << std::endl;
            diagram.reset();
        }

        input_data.clear(); input_data.seekg(0);

        return diagram;
    }

private:
    DataProcessor() = default;

    std::shared_ptr<std::vector<std::string> > ProcessHeadLine(std::string& headline)
    {
        auto columns = std::make_shared<std::vector<std::string> >();
#warning "This should be working in the same was in both OSs. Either include locale for Windows of use the cctype version on Linux."
#ifdef _WIN32
        headline.erase(std::remove_if(headline.begin(), headline.end(), std::isspace), headline.end());
#else
        headline.erase(std::remove_if(headline.begin(), headline.end(), std::bind(std::isspace<char>, std::placeholders::_1, std::locale::classic())), headline.end());
#endif

        std::size_t nameStartPos = 0;
        std::size_t nameEndPos = 0;
        while(std::string::npos != (nameEndPos = headline.find(',', nameStartPos)))
        {
            std::string columnTitle;

            columnTitle = headline.substr(nameStartPos, nameEndPos - nameStartPos);
            columns->emplace_back(columnTitle);
            nameStartPos = nameEndPos + 1;
        }
#warning "At least two from now one..."
        if(0 == columns->size())
        {
            std::cerr << "The headline could not be processed. Content:" << headline << std::endl;
            columns.reset();
        }

        return columns;
    }

    std::shared_ptr<std::vector<DataPointType> > ProcessDataLine(std::string& data_line)
    {
        auto data_points = std::make_shared<std::vector<DataPointType> >();
#warning "This should be working in the same was in both OSs. Either include locale for Windows of use the cctype version on Linux."
#ifdef _WIN32
        data_line.erase(std::remove_if(data_line.begin(), data_line.end(), std::isspace), data_line.end());
#else
        data_line.erase(std::remove_if(data_line.begin(), data_line.end(), std::bind(std::isspace<char>, std::placeholders::_1, std::locale::classic())), data_line.end());
#endif

        std::size_t numberStartPos = 0;
        std::size_t numberEndPos = 0;
        while(std::string::npos != (numberEndPos = data_line.find(',', numberStartPos)))
        {
            std::string readNumber = data_line.substr(numberStartPos, numberEndPos - numberStartPos);
            data_points->push_back(static_cast<DataPointType>(atof(readNumber.c_str())));
            numberStartPos = numberEndPos + 1;
        }

        if(0 == data_points->size())
        {
            std::cerr << "The dataline could not be processed. Content:" << data_line << std::endl;
            data_points.reset();
        }

        return data_points;
    }
};

#endif // DATA_PROCESSOR_HPP
