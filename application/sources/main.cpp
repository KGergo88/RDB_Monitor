//==============================================================================//
//                                                                              //
//    RDB Monitor                                                               //
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

#include <QString>
#include <QApplication>

#include "backend.hpp"
#include "main_window.hpp"


extern const QString application_name = "RDB Monitor";
extern const QString application_version = "v2.2.0";

int main(int argc, char **argv)
{
    int result = EXIT_FAILURE;

    try
    {
        std::cout << "Hello RDB!" << std::endl;

        QApplication q_application(argc, argv);

        Backend backend;
        MainWindow main_window;

        backend.RegisterGuiSignalInterface(&main_window);
        main_window.RegisterBackendSignalInterface(&backend);

        std::cout << "Running the GUI." << std::endl;
        auto q_application_result = q_application.exec();
        std::cout << "The GUI has stopped with result: " << q_application_result << std::endl;

        result = q_application_result;
    }
    catch(const std::string& exception_text)
    {
        std::cerr << "An exception with a string was caught! Content: " << exception_text << std::endl;
    }
    catch(...)
    {
        std::cerr << "An exception was caught without any detailed info!" << std::endl;
    }

    return result;
}
