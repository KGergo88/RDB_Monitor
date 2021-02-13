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


#ifndef NETWORK_HANDLER_HPP
#define NETWORK_HANDLER_HPP


#include <iostream>
#include <memory>

#include <QObject>
#include <QString>
#include <QModelIndex>

#include "diagram.hpp"
#include "i_protocol.hpp"

class I_Connection;
class I_ConnectionSettings;

class NetworkHandler : public QObject
{
    Q_OBJECT

public:
    using error_collector_t = std::function<void(const std::string&)>;

    NetworkHandler(const QString& user_defined_name,
                   std::shared_ptr<I_Connection> connection_interface,
                   std::shared_ptr<I_ConnectionSettings> connection_settings,
                   std::shared_ptr<I_Protocol> protocol_interface,
                   I_Protocol::diagram_collector_t diagram_collector,
                   I_Protocol::diagram_updater_t diagram_updater,
                   error_collector_t error_collector);

    ~NetworkHandler() = default;

    NetworkHandler(const NetworkHandler&) = delete;
    NetworkHandler(NetworkHandler&&) = delete;

    NetworkHandler& operator=(const NetworkHandler&) = delete;
    NetworkHandler& operator=(NetworkHandler&&) = delete;

    bool Run(void);
    void Stop(void);
    QString GetUserDefinedName(void) { return m_user_defined_name; }

private:
    void DataAvailable(std::istream& received_data);

    QString m_user_defined_name;
    std::shared_ptr<I_Connection> m_connection_interface;
    std::shared_ptr<I_ConnectionSettings> m_connection_settings;
    std::shared_ptr<I_Protocol> m_protocol_interface;
    I_Protocol::diagram_collector_t m_diagram_collector;
    I_Protocol::diagram_updater_t m_diagram_updater;
    error_collector_t m_error_collector;
};


#endif // NETWORK_HANDLER_HPP
