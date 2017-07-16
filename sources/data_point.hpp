/* 
 * File:   data_point.hpp
 * Author: gergo
 *
 * Created on July 16, 2017, 7:23 PM
 */

#ifndef DATA_POINT_HPP
#define DATA_POINT_HPP

class DataPoint
{
public:
    DataPoint(long long int newX = 0, long long int newY = 0) : x(newX), y(newY) {};
    DataPoint(DataPoint&  newDataPoint) = delete;
    DataPoint(DataPoint&& newDataPoint) = delete;
    
    ~DataPoint();
    
    DataPoint& operator=(DataPoint&  newDataPoint) = delete;
    DataPoint& operator=(DataPoint&& newDataPoint) = delete;
    
    inline long long int GetX(void) {return x;}
    inline long long int GetY(void) {return y;}
    
    inline long long int SetX(long long int newX = 0) {x = newX; return x;}
    inline long long int SetY(long long int newY = 0) {y = newY; return y;}
    
private:
    long long int x;
    long long int y;
};

#endif /* DATAPOINT_HPP */

