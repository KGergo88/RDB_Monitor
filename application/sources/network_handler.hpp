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
#include "i_connection.hpp"
#include "i_protocol.hpp"
#include "diagram.hpp"



#ifndef NETWORK_HANDLER_HPP
#define NETWORK_HANDLER_HPP



class NetworkHandler : public QObject
{
    Q_OBJECT

public:

    using diagram_collector_type = std::function<void(const std::string, std::vector<DiagramSpecialized>&)>;
    using error_collector_type = std::function<void(const std::string&)>;

    NetworkHandler(I_Connection *new_connection_interface,
                   I_Protocol *new_protocol_interface,
                   diagram_collector_type new_diagram_collector,
                   error_collector_type new_error_collector)
                              : connection_interface(new_connection_interface),
                                protocol_interface(new_protocol_interface),
                                diagram_collector(new_diagram_collector),
                                error_collector(new_error_collector)
    {
        if(!connection_interface)
        {
            std::string errorMessage = "There was no connection_interface set in NetworkHandler::NetworkHandler!";
            throw errorMessage;
        }
        if(!protocol_interface)
        {
            std::string errorMessage = "There was no protocol_interface set in NetworkHandler::NetworkHandler!";
            throw errorMessage;
        }
        if(!diagram_collector)
        {
            std::string errorMessage = "There was no diagram_collector set in NetworkHandler::NetworkHandler!";
            throw errorMessage;
        }
        if(!error_collector)
        {
            std::string errorMessage = "There was no error_collector set in NetworkHandler::NetworkHandler!";
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
    void ErrorReport(const std::string& error_message);

private:
    I_Connection* connection_interface;
    I_Protocol* protocol_interface;
    diagram_collector_type diagram_collector;
    error_collector_type error_collector;
    std::string port_name;
};



#endif // NETWORK_HANDLER_HPP
