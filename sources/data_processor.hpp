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



#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cctype>
#include <regex>

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

    std::vector<std::unique_ptr<DiagramSpecialized> > ProcessData(std::string data_source, std::istream& input_data);

private:
    enum class ProcessingStates : uint8_t
    {
        WaitingForStartLine,
        ProcessingHeadline,
        ProcessingDataLines
    };

#warning "This part needs to be reworked...the <<<START>>> and <<<END>>> strings are duplicated here and in the global.hpp"
    // REGEX strings to search the input data for valid measurement session
    std::string regex_start_line         = R"(^\s*<<<START>>>$)";
    std::string regex_headline           = R"(^\s*(\w+,){2,}$)";
    std::string regex_headline_analyzer  = R"(^\s*(\w+),)";
    std::string regex_data_line          = R"(^\s*(((?:\+|\-)?\d+),){2,}$)";
    std::string regex_data_line_analyzer = R"(^\s*((?:\+|\-)?\d+),)";
    std::string regex_end_line           = R"(^\s*<<<END>>>$)";

    DataProcessor() = default;

    std::shared_ptr<std::vector<std::string> > ProcessHeadLine(std::string& headline);
    std::shared_ptr<std::vector<DataPointType> > ProcessDataLine(std::string& data_line);
};

#endif // DATA_PROCESSOR_HPP
