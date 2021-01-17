//==============================================================================//
//                                                                              //
//    RDB Monitor                                                               //
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


#ifndef DATA_LINE_HPP
#define DATA_LINE_HPP


#include <string>
#include <vector>
#include <functional>
#include <algorithm>

#include "data_point.hpp"


template <typename T_DATA_POINT, typename T_INDEX >
class DataLine
{
public:
    using coordinate_t = T_DATA_POINT;
    using index_t = T_INDEX;

    explicit DataLine(const std::string& newDataLineTitle = "") : DataLineTitle(newDataLineTitle) {}

    DataLine(const DataLine& newDataLine) = default;
    DataLine(DataLine&& newDataLine) = default;
    
    ~DataLine() = default;
    
    DataLine& operator=(const DataLine& newDataLine) = default;
    DataLine& operator=(DataLine&& newDataLine) = default;
    
    inline std::string GetTitle(void) const
    {
        return DataLineTitle;
    }

    inline void SetTitle(const std::string& newDataLineTitle)
    {
        DataLineTitle = newDataLineTitle;
    }
    
    inline void AddNewDataPoint(const DataPoint<coordinate_t>& newDataPoint)
    {
        Data.push_back(newDataPoint);
    }

    inline DataLine<coordinate_t, index_t>& operator<<(const DataPoint<coordinate_t>& newDataPoint)
    {
        AddNewDataPoint(newDataPoint);
        return *this;
    }

    inline index_t GetTheNumberOfDataPoints(void) const
    {
        return Data.size();
    }
    
    inline const DataPoint<coordinate_t> GetDataPoint(const index_t& dataPointIndex) const
    {
        CheckDataPointIndex(dataPointIndex);

        return Data[dataPointIndex];
    }  
    
    inline void SetDataPoint(const index_t& dataPointIndex, const DataPoint<coordinate_t>& newDataPoint)
    {
        CheckDataPointIndex(dataPointIndex);

        Data[dataPointIndex] = newDataPoint;
    }

    inline const DataPoint<coordinate_t> GetDataPointWithMinValue(const std::function<bool(DataPoint<coordinate_t>,
                                                                                           DataPoint<coordinate_t>)>& compare) const
    {
        auto min_value = std::min_element(Data.begin(), Data.end(), compare);

        if(Data.end() == min_value)
        {
            std::string errorMessage = "The DataLine is empty!";
            throw errorMessage;
        }

        return *min_value;
    }

    inline const DataPoint<coordinate_t> GetDataPointWithMaxValue(const std::function<bool(DataPoint<coordinate_t>,
                                                                                           DataPoint<coordinate_t>)>& compare) const
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
    void CheckDataPointIndex(const index_t& dataPointIndex) const
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
    std::vector<DataPoint<coordinate_t> > Data;
};


#endif /* DATA_LINE_HPP */
