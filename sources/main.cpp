/* 
 * File:   main.cpp
 * Author: gergo
 * 
 * Created on July 15, 2017, 7:23 PM
 */

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>

#include "data_line.hpp"

using DataPointType = double;

int main(void)
{
    std::fstream myFile;
    std::string myFilePathAndName("../ControllerTestOuput.txt");    
    
    std::cout << "Hello RDB!" << std::endl;
    std::cout << std::endl;
  
    try
    {
        std::cout << "1 - Accessing the measured data from a file:" << std::endl;
        myFile.open(myFilePathAndName);
        if(myFile.is_open())
        {
            std::cout << "The file \"" << myFilePathAndName << "\" was successfully opened..." << std::endl;            
        }
        else
        {
            throw std::string("The file could not be opened. Maybe wrong path or name?");
        }
        
        std::cout << "The content of the file:" << std::endl;
        myFile.clear(); myFile.seekg(0);
        std::string myLine;
        while(std::getline(myFile,myLine))
        {
            std::cout << myLine << std::endl;            
        }        
        std::cout << std::endl;
        
        std::cout << "2 - The analization of the columns:" << std::endl;
        myFile.clear(); myFile.seekg(0);      
        std::vector<DataLine<DataPointType> > myDataLines;
        std::getline(myFile,myLine);
        myLine.erase(std::remove(myLine.begin(), myLine.end(), ' '), myLine.end());
        std::cout << "The first line without spaces:" << std::endl << myLine << std::endl;
        std::cout << "The received column titles:" << std::endl;       
        auto nameStartPos = 0;
        auto nameEndPos = 0;
        while(std::string::npos != (nameEndPos = myLine.find(',', nameStartPos)))
        {
            std::string columnTitle;
                       
            columnTitle = myLine.substr(nameStartPos, nameEndPos - nameStartPos);
            std::cout << columnTitle << std::endl;
            myDataLines.push_back(columnTitle);
            nameStartPos = nameEndPos + 1;
        }
        std::cout << "The number of colums: " << myDataLines.size() << std::endl;
        std::cout << std::endl;
        
        std::cout << "3 - The analization of the data:" << std::endl;
        auto lineCounter = 0;
        while(std::getline(myFile,myLine) && !myFile.eof())
        {
            myLine.erase(std::remove(myLine.begin(), myLine.end(), ' '), myLine.end());
            std::cout << "The next line (" << lineCounter << ") without spaces:" << std::endl << myLine << std::endl;
            std::cout << "The received numbers:" << std::endl;  
            auto numberStartPos = 0;
            auto numberEndPos = 0;
            std::vector<DataPointType> readNumbersOfTheLine;
            while(std::string::npos != (numberEndPos = myLine.find(',', numberStartPos)))
            {
                std::string readNumber = myLine.substr(numberStartPos, numberEndPos - numberStartPos);
                std::cout << readNumber << std::endl;
                readNumbersOfTheLine.push_back(atof(readNumber.c_str()));
                numberStartPos = numberEndPos + 1;
            }
            if(myDataLines.size() != readNumbersOfTheLine.size())
            {
                throw std::string("The dataline \"" + myLine + "\" could not be processed.");
            }
            else
            {
                for(auto i = 0; i < myDataLines.size(); i++)
                {
                    myDataLines[i].AddNewDataPoint(DataPoint<DataPointType>(static_cast<DataPointType>(lineCounter),
                                                   readNumbersOfTheLine.at(i)));
                }
            }
            lineCounter++;
        }
        std::cout << std::endl;
        
        std::cout << "The End." << std::endl;        
    }
    catch(std::string errorString)
    {
        std::cerr << "Exception catched with text: " << std::endl << errorString << std::endl;
    }
    catch(...)
    {
        std::cout << "Exception was catched with the ... argument." << std::endl;
    }
}
