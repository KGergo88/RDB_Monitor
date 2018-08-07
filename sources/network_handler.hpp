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
#include <memory>
#include <functional>

#include <QObject>

#include "global.hpp"
#include "network_connection_interface.hpp"
#include "data_processing_interface.hpp"
#include "diagram.hpp"



#ifndef NETWORK_HANDLER_HPP
#define NETWORK_HANDLER_HPP



class NetworkHandler : public QObject
{
    Q_OBJECT

public:

    using diagram_collector_type = std::function<void(std::vector<std::unique_ptr<DiagramSpecialized> >&&)>;

    NetworkHandler(NetworkConnectionInterface *new_network_connection_interface,
                   DataProcessingInterface *new_data_processor_interface,
                   diagram_collector_type new_diagram_collector)
                              : network_connection_interface(new_network_connection_interface),
                                data_processor_interface(new_data_processor_interface),
                                diagram_collector(new_diagram_collector)
    {
        if(!network_connection_interface)
        {
            std::string errorMessage = "There was no network_connection_interface set in NetworkHandler::NetworkHandler!";
            throw errorMessage;
        }
        if(!data_processor_interface)
        {
            std::string errorMessage = "There was no data_processor_interface set in NetworkHandler::NetworkHandler!";
            throw errorMessage;
        }
        if(!diagram_collector)
        {
            std::string errorMessage = "There was no diagram_collector set in NetworkHandler::NetworkHandler!";
            throw errorMessage;
        }
    }

    ~NetworkHandler() = default;

    NetworkHandler(const NetworkHandler&) = delete;
    NetworkHandler(NetworkHandler&&) = delete;

    NetworkHandler& operator=(const NetworkHandler&) = delete;
    NetworkHandler& operator=(NetworkHandler&&) = delete;

    bool Run(const std::string& new_port_name);

    void Stop(void);

private slots:
    void DataAvailable(std::istream& received_data);

private:
    NetworkConnectionInterface* network_connection_interface;
    DataProcessingInterface* data_processor_interface;
    diagram_collector_type diagram_collector;
    std::string port_name;
};



#endif // NETWORK_HANDLER_HPP
