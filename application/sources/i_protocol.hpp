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


#ifndef I_PROTOCOL_HPP
#define I_PROTOCOL_HPP


#include <string>
#include <vector>

#include "global.hpp"


class I_Protocol
{
public:
    virtual ~I_Protocol() = default;

    virtual std::string GetProtocolName(void) = 0;
    virtual std::vector<DiagramSpecialized> ProcessData(std::istream& input_data) = 0;
    virtual std::stringstream ExportData(const std::vector<DiagramSpecialized>& diagrams_to_export) = 0;
    virtual bool CanThisFileBeProcessed(const std::string path_to_file) = 0;
    virtual bool CanThisFileBeExportedInto(const std::string path_to_file) = 0;
    virtual std::string GetSupportedFileType(void) = 0;
};


#endif // I_PROTOCOL_HPP
