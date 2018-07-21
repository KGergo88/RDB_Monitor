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



#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <signal.h>

#include "global.hpp"
#include "diagram.hpp"
#include "gui.hpp"
#include "serial_port.hpp"
#include "data_processor.hpp"



std::atomic_bool shutdown_worker_thread;



void WorkerThread(void)
{
    while(!Gui::IsRunning());

    while(!shutdown_worker_thread)
    {
        if(SerialPort::GetInstance().IsOpen())
        {
            auto received_data = SerialPort::GetInstance().ReceiveMeasurementData();
            if(received_data)
            {
                auto diagram_container = DataProcessor::GetInstance().ProcessData("SerialPort", *received_data);
                if(!diagram_container.empty())
                {
                    for(auto &i : diagram_container)
                    {
                        Gui::AddToDiagramList(std::move(*i.release()));
                    }
                }
                else
                {
                    std::cerr << "We could not assemble any diagram from the input data..." << std::endl;
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    try
    {
        shutdown_worker_thread = false;

        std::cout << "Hello RDB!" << std::endl << std::endl;

        std::thread worker_thread(WorkerThread);

        Gui::Run(argc, argv);
        std::cout << "The GUI has stopped." << std::endl;

        std::cout << "Stopping the worker thread." << std::endl;
        shutdown_worker_thread = true;

        worker_thread.join();
        std::cout << "The worker thread has stopped." << std::endl;

        std::cout << "The End." << std::endl;
        return EXIT_SUCCESS;
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
