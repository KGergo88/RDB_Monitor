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
    
    DataPoint& operator=(DataPoint&  newDataPoint) = delete;
    DataPoint& operator=(DataPoint&& newDataPoint) = delete;
    
    inline const T& GetX(void) const {return x;}
    inline const T& GetY(void) const {return y;}
    
    inline void SetX(T& newX = 0) {x = newX; return x;}
    inline void SetY(T& newY = 0) {y = newY; return y;}
    
private:
    T x;
    T y;
};

#endif /* DATAPOINT_HPP */

