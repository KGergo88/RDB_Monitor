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


#include <functional>

#include "network_handler.hpp"
#include "i_connection.hpp"
#include "i_protocol.hpp"


NetworkHandler::NetworkHandler(const QString &user_defined_name,
                               std::shared_ptr<I_Connection> connection_interface,
                               std::shared_ptr<I_ConnectionSettings> connection_settings,
                               std::shared_ptr<I_Protocol> protocol_interface,
                               I_Protocol::diagram_collector_t diagram_collector,
                               I_Protocol::diagram_updater_t diagram_updater,
                               error_collector_t error_collector) :
                                    m_user_defined_name(user_defined_name),
                                    m_connection_interface(connection_interface),
                                    m_connection_settings(connection_settings),
                                    m_protocol_interface(protocol_interface),
                                    m_diagram_collector(diagram_collector),
                                    m_diagram_updater(diagram_updater),
                                    m_error_collector(error_collector)
{
    m_connection_interface->RegisterCallbacks(std::bind(&NetworkHandler::DataAvailable, this, std::placeholders::_1),
                                              m_error_collector);
    m_protocol_interface->RegisterCallbacks(m_diagram_collector,
                                            m_diagram_updater,
                                            m_error_collector);
}

bool NetworkHandler::Run(void)
{
    bool result = false;

    if(m_connection_interface->Open(m_connection_settings))
    {
        result = true;
    }

    return result;
}

void NetworkHandler::Stop(void)
{
    m_connection_interface->Close();
}

void NetworkHandler::DataAvailable(std::istream& received_data)
{
    m_protocol_interface->ProcessNetworkData(received_data);
}
