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

#include "global.hpp"
#include "data_line.hpp"



#ifndef DIAGRAM_HPP
#define DIAGRAM_HPP



template <typename T_DATA_POINT, typename T_INDEX >
class Diagram {
public:
    Diagram(const std::string& newDiagramTitle = "", const std::string& newAxisXTitle = "") : DiagramTitle(newDiagramTitle), AxisXTitle(newAxisXTitle) {}

    Diagram(const Diagram&  newDiagram) = default;

    Diagram(Diagram&& newDiagram) = default;

    ~Diagram() {}

    Diagram& operator=(const Diagram&  newDiagram) = default;
    Diagram& operator=(Diagram&& newDiagram) = default;

    inline const std::string&   GetTitle(void) const { return DiagramTitle; }
    inline void                 SetTitle(const std::string& newDiagramTitle) { DiagramTitle = newDiagramTitle; }
    inline const std::string&   GetAxisXTitle(void) const { return AxisXTitle; }
    inline void                 SetAxisXTitle(const std::string& newAxisXTitle) { AxisXTitle = newAxisXTitle; }
    
    inline const std::string& GetDataLineTitle(const T_INDEX& dataLineIndex)
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetTitle();
    }

    inline void SetDataLineTitle(const T_INDEX& dataLineIndex, const std::string& newDataLineTitle)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].SetTitle(newDataLineTitle);
    }
    
    inline const T_INDEX GetTheNumberOfDataLines(void) { return Data.size(); }
    
    void AddNewDataLine(const std::string& newDataLineTitle = "")
    {
        Data.push_back(DataLine<T_DATA_POINT, T_INDEX>(newDataLineTitle));
    }
    
    void AddNewDataPoint(T_INDEX dataLineIndex, const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].AddNewDataPoint(newDataPoint);
    }

    inline const T_INDEX GetTheNumberOfDataPoints(const T_INDEX& dataLineIndex)
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetTheNumberOfDataPoints();
    }

    inline const DataPoint<T_DATA_POINT>& GetDataPoint(const T_INDEX& dataLineIndex, const T_INDEX& dataPointIndex)
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetDataPoint(dataPointIndex);
    }
    
    void SetDataPoint(const T_INDEX& dataLineIndex, const T_INDEX& dataPointIndex, const DataPoint<T_DATA_POINT>& newDataPoint)
    {
        CheckDataLineIndex(dataLineIndex);

        Data[dataLineIndex].SetDataPoint(dataPointIndex, newDataPoint);
    }    

    void EraseContent(void)
    {
        DiagramTitle = "";
        AxisXTitle = "";
        Data.clear();
    }

private:
    bool CheckDataLineIndex(const T_INDEX& dataLineIndex)
    {
        bool result = false;

        if(Data.size() > dataLineIndex)
        {
            result = true;
        }
        else
        {            
            std::string errorMessage = "The indexed DataLine does not exist: /n Requested index: ";
            errorMessage += std::to_string(dataLineIndex);
            errorMessage += "/nMax index: ";
            errorMessage += std::to_string(Data.size());
            throw errorMessage;
        }    

        return result;
    }
    
    std::string DiagramTitle;
    std::string AxisXTitle;
    std::vector<DataLine<T_DATA_POINT, T_INDEX> > Data;
};



#endif /* DIAGRAM_HPP */
