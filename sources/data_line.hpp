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



#include <string>
#include <vector>
#include <functional>
#include <algorithm>

#include "global.hpp"
#include "data_point.hpp"



#ifndef DATALINE_HPP
#define DATALINE_HPP



template <typename T_DATA_POINT, typename T_INDEX >
class DataLine
{
public:
    explicit DataLine(const std::string& newDataLineTitle = "") : DataLineTitle(newDataLineTitle) {}

    DataLine(const DataLine&  newDataLine) = default;
    DataLine(DataLine&& newDataLine) = default;
    
    ~DataLine() {}
    
    DataLine& operator=(const DataLine&  newDataLine) = delete;
    DataLine& operator=(DataLine&& newDataLine) = delete;
    
    inline const std::string& GetTitle(void) const
    {
        return DataLineTitle;
    }

    inline void SetTitle(const std::string& newDataLineTitle)
    {
        DataLineTitle = newDataLineTitle;
    }
    
    inline void AddNewDataPoint(const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        Data.push_back(newDataPoint);       
    }

    inline DataLine<T_DATA_POINT, T_INDEX>& operator<<(const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        AddNewDataPoint(newDataPoint);
    }

    inline T_INDEX GetTheNumberOfDataPoints(void) const
    {
        return Data.size();
    }
    
    inline const DataPoint<T_DATA_POINT>& GetDataPoint(const T_INDEX& dataPointIndex) const
    {
        CheckDataPointIndex(dataPointIndex);

        return Data[dataPointIndex];
    }  
    
    inline void SetDataPoint(const T_INDEX& dataPointIndex, const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        CheckDataPointIndex(dataPointIndex);

        Data[dataPointIndex] = newDataPoint;
    }

    inline const DataPoint<T_DATA_POINT>& GetDataPointWithMinValue(const std::function<bool(DataPoint<T_DATA_POINT>, DataPoint<T_DATA_POINT>)>& compare) const
    {
        auto min_value = std::min_element(Data.begin(), Data.end(), compare);

        if(Data.end() == min_value)
        {
            std::string errorMessage = "The DataLine is empty!";
            throw errorMessage;
        }

        return *min_value;
    }

    inline const DataPoint<T_DATA_POINT>& GetDataPointWithMaxValue(const std::function<bool(DataPoint<T_DATA_POINT>, DataPoint<T_DATA_POINT>)>& compare) const
    {
        auto max_value = std::max_element(Data.begin(), Data.end(), compare);

        if(Data.end() == max_value)
        {
            std::string errorMessage = "The DataLine is empty!";
            throw errorMessage;
        }

        return *max_value;
    }
    
private:
    void CheckDataPointIndex(const T_INDEX& dataPointIndex) const
    {
        if(Data.size() <= dataPointIndex)
        {
            std::string errorMessage = "The indexed DataPoint does not exist: /n Requested index: ";
            errorMessage += std::to_string(dataPointIndex);
            errorMessage += "/nMax index: ";
            errorMessage += std::to_string(Data.size());
            throw errorMessage;
        }
    }
    
    std::string DataLineTitle;
    std::vector<DataPoint<T_DATA_POINT> > Data;
};



#endif /* DATALINE_HPP */
