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
#include <algorithm>

#include "global.hpp"
#include "data_point.hpp"
#include "data_line.hpp"



#ifndef DIAGRAM_HPP
#define DIAGRAM_HPP



template <typename T_DATA_POINT, typename T_INDEX >
class Diagram {
public:
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

    void AddNewDataLine(const std::string& newDataLineId = DataLine<T_DATA_POINT, T_INDEX>::invalid_id,
                        const std::string& newDataLineTitle = "")
    {
        Data.push_back(DataLine<T_DATA_POINT, T_INDEX>(newDataLineId, newDataLineTitle));
    }

    inline const T_INDEX GetTheNumberOfDataLines(void) const
    {
        return Data.size();
    }

    inline const std::string GetDataLineTitle(const T_INDEX& dataLineIndex) const
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetTitle();
    }

    inline void SetDataLineTitle(const T_INDEX& dataLineIndex, const std::string& newDataLineTitle)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].SetTitle(newDataLineTitle);
    }

    inline std::string GetDataLineId(const T_INDEX& dataLineIndex) const
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetId();
    }

    inline void SetDataLineId(const T_INDEX& dataLineIndex, const std::string& newDataLineId)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].SetId(newDataLineId);
    }

    inline bool GetDataLineIndex(const std::string& dataLineId, T_INDEX& dataLineIndex) const
    {
        bool result = false;
        dataLineIndex = 0;

        for(auto const& dataLine : Data)
        {
            if(dataLine.GetId() == dataLineId)
            {
                result = true;
                break;
            }
            dataLineIndex++;
        }

        return result;
    }

    void AddNewDataPoint(T_INDEX dataLineIndex, const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].AddNewDataPoint(newDataPoint);
    }

    void AddNewDataPoint(std::string dataLineId, const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        T_INDEX data_line_index = 0;

        if(GetDataLineIndex(dataLineId, data_line_index))
        {
            Data[data_line_index].AddNewDataPoint(newDataPoint);
        }
    }

    inline const T_INDEX GetTheNumberOfDataPoints(const T_INDEX& dataLineIndex) const
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetTheNumberOfDataPoints();
    }

    inline const DataPoint<T_DATA_POINT> GetDataPoint(const T_INDEX& dataLineIndex, const T_INDEX& dataPointIndex) const
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetDataPoint(dataPointIndex);
    }
    
    void SetDataPoint(const T_INDEX& dataLineIndex, const T_INDEX& dataPointIndex, const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].SetDataPoint(dataPointIndex, newDataPoint);
    }    

    std::pair<DataPoint<T_DATA_POINT>, DataPoint<T_DATA_POINT> > GetExtremeValues(const T_INDEX& dataLineIndex) const
    {
        CheckDataLineIndex(dataLineIndex);

        auto min_x_value = Data[dataLineIndex].GetDataPointWithMinValue(DataPoint<T_DATA_POINT>::CompareXValues).GetX();
        auto max_x_value = Data[dataLineIndex].GetDataPointWithMaxValue(DataPoint<T_DATA_POINT>::CompareXValues).GetX();
        auto min_y_value = Data[dataLineIndex].GetDataPointWithMinValue(DataPoint<T_DATA_POINT>::CompareYValues).GetY();
        auto max_y_value = Data[dataLineIndex].GetDataPointWithMaxValue(DataPoint<T_DATA_POINT>::CompareYValues).GetY();

        std::pair<DataPoint<T_DATA_POINT>, DataPoint<T_DATA_POINT> > extreme_values(DataPoint<T_DATA_POINT>(min_x_value, min_y_value),
                                                                                    DataPoint<T_DATA_POINT>(max_x_value, max_y_value));

        return extreme_values;
    }

    std::pair<DataPoint<T_DATA_POINT>, DataPoint<T_DATA_POINT> > GetExtremeValues(void) const
    {
        if(!Data.empty())
        {
            DataLine<T_DATA_POINT, T_INDEX> data_points_with_min_x_values;
            DataLine<T_DATA_POINT, T_INDEX> data_points_with_max_x_values;
            DataLine<T_DATA_POINT, T_INDEX> data_points_with_min_y_values;
            DataLine<T_DATA_POINT, T_INDEX> data_points_with_max_y_values;

            for(const auto& i : Data)
            {
                data_points_with_min_x_values.AddNewDataPoint(i.GetDataPointWithMinValue(DataPoint<T_DATA_POINT>::CompareXValues));
                data_points_with_max_x_values.AddNewDataPoint(i.GetDataPointWithMaxValue(DataPoint<T_DATA_POINT>::CompareXValues));
                data_points_with_min_y_values.AddNewDataPoint(i.GetDataPointWithMinValue(DataPoint<T_DATA_POINT>::CompareYValues));
                data_points_with_max_y_values.AddNewDataPoint(i.GetDataPointWithMaxValue(DataPoint<T_DATA_POINT>::CompareYValues));
            }

            auto min_x_value = data_points_with_min_x_values.GetDataPointWithMinValue(DataPoint<T_DATA_POINT>::CompareXValues).GetX();
            auto max_x_value = data_points_with_max_x_values.GetDataPointWithMaxValue(DataPoint<T_DATA_POINT>::CompareXValues).GetX();
            auto min_y_value = data_points_with_min_y_values.GetDataPointWithMinValue(DataPoint<T_DATA_POINT>::CompareYValues).GetY();
            auto max_y_value = data_points_with_max_y_values.GetDataPointWithMaxValue(DataPoint<T_DATA_POINT>::CompareYValues).GetY();

            std::pair<DataPoint<T_DATA_POINT>, DataPoint<T_DATA_POINT> > extreme_values(DataPoint<T_DATA_POINT>(min_x_value, min_y_value),
                                                                                        DataPoint<T_DATA_POINT>(max_x_value, max_y_value));

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
    void CheckDataLineIndex(const T_INDEX& dataLineIndex) const
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
    std::vector<DataLine<T_DATA_POINT, T_INDEX> > Data;
};



#endif /* DIAGRAM_HPP */
