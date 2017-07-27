/* 
 * File:   data_point.hpp
 * Author: gergo
 *
 * Created on July 16, 2017, 7:23 PM
 */

#ifndef DATA_POINT_HPP
#define DATA_POINT_HPP

template <typename T>
class DataPoint
{
public:
    DataPoint(const T& newX = 0, const T& newY = 0) : x(newX), y(newY) {};
    DataPoint(const DataPoint&  newDataPoint) : x(newDataPoint.x), y(newDataPoint.y) {};
    DataPoint(const DataPoint&& newDataPoint) : x(newDataPoint.x), y(newDataPoint.y) {};
    
    ~DataPoint() {}
    
    DataPoint& operator=(const DataPoint&  newDataPoint) = delete;
    DataPoint& operator=(const DataPoint&& newDataPoint) = delete;
    
    inline T GetX(void) {return x;}
    inline T GetY(void) {return y;}
    
    inline T SetX(T& newX = 0) {x = newX; return x;}
    inline T SetY(T& newY = 0) {y = newY; return y;}
    
private:
    T x;
    T y;
};

#endif /* DATAPOINT_HPP */

