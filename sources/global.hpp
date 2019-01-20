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



#include <cstdint>



#ifndef GLOBAL_HPP
#define GLOBAL_HPP



using DataPointType = double;
using DataIndexType = std::size_t;

template <typename T_DATA_POINT, typename T_INDEX>
class Diagram;
using DiagramSpecialized = Diagram<DataPointType, DataIndexType>;

template <typename T_DATA_POINT, typename T_INDEX>
class DataLine;
using DataLineSpecialized = DataLine<DataPointType, DataIndexType>;

template <typename T_DATA_POINT>
class DataPoint;
using DataPointSpecialized = DataPoint<DataPointType>;

#define APPLICATION_NAME                ("RDB Diplomaterv Monitor v1.0.2")

#ifdef _WIN32
    #define SERIAL_PORT_DEFAULT_PORT_NAME ("COM3")
#else
    #define SERIAL_PORT_DEFAULT_PORT_NAME ("/dev/ttyACM0")
#endif

constexpr uint32_t SERIAL_PORT_DEFAULT_BAUDRATE = 115200;
constexpr std::size_t SERIAL_PORT_MAX_READ_LENGTH_IN_BYTES = 100 * 1024;

#define DATA_START_LINE                 ("<<<START>>>")
#define DATA_END_LINE                   ("<<<END>>>")



#endif /* GLOBAL_HPP */
