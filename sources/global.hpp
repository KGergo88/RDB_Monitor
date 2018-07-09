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
using DiagramSpecialized = Diagram<DataPointType, DataIndexType>;

template <typename T_DATA_POINT, typename T_INDEX>
class DataLine;
using DataLineSpecialized = DataLine<DataPointType, DataIndexType>;

template <typename T_DATA_POINT>
class DataPoint;
using DataPointSpecialized = DataPoint<DataPointType>;

#define APPLICATION_NAME                ("RDB Diplomaterv Monitor V1.0.0")

#ifdef _WIN32
    #define SERIAL_PORT_DEFAULT_DEVICE_NAME ("COM3")
#else
    #define SERIAL_PORT_DEFAULT_DEVICE_NAME ("/dev/ttyACM0")
#endif

#define DATA_START_LINE                 (R"(<<<START>>>)")
#define DATA_END_LINE                   (R"(<<<END>>>)")

#endif /* GLOBAL_HPP */
