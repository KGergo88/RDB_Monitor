/* 
 * File:   diagram.cpp
 * Author: gergo
 * 
 * Created on July 18, 2017, 6:29 PM
 */

#include "diagram.hpp"

const std::string& Diagram::GetDataLineTitle(const uint64_t& dataLineIndex)
{
    
}
//    void                SetDataLineTitle(const uint64_t& dataLineIndex, const std::string newDataLineTitle);
//    
//    inline uint64_t GetTheNumberOfDataLines(void) { return Data.size(); }
//    
//    const DataPoint&    GetDataPoint(const uint64_t& dataLineIndex, const uint64_t& dataPointIndex);
//    void                SetDataPoint(const uint64_t& dataLineIndex, const uint64_t& dataPointIndex, const DataPoint& newDataPoint);  

void Diagram::AddNewDataPoint(uint64_t dataLineIndex, const DataPoint& newDataPoint)
{
    if(Data.size() > dataLineIndex)
    {
        Data[dataLineIndex].AddNewDataPoint(newDataPoint);
    }
    else
    {
        throw std::string("The indexed DataLine does not exist: " + std::endl +
                          "Requested index: " + dataLineIndex + std::endl + 
                          "Max index: " + Data.size());
    }
}

DataPoint& Diagram::GetDataPoint(const uint64_t& dataLineIndex, const uint64_t& dataPointIndex)
{
    if(Data.size() > dataLineIndex)
    {
        if(Data[dataLineIndex].size() > dataPointIndex)
        {
            return ((Data[dataLineIndex])[dataPointIndex]);
        }
        else
        {
            throw std::string("The indexed DataPoint does not exist: " + std::endl +
                              "Requested index: " + dataPointIndex + std::endl + 
                              "Max index: " + Data[dataLineIndex].size());
        }
    }
    else
    {
        throw std::string("The indexed DataLine does not exist: " + std::endl +
                          "Requested index: " + dataLineIndex + std::endl + 
                          "Max index: " + Data.size());
    }    
}

bool Diagram::CheckDataLineIndex(T_INDEX dataLineIndex)
{
    bool result = false;

    if(Data.size() > dataLineIndex)
    {
        result = true;
    }
    return result;
}
