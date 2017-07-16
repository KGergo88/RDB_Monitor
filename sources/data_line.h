/* 
 * File:   data_line.h
 * Author: gergo
 *
 * Created on July 16, 2017, 7:40 PM
 */

#ifndef DATALINE_H
#define DATALINE_H

class DataLine
{
public:
    DataLine(){};
    DataLine(DataLine&  newDataLine) = delete;
    DataLine(DataLine&& newDataLine) = delete;
    
    ~DataLine();
    
    DataLine& operator=(DataLine&  newDataLine) = delete;
    DataLine& operator=(DataLine&& newDataLine) = delete;
    
private:

};

#endif /* DATALINE_H */
