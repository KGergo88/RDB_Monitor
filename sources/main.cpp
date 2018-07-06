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

#warning "Only debug..."
#include <chrono>
    bool demoDiagramWasAdded = false;
#warning "Only debug..."

void WorkerThread(void)
{
    while(!shutdown_worker_thread)
    {
        while(!Gui::GetInstance().IsRunning());
#warning "Only debug..."
        if(!demoDiagramWasAdded)
        {
            demoDiagramWasAdded = true;
            //std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
            std::ifstream sample_measurement_data;

            try
            {
                sample_measurement_data.open("/media/gergo/Data_HDD/Programozas/RDB_Diplomaterv_Monitor/MotorTestOutput.txt");

                auto diagram_container = DataProcessor::GetInstance().GetInstance().ProcessData("File", sample_measurement_data);

                for(auto &i : diagram_container)
                {
                    Gui::GetInstance().AddToDiagramList(std::move(*i.release()));
                }
            }
            catch(...)
            {
                std::cout << "Exception!" << std::endl;
            }

            sample_measurement_data.close();
        }
#warning "Only debug..."

//        if(SerialPort::GetInstance().IsOpen())
//        {
//            auto received_data = SerialPort::GetInstance().ReceiveMeasurementData();
//            if(received_data)
//            {
//                auto diagram_container = DataProcessor::GetInstance().ProcessData("SerialPort", *received_data);
//                if(!assembled_diagram.empty())
//                {
//                    for(const auto &i : diagram_container)
//                    {
//                        Gui::GetInstance().AddToDiagramList(*i);
//                    }
//                }
//                else
//                {
//                    std::cerr << "We could not assemble any diagram from the input data..." << std::endl;
//                }
//            }
//        }
    }
}

int main(int argc, char **argv)
{
    shutdown_worker_thread = false;

    std::cout << "Hello RDB!" << std::endl << std::endl;

    std::thread worker_thread(WorkerThread);

    Gui::SetArgcArgv(argc, argv);
    Gui::GetInstance().Run();
    std::cout << "The GUI has stopped." << std::endl;

    std::cout << "Stopping the worker thread." << std::endl;
    shutdown_worker_thread = true;

    worker_thread.join();
    std::cout << "The worker thread has stopped." << std::endl;

    std::cout << "The End." << std::endl;
    return EXIT_SUCCESS;
#warning "Linux: The program has unexpectedly finished. The process was ended forcefully. This is probably because the Gui module has memory leak...check it later with Valgrind!"
}
