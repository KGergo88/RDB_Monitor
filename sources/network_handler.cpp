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



#include "network_handler.hpp"



bool NetworkHandler::Run(const std::string& new_port_name)
{
    bool result = true;

    if(network_connection && data_processor && diagram_collector)
    {
        if(network_connection->Open(port_name))
        {
            if(network_connection->StartListening())
            {
                QObject::connect(dynamic_cast<QObject *>(network_connection), SIGNAL(DataReceived(std::istream& received_data)), this, SLOT(DataAvailable(std::istream& received_data)));
                port_name = new_port_name;
                result = true;
            }
        }
    }

    return result;
}

void NetworkHandler::Stop(void)
{
    if(network_connection)
    {
        network_connection->Close();
    }
}

void NetworkHandler::DataAvailable(std::istream& received_data)
{
    auto assembled_diagrams = data_processor->ProcessData(port_name, received_data);

    for(const auto& i: assembled_diagrams)
    {
        diagram_collector(std::move(*i));
    }
}
