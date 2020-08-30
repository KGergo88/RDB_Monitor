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



#include <string>
#include <vector>

#include "global.hpp"
#include "protocol_interface.hpp"



#ifndef DATA_PROCESSING_INTERFACE_HPP
#define DATA_PROCESSING_INTERFACE_HPP



class DataProcessingInterface : virtual ProtocolInterface
{
public:
    virtual std::vector<DiagramSpecialized> ProcessData(std::istream& input_data) = 0;
    virtual bool CanThisFileBeProcessed(const std::string path_to_file) = 0;
    virtual std::string GetSupportedFileType(void) = 0;
};

#endif // DATA_PROCESSING_INTERFACE_HPP
