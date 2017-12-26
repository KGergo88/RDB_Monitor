/* 
 * File:   main.cpp
 * Author: gergo
 * 
 * Created on July 15, 2017, 7:23 PM
 */

#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <thread>

#include "global.hpp"
#include "diagram.hpp"
#include "gui.hpp"
#include "serial_port.hpp"
#include "data_processor.hpp"


/*
void ProcessInputFile(const std::string& path, DiagramObject& myDiagram)
{
    std::fstream myFile;
    std::string myFilePathAndName(path);

    try
    {
//        std::cout << "1 - Accessing the measured data from a file:" << std::endl;
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
            //std::cout << myLine << std::endl;
        }
        std::cout << std::endl;

//        std::cout << "2 - The analization of the columns:" << std::endl;
        myFile.clear(); myFile.seekg(0);
        std::getline(myFile,myLine);
        myLine.erase(std::remove(myLine.begin(), myLine.end(), ' '), myLine.end());
//        std::cout << "The first line without spaces:" << std::endl << myLine << std::endl;
//        std::cout << "The received column titles:" << std::endl;
        std::size_t nameStartPos = 0;
        std::size_t nameEndPos = 0;
        while(std::string::npos != (nameEndPos = myLine.find(',', nameStartPos)))
        {
            std::string columnTitle;

            columnTitle = myLine.substr(nameStartPos, nameEndPos - nameStartPos);
//            std::cout << columnTitle << std::endl;
            myDiagram.AddNewDataLine(columnTitle);
            nameStartPos = nameEndPos + 1;
        }
//        std::cout << "The number of colums: " << myDiagram.GetTheNumberOfDataLines() << std::endl;
        std::cout << std::endl;

//        std::cout << "3 - The analization of the data:" << std::endl;
        auto lineCounter = 0;
        while(std::getline(myFile,myLine) && !myFile.eof())
        {
            myLine.erase(std::remove(myLine.begin(), myLine.end(), ' '), myLine.end());
//            std::cout << "The next line (" << lineCounter << ") without spaces:" << std::endl << myLine << std::endl;
//            std::cout << "The received numbers:" << std::endl;
            std::size_t numberStartPos = 0;
            std::size_t numberEndPos = 0;
            std::vector<DataPointType> readNumbersOfTheLine;
            while(std::string::npos != (numberEndPos = myLine.find(',', numberStartPos)))
            {
                std::string readNumber = myLine.substr(numberStartPos, numberEndPos - numberStartPos);
//                std::cout << readNumber << std::endl;
                readNumbersOfTheLine.push_back(static_cast<DataPointType>(atof(readNumber.c_str())));
                numberStartPos = numberEndPos + 1;
            }
            if(myDiagram.GetTheNumberOfDataLines() != readNumbersOfTheLine.size())
            {
                throw std::string("The dataline \"" + myLine + "\" could not be processed.");
            }
            else
            {
                for(DataIndexType i = 0; i < myDiagram.GetTheNumberOfDataLines(); i++)
                {
                    myDiagram.AddNewDataPoint(i, DataPointObject(static_cast<DataPointType>(lineCounter), readNumbersOfTheLine.at(i)));
                }
            }
            lineCounter++;
        }
        std::cout << std::endl;
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
*/

bool WorkerThread(void)
{
    while(1)
    {
        if(SerialPort::GetInstance().IsOpen())
        {
            auto received_data = SerialPort::GetInstance().ReceiveMeasurementData();
            if(received_data)
            {
                auto assembled_diagram = DataProcessor::GetInstance().ProcessData("SerialPort", *received_data);
                if(assembled_diagram)
                {
                    Gui::GetInstance().AddToDiagramList(*assembled_diagram);
                }
                else
                {
                    std::cerr << "Empty assembled_diagram..." << std::endl;
                }
            }
        }
    }
}

int main(void)
{
    std::cout << "Hello RDB!" << std::endl << std::endl;

    std::thread worker_thread(WorkerThread);

    Gui::GetInstance().Run();
    std::cout << "The GUI has stopped." << std::endl;

    worker_thread.join();
    std::cout << "The working thread has stopped." << std::endl;

    std::cout << "The End." << std::endl;
}
