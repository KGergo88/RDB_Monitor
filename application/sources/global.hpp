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


#ifndef GLOBAL_HPP
#define GLOBAL_HPP


#include <cstddef>
#include <cstdint>


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

#define APPLICATION_NAME                ("RDB Monitor v2.1.0")


#endif /* GLOBAL_HPP */
