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

#include <QFileInfo>

#include "global.hpp"
#include "protocol_interface.hpp"
#include "data_processing_interface.hpp"
#include "data_exporting_interface.hpp"
#include "diagram.hpp"



#ifndef MEAUREMENT_DATA_PROTOCOL_HPP
#define MEAUREMENT_DATA_PROTOCOL_HPP



class MeasurementDataProtocol : public DataProcessingInterface, public DataExportingInterface
{
public:
    MeasurementDataProtocol();
    virtual ~MeasurementDataProtocol() = default;

    MeasurementDataProtocol(const MeasurementDataProtocol&) = delete;
    MeasurementDataProtocol(MeasurementDataProtocol&&) = delete;

    MeasurementDataProtocol& operator=(const MeasurementDataProtocol&) = delete;
    MeasurementDataProtocol& operator=(MeasurementDataProtocol&&) = delete;

    virtual std::string GetProtocolName(void) override;
    virtual std::vector<DiagramSpecialized> ProcessData(std::istream& input_data) override;
    virtual bool CanThisFileBeProcessed(const std::string path_to_file) override;
    virtual std::string GetSupportedFileType(void) override {return Constants::native_file_extension;}
    virtual std::stringstream ExportData(const std::vector<DiagramSpecialized>& diagrams_to_export) override;
    virtual bool CanThisFileBeExportedInto(const std::string path_to_file) override;

private:
    struct Constants
    {
        static constexpr char protocol_name[] = "Measurement Data Protocol MDP";
        static constexpr char native_file_extension[] = "mdp";

        enum class States : uint8_t
        {
            INVALID = 0,
            WaitingForStartLine,
            ProcessingTitleLine,
            ProcessingHeadline,
            ProcessingDataLines
        };

        struct Regex
        {
            // REGEX strings to search the input data for valid measurement session
            static constexpr char start_line[]         = R"(^\s*<<<START>>>$)";
            static constexpr char title_line[]         = R"(^<(.*)>$)";
            static constexpr char headline[]           = R"(^\s*(\w+,){2,}$)";
            static constexpr char headline_analyzer[]  = R"(^\s*(\w+),)";
            static constexpr char data_line[]          = R"(^\s*(((?:\+|\-)?\d+),){2,}$)";
            static constexpr char data_line_analyzer[] = R"(^\s*((?:\+|\-)?\d+),)";
            static constexpr char end_line[]           = R"(^\s*<<<END>>>$)";
        };

        struct Export
        {
            static constexpr char start_line[]          = "<<<START>>>";
            static constexpr char end_line[]            = "<<<END>>>";
            static constexpr char diagram_title_start[] = "<";
            static constexpr char diagram_title_end[]   = ">";
            static constexpr char element_separator     = ',';
        };
    };

    Constants::States state;
    DiagramSpecialized actual_diagram;
};



#endif // MEAUREMENT_DATA_PROTOCOL_HPP
