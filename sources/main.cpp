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

std::atomic_bool shutdown_worker_thread = false;

void WorkerThread(void)
{
    while(!shutdown_worker_thread)
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

    std::cout << "Stopping the worker thread." << std::endl;
    shutdown_worker_thread = true;

    worker_thread.join();
    std::cout << "The worker thread has stopped." << std::endl;

    std::cout << "The End." << std::endl;
    return EXIT_SUCCESS;
}
