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

int main(void)
{
    std::fstream myFile;
    std::string myFilePathAndName("/home/gergo/sources/rdb_diplomaterv_monitor/ControllerTestOuput.txt");    
    
    std::cout << "Hello RDB!" << std::endl;
    std::cout << std::endl;
  
    try
    {
        myFile.open(myFilePathAndName);
        std::cout << "The file \"" << myFilePathAndName << "\" was successfully opened..." << std::endl;
        
        std::cout << "The content of the file:" << std::endl;
        myFile.clear(); myFile.seekg(0);
        std::string myLine;
        while(std::getline(myFile,myLine))
        {
            std::cout << myLine << std::endl;            
        }        
        std::cout << std::endl;
        
        std::cout << "The analization of the data:" << std::endl;
        myFile.clear(); myFile.seekg(0);      
        std::getline(myFile,myLine);
        myLine.erase(std::remove(myLine.begin(), myLine.end(), ' '), myLine.end());
        std::cout << "The first line without spaces:" << std::endl;
        std::cout << myLine << std::endl;
        std::cout << "The received column titles:" << std::endl;
        std::vector myColumns;
        auto nameStartPos = 0;
        auto nameEndPos = myLine.find(',', nameStartPos);        
        while(std::string::npos != nameEndPos)
        {            
            nameEndPos = myLine.find(',', nameStartPos);        
            std::cout << myLine.substr(nameStartPos, nameEndPos - nameStartPos) << std::endl;
            nameStartPos = nameEndPos + 1;         
        }
        std::cout << "The number of colums: " << numberOfColumns << std::endl;
    }
    catch(...)
    {
        std::cout << "ERROR: the opening of the file was unsuccessful..." << std::endl;
    }
}
