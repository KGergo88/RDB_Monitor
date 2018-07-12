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
#include <atomic>
#include <signal.h>

#include "global.hpp"
#include "diagram.hpp"
#include "gui.hpp"
#include "serial_port.hpp"
#include "data_processor.hpp"

#warning "The exception handling needs to be solved overall the program..."

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
