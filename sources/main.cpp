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

std::atomic_bool shutdown_worker_thread;

#warning "Only debug..."
#include <chrono>
    bool demoDiagramWasAdded = false;
#warning "Only debug..."

void WorkerThread(void)
{
    while(!shutdown_worker_thread)
    {
#warning "Only debug..."
        if(!demoDiagramWasAdded)
        {
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
            demoDiagramWasAdded = true;

            DiagramSpecialized myDiagram("MyTestDiagram", "Degrees");
            myDiagram.AddNewDataLine("Sin");
            myDiagram.AddNewDataLine("Cos");

            for(DataIndexType loop_point = 0; loop_point < 360; loop_point++)
            {
                DataPointSpecialized SinDataPoint(loop_point, sin(double(loop_point) * (3.14159265/180.0)));
                DataPointSpecialized CosDataPoint(loop_point, cos(double(loop_point) * (3.14159265/180.0)));
                myDiagram.AddNewDataPoint(0, SinDataPoint);
                myDiagram.AddNewDataPoint(1, CosDataPoint);
            }

            Gui::GetInstance().AddToDiagramList(myDiagram);
        }
#warning "Only debug..."

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
