#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <functional>

#include "global.hpp"
#include "diagram.hpp"

#ifndef DATA_PROCESSOR_HPP
#define DATA_PROCESSOR_HPP

class DataProcessor
{
public:
    DataProcessor() {}

    DataProcessor(const DataProcessor&) = delete;
    DataProcessor(DataProcessor&&) = delete;

    DataProcessor& operator=(const DataProcessor&) = delete;
    DataProcessor& operator=(DataProcessor&&) = delete;

    std::shared_ptr<DiagramObject> ProcessData(std::istream& input_data)
    {
        std::shared_ptr<DiagramObject> diagram(nullptr);

        if(input_data)
        {
            std::string actual_line;

#warning "Add the date and time as a diagram title..."
#warning "Also add some error handling to other parts of the module and the whole program..."
            diagram = std::make_shared<DiagramObject>("Yoyo Ketszihh");

            while(std::getline(input_data, actual_line) && !input_data.eof())
            {
                if(std::string::npos != actual_line.find("<<<START>>>"))
                {
                    break;
                }
            }

            std::getline(input_data, actual_line);
            auto columns = ProcessHeadLine(actual_line);

            for(const auto& i : *columns)
            {
                diagram->AddNewDataLine(i);
            }

            DataIndexType data_point_x_coordinate = 0;
            while(std::getline(input_data, actual_line) && !input_data.eof())
            {
                if(std::string::npos != actual_line.find("<<<END>>>"))
                {
                    break;
                }

                auto data_points = ProcessDataLine(actual_line);

                if(columns->size() != data_points->size())
                {
                    diagram.reset();
                    break;
                }

                DataIndexType data_line_index = 0;
                for(auto& i : *data_points)
                {
                    diagram->AddNewDataPoint(data_line_index, DataPointObject(data_point_x_coordinate, i));
                    data_line_index++;
                }
                data_point_x_coordinate++;
            }
        }

        input_data.clear(); input_data.seekg(0);

        return diagram;
    }

private:
    std::shared_ptr<std::vector<std::string> > ProcessHeadLine(std::string& headline)
    {
        auto columns = std::make_shared<std::vector<std::string> >();

        headline.erase(std::remove_if(headline.begin(), headline.end(), std::bind(std::isspace<char>, std::placeholders::_1, std::locale::classic())), headline.end());

        std::size_t nameStartPos = 0;
        std::size_t nameEndPos = 0;
        while(std::string::npos != (nameEndPos = headline.find(',', nameStartPos)))
        {
            std::string columnTitle;

            columnTitle = headline.substr(nameStartPos, nameEndPos - nameStartPos);
            columns->emplace_back(columnTitle);
            nameStartPos = nameEndPos + 1;
        }

        return columns;
    }

    std::shared_ptr<std::vector<DataPointType> > ProcessDataLine(std::string& data_line)
    {
        auto data_points = std::make_shared<std::vector<DataPointType> >();

        data_line.erase(std::remove_if(data_line.begin(), data_line.end(), std::bind(std::isspace<char>, std::placeholders::_1, std::locale::classic())), data_line.end());

        std::size_t numberStartPos = 0;
        std::size_t numberEndPos = 0;
        while(std::string::npos != (numberEndPos = data_line.find(',', numberStartPos)))
        {
            std::string readNumber = data_line.substr(numberStartPos, numberEndPos - numberStartPos);
            data_points->push_back(static_cast<DataPointType>(atof(readNumber.c_str())));
            numberStartPos = numberEndPos + 1;
        }

        return data_points;
    }
};

#endif // DATA_PROCESSOR_HPP
