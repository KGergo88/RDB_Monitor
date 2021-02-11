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


#ifndef CONTINOUS_MEAUREMENT_DATA_PROTOCOL_HPP
#define CONTINOUS_MEAUREMENT_DATA_PROTOCOL_HPP


#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cctype>
#include <type_traits>

#include <QFileInfo>
#include <QRegularExpression>

#include "i_protocol.hpp"


extern const std::string continous_measurement_data_protocol_name;

class ContinousMeasurementDataProtocol : public I_Protocol
{
public:
    ContinousMeasurementDataProtocol();
    virtual ~ContinousMeasurementDataProtocol() = default;

    ContinousMeasurementDataProtocol(const ContinousMeasurementDataProtocol&) = delete;
    ContinousMeasurementDataProtocol(ContinousMeasurementDataProtocol&&) = delete;

    ContinousMeasurementDataProtocol& operator=(const ContinousMeasurementDataProtocol&) = delete;
    ContinousMeasurementDataProtocol& operator=(ContinousMeasurementDataProtocol&&) = delete;

    virtual std::string GetProtocolName(void) override;
    virtual void ProcessNetworkData(std::istream& input_data) override;
    virtual std::string GetSupportedFileType(void) override;
    virtual bool CanThisFileBeImportedFrom(const std::string path_to_file) override;
    virtual std::vector<DefaultDiagram> ImportFromFile(std::ifstream& file_tream) override;
    virtual std::stringstream ExportToFile(const std::vector<DefaultDiagram>& diagrams_to_export) override;
    virtual bool CanThisFileBeExportedInto(const std::string path_to_file) override;

private:
    struct Constants
    {
        enum class States : uint8_t
        {
            INVALID = 0,
            WaitingForHeaderMessageStart,
            ProcessingHeaderDiagramTitle,
            ProcessingHeaderDataLines,
            WaitingForHeaderMessageEnd,
            WaitingForDataMessageStart,
            ProcessingDataMessageContent,
        };
    };

    struct RegexPatterns
    {
        // REGEX patterns to search the input data for valid measurement session
        QRegularExpression header_start             = QRegularExpression(R"(^<CMDP_H>$)");
        QRegularExpression header_diagram_title     = QRegularExpression(R"(^<(?<diagram_title>.*)>$)");
        QRegularExpression header_datalines         = QRegularExpression(R"(^X:(?<x_title>[^,]*),(?<y_titles>(Y\d+:[^,]*,)+)$)");
        QRegularExpression header_dataline_y_titles = QRegularExpression(R"((?<id>Y\d+):(?<title>[^,]*),)");
        QRegularExpression header_end               = QRegularExpression(R"(^>CMDP_H<$)");
        QRegularExpression data_start               = QRegularExpression(R"(^<CMDP_D>$)");
        QRegularExpression data_content             = QRegularExpression(R"(^X:(?<x_value>\d+),(?<y_content>(Y\d+:\d+,)+)$)");
        QRegularExpression data_y_content           = QRegularExpression(R"((?<id>Y\d+):(?<value>\d+),)");
        QRegularExpression data_end                 = QRegularExpression(R"(^>CMDP_D<$)");
        QRegularExpression tail                     = QRegularExpression(R"(^<CMDP_T>$)");
        QRegularExpression reset                    = QRegularExpression(R"(^<CMDP_R>$)");
    };

    Constants::States state;
    DefaultDiagram actual_diagram;
    QModelIndex actual_model_index;
    RegexPatterns regex_patterns;
};


#endif // CONTINOUS_MEAUREMENT_DATA_PROTOCOL_HPP
