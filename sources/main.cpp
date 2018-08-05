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



#include <iostream>
#include <string>

#include <QApplication>

#include "global.hpp"
#include "backend.hpp"



int main(int argc, char **argv)
{
    try
    {
        std::cout << "Hello RDB!" << std::endl;

        std::cout << "Running the GUI." << std::endl;
        QApplication q_application(argc, argv);

        auto q_application_result = q_application.exec();
        std::cout << "The GUI has stopped." << std::endl;

        std::cout << "The End." << std::endl;
        return q_application_result;
    }
    catch(const std::string& exception_text)
    {
        std::cerr << "An exception with a string was caught! Content: " << exception_text << std::endl;
    }
    catch(...)
    {
        std::cerr << "An exception was caught without any detailed info!" << std::endl;
    }
}
