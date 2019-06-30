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



#include "global.hpp"



#ifndef DATA_POINT_HPP
#define DATA_POINT_HPP



template <typename T_DATA_POINT>
class DataPoint
{
public:
    DataPoint(const T_DATA_POINT& newX = 0, const T_DATA_POINT& newY = 0) : x(newX), y(newY) {}
    DataPoint(const DataPoint&  newDataPoint) : x(newDataPoint.x), y(newDataPoint.y) {}
    DataPoint(DataPoint&& newDataPoint) : x(newDataPoint.x), y(newDataPoint.y) {}

    ~DataPoint() = default;

    DataPoint& operator=(const DataPoint& newDataPoint) = default;
    DataPoint& operator=(DataPoint&& newDataPoint) = default;
    bool operator==(const DataPoint& other) const {return ((x == other.x) && (y == other.y));}
    bool operator!=(const DataPoint& other) const {return !(*this == other);}

    inline const T_DATA_POINT GetX(void) const {return x;}
    inline const T_DATA_POINT GetY(void) const {return y;}

    inline static const T_DATA_POINT GetXValueOf(const DataPoint& object) {return object.GetX();}
    inline static const T_DATA_POINT GetYValueOf(const DataPoint& object) {return object.GetY();}

    inline void SetX(const T_DATA_POINT newX = 0) {x = newX;}
    inline void SetY(const T_DATA_POINT newY = 0) {y = newY;}

    inline static void SetXValueOf(DataPoint& object, const T_DATA_POINT newX = 0) {object.SetX(newX);}
    inline static void SetYValueOf(DataPoint& object, const T_DATA_POINT newY = 0) {object.SetY(newY);}

    inline static bool CompareXValues(const DataPoint& object_A, const DataPoint& object_B) {return (GetXValueOf(object_A) < GetXValueOf(object_B));}
    inline static bool CompareYValues(const DataPoint& object_A, const DataPoint& object_B) {return (GetYValueOf(object_A) < GetYValueOf(object_B));}

private:
    T_DATA_POINT x;
    T_DATA_POINT y;
};



#endif /* DATAPOINT_HPP */

