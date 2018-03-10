/* 
 * File:   diagram.hpp
 * Author: gergo
 *
 * Created on July 18, 2017, 6:29 PM
 */

#include <string>
#include <vector>

#include "global.hpp"
#include "data_line.hpp"

#ifndef DIAGRAM_HPP
#define DIAGRAM_HPP

template <typename T_DATA_POINT, typename T_INDEX >
class Diagram {
public:
    Diagram(const std::string& newTitle = "") : Title(newTitle) {};

    Diagram(const Diagram&  newDiagram) = default;

    Diagram(Diagram&& newDiagram) = default;

    ~Diagram() {}

    Diagram& operator=(const Diagram&  newDiagram) = default;
    Diagram& operator=(Diagram&& newDiagram) = default;

    inline const std::string&   GetTitle(void) const { return Title; }
    inline void                 SetTitle(const std::string& newTitle) { Title = newTitle; }
    
    inline const std::string& GetDataLineTitle(const T_INDEX& dataLineIndex)
    {
        CheckDataLineIndex(dataLineIndex);

        return Data[dataLineIndex].GetTitle();
    }

    inline void SetDataLineTitle(const T_INDEX& dataLineIndex, const std::string newDataLineTitle)
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
        Title = "";
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
    
    std::string Title;
    std::vector<DataLine<T_DATA_POINT, T_INDEX> > Data;
};

#endif /* DIAGRAM_HPP */
