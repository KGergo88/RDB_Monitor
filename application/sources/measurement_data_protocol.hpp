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


#ifndef MEAUREMENT_DATA_PROTOCOL_HPP
#define MEAUREMENT_DATA_PROTOCOL_HPP


#include <vector>
#include <memory>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cctype>
#include <regex>
#include <type_traits>

#include <QFileInfo>

#include "i_protocol.hpp"
#include "diagram.hpp"


extern const std::string measurement_data_protocol_name;

class MeasurementDataProtocol : public I_Protocol
{
public:
    MeasurementDataProtocol() = default;
    virtual ~MeasurementDataProtocol() = default;

    MeasurementDataProtocol(const MeasurementDataProtocol&) = delete;
    MeasurementDataProtocol(MeasurementDataProtocol&&) = delete;

    MeasurementDataProtocol& operator=(const MeasurementDataProtocol&) = delete;
    MeasurementDataProtocol& operator=(MeasurementDataProtocol&&) = delete;

    virtual std::string GetProtocolName(void) override;
    virtual void ProcessNetworkData(std::istream& input_data) override;
    virtual std::string GetSupportedFileType(void) override { return Constants::native_file_extension; }
    virtual bool CanThisFileBeImportedFrom(const std::string path_to_file) override;
    virtual std::vector<DefaultDiagram> ImportFromFile(std::istream& input_stream) override;
    virtual bool CanThisFileBeExportedInto(const std::string path_to_file) override;
    virtual std::stringstream ExportToFile(const std::vector<DefaultDiagram>& diagrams_to_export) override;

private:
    struct Constants
    {
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

    struct ProcessingData
    {
        Constants::States state = Constants::States::WaitingForStartLine;
        DefaultDiagram actual_diagram;
    };

    std::vector<DefaultDiagram> ProcessData(std::istream& input_data, ProcessingData& processing_data);
    ProcessingData network_processing_data;
};


#endif // MEAUREMENT_DATA_PROTOCOL_HPP
