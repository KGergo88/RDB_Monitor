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

#define APPLICATION_NAME                ("RDB Diplomaterv Monitor V1.0")
#define SERIAL_PORT_DEFAULT_DEVICE_NAME ("/dev/ttyACM0")
#define DATA_START_LINE                 ("<<<START>>>")
#define DATA_END_LINE                   ("<<<END>>>")

#endif /* GLOBAL_HPP */
