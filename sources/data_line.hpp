/* 
 * File:   data_line.h
 * Author: gergo
 *
 * Created on July 16, 2017, 7:40 PM
 */

#include <string>
#include <vector>

#include "data_point.hpp"

#ifndef DATALINE_H
#define DATALINE_H

template <typename T>
class DataLine
{
public:
    DataLine() {}
    DataLine(const std::string& newTitle) : Title(newTitle) {}
    DataLine(const DataLine&  newDataLine): Title(newDataLine.Title), Data(newDataLine.Data) {}
    DataLine(const DataLine&& newDataLine): Title(newDataLine.Title), Data(newDataLine.Data) {}
    
    ~DataLine() {}
    
    DataLine& operator=(DataLine&  newDataLine) = delete;
    DataLine& operator=(DataLine&& newDataLine) = delete;
    
    inline void ChangeTitle(const std::string& newTitle) {Title = newTitle;}
    inline void AddDataPoint(const DataPoint<T>& newDataPoint)
    {
        Data.push_back(newDataPoint);
    }
    inline DataLine& operator<<(const DataPoint<T>& newDataPoint)
    {
        Data.push_back(newDataPoint);
    }
    
private:
    std::string Title;
    std::vector<DataPoint<T> > Data;
};

#endif /* DATALINE_H */
