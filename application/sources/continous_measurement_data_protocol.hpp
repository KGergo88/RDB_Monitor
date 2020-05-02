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
#include "data_processing_interface.hpp"
#include "diagram.hpp"



#ifndef CONTINOUS_MEAUREMENT_DATA_PROTOCOL_HPP
#define CONTINOUS_MEAUREMENT_DATA_PROTOCOL_HPP



class ContinousMeasurementDataProtocol : public DataProcessingInterface
{
public:
    ContinousMeasurementDataProtocol();
    virtual ~ContinousMeasurementDataProtocol() = default;

    ContinousMeasurementDataProtocol(const ContinousMeasurementDataProtocol&) = delete;
    ContinousMeasurementDataProtocol(ContinousMeasurementDataProtocol&&) = delete;

    ContinousMeasurementDataProtocol& operator=(const ContinousMeasurementDataProtocol&) = delete;
    ContinousMeasurementDataProtocol& operator=(ContinousMeasurementDataProtocol&&) = delete;

    std::string GetProtocolName(void) override;
    std::vector<DiagramSpecialized> ProcessData(std::istream& input_data) override;
    bool CanThisFileBeProcessed(const std::string path_to_file) override;
    std::string GetSupportedFileType(void) override {return Constants::native_file_extension;}

private:
    struct Constants
    {
        static constexpr char protocol_name[] = "Continous Measurement Data Protocol CMDP";
        static constexpr char native_file_extension[] = "cmdp";

        enum class States : uint8_t
        {
            INVALID = 0,
            WaitingForTransmission,
            HeaderStartReceived,
            HeaderContentReceived,
            HeaderEndReceived,
            DataStartReceived,
            DataEndReceived
        };
    };

    Constants::States state;
    DiagramSpecialized actual_diagram;
};



#endif // CONTINOUS_MEAUREMENT_DATA_PROTOCOL_HPP
