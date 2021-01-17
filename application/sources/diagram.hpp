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


#ifndef DIAGRAM_HPP
#define DIAGRAM_HPP


#include <string>
#include <algorithm>

#include "ordered_dict.h"
#include "data_line.hpp"


template <typename T_DATA_POINT = DataPointDefaultCoordinate_t, typename T_INDEX = DataLineDefaultIndex_t >
class Diagram {
public:
    using coordinate_t = T_DATA_POINT;
    using index_t = T_INDEX;
    using DataPoint_t = DataPoint<coordinate_t>;
    using DataLine_t = DataLine<coordinate_t, index_t>;
    using DataContainer_t = OrderedDict<DataLine_t>;

    Diagram(const std::string& newDiagramTitle = "", const std::string& newAxisXTitle = "") : DiagramTitle(newDiagramTitle), AxisXTitle(newAxisXTitle) {}

    Diagram(const Diagram& newDiagram) = default;
    Diagram(Diagram&& newDiagram) = default;

    ~Diagram() = default;

    Diagram& operator=(const Diagram& newDiagram) = default;
    Diagram& operator=(Diagram&& newDiagram) = default;

    inline const std::string& GetTitle(void) const
    {
        return DiagramTitle;
    }

    inline void SetTitle(const std::string& newDiagramTitle)
    {
        DiagramTitle = newDiagramTitle;
    }

    inline const std::string& GetAxisXTitle(void) const
    {
        return AxisXTitle;
    }

    inline void SetAxisXTitle(const std::string& newAxisXTitle)
    {
        AxisXTitle = newAxisXTitle;
    }

    bool AddNewDataLine(const std::string& newDataLineId,
                        const std::string& newDataLineTitle = "")
    {
        return Data.emplace_back(newDataLineId, DataLine_t(newDataLineTitle));
    }

    inline bool HasDataLine(const index_t& dataLineIndex) const
    {
        return (!Data.empty() && (dataLineIndex <= GetTheNumberOfDataLines()));
    }

    inline bool HasDataLine(const std::string& dataLineId) const
    {
        return Data.hasKey(dataLineId);
    }

    inline const index_t GetTheNumberOfDataLines(void) const
    {
        return Data.size();
    }

    inline const std::string GetDataLineTitle(const index_t& dataLineIndex) const
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetTitle();
    }

    inline void SetDataLineTitle(const index_t& dataLineIndex, const std::string& newDataLineTitle)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].SetTitle(newDataLineTitle);
    }

    void AddNewDataPoint(const index_t& dataLineIndex, const DataPoint_t& newDataPoint)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].AddNewDataPoint(newDataPoint);
    }

    void AddNewDataPoint(const std::string& dataLineId, const DataPoint_t& newDataPoint)
    {
        Data[dataLineId].AddNewDataPoint(newDataPoint);
    }

    inline const index_t GetTheNumberOfDataPoints(const index_t& dataLineIndex) const
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetTheNumberOfDataPoints();
    }

    inline const DataPoint_t GetDataPoint(const index_t& dataLineIndex, const index_t& dataPointIndex) const
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetDataPoint(dataPointIndex);
    }
    
    void SetDataPoint(const index_t& dataLineIndex, const index_t& dataPointIndex, const DataPoint_t& newDataPoint)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].SetDataPoint(dataPointIndex, newDataPoint);
    }    

    std::pair<DataPoint_t, DataPoint_t> GetExtremeValues(const index_t& dataLineIndex) const
    {
        CheckDataLineIndex(dataLineIndex);

        auto min_x_value = Data[dataLineIndex].GetDataPointWithMinValue(DataPoint_t::CompareXValues).GetX();
        auto max_x_value = Data[dataLineIndex].GetDataPointWithMaxValue(DataPoint_t::CompareXValues).GetX();
        auto min_y_value = Data[dataLineIndex].GetDataPointWithMinValue(DataPoint_t::CompareYValues).GetY();
        auto max_y_value = Data[dataLineIndex].GetDataPointWithMaxValue(DataPoint_t::CompareYValues).GetY();

        std::pair<DataPoint_t, DataPoint_t> extreme_values(DataPoint_t(min_x_value, min_y_value), DataPoint_t(max_x_value, max_y_value));

        return extreme_values;
    }

    std::pair<DataPoint_t, DataPoint_t> GetExtremeValues(void) const
    {
        if(!Data.empty())
        {
            DataLine_t data_points_with_min_x_values;
            DataLine_t data_points_with_max_x_values;
            DataLine_t data_points_with_min_y_values;
            DataLine_t data_points_with_max_y_values;

            for(const auto& i : Data)
            {
                data_points_with_min_x_values.AddNewDataPoint(i.second.GetDataPointWithMinValue(DataPoint_t::CompareXValues));
                data_points_with_max_x_values.AddNewDataPoint(i.second.GetDataPointWithMaxValue(DataPoint_t::CompareXValues));
                data_points_with_min_y_values.AddNewDataPoint(i.second.GetDataPointWithMinValue(DataPoint_t::CompareYValues));
                data_points_with_max_y_values.AddNewDataPoint(i.second.GetDataPointWithMaxValue(DataPoint_t::CompareYValues));
            }

            auto min_x_value = data_points_with_min_x_values.GetDataPointWithMinValue(DataPoint_t::CompareXValues).GetX();
            auto max_x_value = data_points_with_max_x_values.GetDataPointWithMaxValue(DataPoint_t::CompareXValues).GetX();
            auto min_y_value = data_points_with_min_y_values.GetDataPointWithMinValue(DataPoint_t::CompareYValues).GetY();
            auto max_y_value = data_points_with_max_y_values.GetDataPointWithMaxValue(DataPoint_t::CompareYValues).GetY();

            std::pair<DataPoint_t, DataPoint_t> extreme_values(DataPoint_t(min_x_value, min_y_value), DataPoint_t(max_x_value, max_y_value));

            return extreme_values;
        }
        else
        {
            std::string errorMessage = "The Diagram is empty!";
            throw errorMessage;
        }
    }

    void EraseContent(void)
    {
        DiagramTitle = "";
        AxisXTitle = "";
        Data.clear();
    }

private:
    void CheckDataLineIndex(const index_t& dataLineIndex) const
    {
        if(Data.size() <= dataLineIndex)
        {
            std::string errorMessage = "The indexed DataLine does not exist: /n Requested index: ";
            errorMessage += std::to_string(dataLineIndex);
            errorMessage += "/nMax index: ";
            errorMessage += std::to_string(Data.size());
            throw errorMessage;
        }
    }

    std::string DiagramTitle;
    std::string AxisXTitle;
    DataContainer_t Data;
};

using DefaultDiagram = Diagram<DataPointDefaultCoordinate_t, DataLineDefaultIndex_t>;


#endif /* DIAGRAM_HPP */
