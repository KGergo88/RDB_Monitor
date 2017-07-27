/* 
 * File:   data_line.h
 * Author: gergo
 *
 * Created on July 16, 2017, 7:40 PM
 */

#include <cstdint>
#include <string>
#include <vector>

#include "data_point.hpp"

#ifndef DATALINE_H
#define DATALINE_H

template <typename T_DATA_POINT, typename T_INDEX >
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
    
    inline const std::string&   GetTitle(void) { return Title; }
    inline void                 SetTitle(const std::string& newTitle) { Title = newTitle; }    
    
    inline void AddNewDataPoint(const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        Data.push_back(newDataPoint);       
    }
    inline DataLine<T_DATA_POINT, T_INDEX>& operator<<(const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        AddNewDataPoint(newDataPoint);
    }
    inline T_INDEX GetTheNumberOfDataPoints(void) { return Data.size(); }
    
    inline const DataPoint<T_DATA_POINT>& GetDataPoint(const T_INDEX& dataPointIndex)
    {
        CheckDataPointIndex();

        return Data[dataPointIndex];
    }
    
    inline void SetDataPoint(const T_INDEX& dataPointIndex, const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        CheckDataPointIndex();
        
        Data[dataPointIndex] = newDataPoint;
    }    
    
private:
    bool CheckDataPointIndex(T_INDEX dataPointIndex)
    {
        bool result = false;

        if(Data.size() > dataPointIndex)
        {
            result = true;
        }
        else
        {
            std::string errorMessage = "The indexed DataPoint does not exist: /n Requested index: ";
            errorMessage += dataPointIndex;
            errorMessage += "/nMax index: ";
            errorMessage += Data.size();
            throw errorMessage;
        }

        return result;
    }
    
    std::string Title;
    std::vector<DataPoint<T_DATA_POINT> > Data;
};

#endif /* DATALINE_H */
