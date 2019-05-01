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
#include <sstream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cctype>
#include <regex>
#include <type_traits>
#include <filesystem>

#include "global.hpp"
#include "data_processing_interface.hpp"
#include "diagram.hpp"



#ifndef MEAUREMENT_DATA_PROTOCOL_HPP
#define MEAUREMENT_DATA_PROTOCOL_HPP



class MeasurementDataProtocol : public DataProcessingInterface
{
public:
    MeasurementDataProtocol();
    virtual ~MeasurementDataProtocol() = default;

    MeasurementDataProtocol(const MeasurementDataProtocol&) = delete;
    MeasurementDataProtocol(MeasurementDataProtocol&&) = delete;

    MeasurementDataProtocol& operator=(const MeasurementDataProtocol&) = delete;
    MeasurementDataProtocol& operator=(MeasurementDataProtocol&&) = delete;

    std::string GetProtocolName(void) override;
    std::vector<DiagramSpecialized> ProcessData(std::istream& input_data) override;
    bool CanThisFileBeProcessed(const std::string path_to_file) override;

private:
    enum class ProcessingStates : uint8_t
    {
        WaitingForStartLine,
        ProcessingHeadline,
        ProcessingDataLines
    };

    // REGEX strings to search the input data for valid measurement session
    const std::string regex_start_line         = R"(^\s*<<<START>>>$)";
    const std::string regex_headline           = R"(^\s*(\w+,){2,}$)";
    const std::string regex_headline_analyzer  = R"(^\s*(\w+),)";
    const std::string regex_data_line          = R"(^\s*(((?:\+|\-)?\d+),){2,}$)";
    const std::string regex_data_line_analyzer = R"(^\s*((?:\+|\-)?\d+),)";
    const std::string regex_end_line           = R"(^\s*<<<END>>>$)";

    ProcessingStates processing_state;
    DiagramSpecialized actual_diagram;
};



#endif // MEAUREMENT_DATA_PROTOCOL_HPP
