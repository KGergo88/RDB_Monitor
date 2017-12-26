/* 
 * File:   data_point.hpp
 * Author: gergo
 *
 * Created on July 16, 2017, 7:23 PM
 */

#include "global.hpp"

#ifndef DATA_POINT_HPP
#define DATA_POINT_HPP

template <typename T_DATA_POINT>
class DataPoint
{
public:
    DataPoint(const T_DATA_POINT& newX = 0, const T_DATA_POINT& newY = 0) : x(newX), y(newY) {};
    DataPoint(const DataPoint&  newDataPoint) : x(newDataPoint.x), y(newDataPoint.y) {};
    DataPoint(DataPoint&& newDataPoint) : x(newDataPoint.x), y(newDataPoint.y) {};
    
    ~DataPoint() {}
    
    DataPoint& operator=(const DataPoint&  newDataPoint) = delete;
    DataPoint& operator=(DataPoint&& newDataPoint) = delete;
    
    inline const T_DATA_POINT& GetX(void) const {return x;}
    inline const T_DATA_POINT& GetY(void) const {return y;}
    
    inline void SetX(T_DATA_POINT& newX = 0) {x = newX; return x;}
    inline void SetY(T_DATA_POINT& newY = 0) {y = newY; return y;}
    
private:
    T_DATA_POINT x;
    T_DATA_POINT y;
};

#endif /* DATAPOINT_HPP */

