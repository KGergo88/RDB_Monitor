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
#include <memory>
#include <cstdlib>

#include "global.hpp"
#include "diagram.hpp"
#include "gui.hpp"

int main(int argc, char *argv[])
{
    std::fstream myFile;
    std::string myFilePathAndName("../../MotorTestOutput.txt");
    
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
        Diagram<DataPointType, DataIndexType> myDiagram("myDiagram");
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
            myDiagram.AddNewDataLine(columnTitle);
            nameStartPos = nameEndPos + 1;
        }
        std::cout << "The number of colums: " << myDiagram.GetTheNumberOfDataLines() << std::endl;
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
                readNumbersOfTheLine.push_back(static_cast<DataPointType>(atof(readNumber.c_str())));
                numberStartPos = numberEndPos + 1;
            }
            if(myDiagram.GetTheNumberOfDataLines() != readNumbersOfTheLine.size())
            {
                throw std::string("The dataline \"" + myLine + "\" could not be processed.");
            }
            else
            {
                for(auto i = 0; i < myDiagram.GetTheNumberOfDataLines(); i++)
                {
                    myDiagram.AddNewDataPoint(i, DataPoint<DataPointType>(static_cast<DataPointType>(lineCounter), readNumbersOfTheLine.at(i)));
                }
            }
            lineCounter++;
        }
        std::cout << std::endl;

        std::cout << "2 - The graphical representation of the data:" << std::endl;
        Gui myGui(argc, argv);
        myGui.SetData(myDiagram);
        myGui.StartGui();

        std::cout << "The End." << std::endl;
    }
    catch(std::string errorString)
    {
        std::cout << "Exception catched with text: " << std::endl << errorString << std::endl;
    }
    catch(...)
    {
        std::cout << "Exception was catched with the ... argument." << std::endl;
    }
}
