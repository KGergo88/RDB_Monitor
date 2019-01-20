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

    if(network_connection_interface && data_processing_interface && diagram_collector)
    {
        if(network_connection_interface->Open(new_port_name))
        {
            if(network_connection_interface->StartListening())
            {
                QObject::connect(dynamic_cast<QObject*>(network_connection_interface), SIGNAL(DataReceived(std::istream&)), this, SLOT(DataAvailable(std::istream&)));
                port_name = new_port_name;
                result = true;
            }
        }
    }

    return result;
}

void NetworkHandler::Stop(void)
{
    if(network_connection_interface)
    {
        network_connection_interface->Close();
        QObject::disconnect(dynamic_cast<QObject*>(network_connection_interface), SIGNAL(DataReceived(std::istream&)), this, SLOT(DataAvailable(std::istream&)));
    }
}

void NetworkHandler::DataAvailable(std::istream& received_data)
{
    if(diagram_collector)
    {
        auto assembled_diagrams = data_processing_interface->ProcessData(port_name, received_data);

        if(!assembled_diagrams.empty())
        {
            diagram_collector(assembled_diagrams);
        }
    }
}
