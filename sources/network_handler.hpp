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
#include <functional>

#include "global.hpp"
#include "network_connection_interface.hpp"
#include "data_processor_interface.hpp"
#include "diagram.hpp"



#ifndef NETWORK_HANDLER_HPP
#define NETWORK_HANDLER_HPP



class NetworkHandler
{
public:
    NetworkHandler(NetworkConnectionInterface *new_network_connection,
                   DataProcessorInterface *new_data_processor)
                              : network_connection(new_network_connection),
                                data_processor(new_data_processor)
    {
        if(!network_connection)
        {
            std::string errorMessage = "There was no network_connection set in NetworkHandler::NetworkHandler!";
            throw errorMessage;
        }
        if(!data_processor)
        {
            std::string errorMessage = "There was no data_connection set in NetworkHandler::NetworkHandler!";
            throw errorMessage;
        }
    }

    void Run(const std::string& port_name, std::function<void(DiagramSpecialized&&)> diagram_collector)
    {
        if(network_connection && data_processor)
        {
            if(!network_connection->Open(port_name))
            {
                std::string errorMessage = "The network_connection " + port_name + " could not be opened!";
                throw errorMessage;
            }

            while(network_connection->IsOpen())
            {
                auto received_data = network_connection->Listen(DATA_END_LINE, SERIAL_PORT_MAX_READ_LENGTH_IN_BYTES);
                if(received_data)
                {
                    auto diagram_container = data_processor->ProcessData("SerialPort", *received_data);
                    if(!diagram_container.empty())
                    {
                        for(auto &i : diagram_container)
                        {
                            diagram_collector(std::move(*i.release()));
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

    void Stop(void)
    {
        if(network_connection)
        {
            network_connection->Close();
        }
    }

private:
    NetworkConnectionInterface *const network_connection = nullptr;
    DataProcessorInterface *const data_processor = nullptr;
};



#endif // NETWORK_HANDLER_HPP
