/*
 * File:   global.hpp
 * Author: gergo
 *
 * Created on August 27, 2017, 13:27 PM
 */

#include <cstdint>

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

using DataPointType = double;
using DataIndexType = uint64_t;

template <typename T_DATA_POINT, typename T_INDEX>
class Diagram;
using DiagramObject = Diagram<DataPointType, DataIndexType>;

template <typename T_DATA_POINT, typename T_INDEX>
class DataLine;
using DataLineObject = DataLine<DataPointType, DataIndexType>;

template <typename T_DATA_POINT>
class DataPoint;
using DataPointObject = DataPoint<DataPointType>;

#endif /* GLOBAL_HPP */
