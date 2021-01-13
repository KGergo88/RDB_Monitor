//==============================================================================//
//                                                                              //
//    RDB Monitor                                                               //
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



bool NetworkHandler::Run(void)
{
    bool result = false;

    if(connection_interface->Open(connection_settings))
    {
        QObject::connect(dynamic_cast<QObject*>(connection_interface.get()), SIGNAL(DataReceived(std::istream&)),     this, SLOT(DataAvailable(std::istream&)));
        QObject::connect(dynamic_cast<QObject*>(connection_interface.get()), SIGNAL(ErrorReport(const std::string&)), this, SLOT(ErrorReport(const std::string&)));
        result = true;
    }

    return result;
}

void NetworkHandler::Stop(void)
{
    connection_interface->Close();
    QObject::disconnect(dynamic_cast<QObject*>(connection_interface.get()), SIGNAL(DataReceived(std::istream&)),     this, SLOT(DataAvailable(std::istream&)));
    QObject::disconnect(dynamic_cast<QObject*>(connection_interface.get()), SIGNAL(ErrorReport(const std::string&)), this, SLOT(ErrorReport(const std::string&)));
}

void NetworkHandler::DataAvailable(std::istream& received_data)
{
    auto assembled_diagrams = protocol_interface->ProcessData(received_data);

    if(!assembled_diagrams.empty())
    {
        diagram_collector(user_defined_name, assembled_diagrams);
    }
}

void NetworkHandler::ErrorReport(const std::string& error_message)
{
    error_collector(error_message);
}
