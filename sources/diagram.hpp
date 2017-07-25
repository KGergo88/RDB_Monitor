/* 
 * File:   diagram.hpp
 * Author: gergo
 *
 * Created on July 18, 2017, 6:29 PM
 */

#include <cstdint>
#include <string>
#include <vector>

#include "data_line.hpp"

#ifndef DIAGRAM_HPP
#define DIAGRAM_HPP

template <typename T_DATA_POINT, T_INDEX >
class Diagram {
public:
    Diagram(const std::string& newTitle = "") : Title(newTitle) {};
    Diagram(const Diagram&  newDiagram) = delete;
    Diagram(const Diagram&& newDiagram) = delete;
    
    ~Diagram() {}
    
    Diagram& operator=(const Diagram&  newDiagram) = delete;
    Diagram& operator=(const Diagram&& newDiagram) = delete;

    inline const std::string&   GetTitle(void) { return Title; }
    inline void                 SetTitle(const std::string& newTitle) { Title = newTitle; }
    
    const std::string&  GetDataLineTitle(const T_INDEX& dataLineIndex);
    void                SetDataLineTitle(const T_INDEX& dataLineIndex, const std::string newDataLineTitle);
    
    inline T_INDEX GetTheNumberOfDataLines(void) { return Data.size(); }
    
    void                AddNewDataPoint(const T_INDEX& dataLineIndex, const DataPoint& newDataPoint);
    const DataPoint&    GetDataPoint(const T_INDEX& dataLineIndex, const uint64_t& dataPointIndex);
    void                SetDataPoint(const T_INDEX& dataLineIndex, const uint64_t& dataPointIndex, const DataPoint& newDataPoint);    

private:
    bool CheckDataLineIndex(T_INDEX dataLineIndex)
    
    std::string Title;
    std::vector<DataLine<T_DATA_POINT> > Data;
};

#endif /* DIAGRAM_HPP */
