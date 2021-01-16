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


#ifndef CONNECTION_FACTORY_HPP
#define CONNECTION_FACTORY_HPP


#include <memory>

#include "i_connection.hpp"
#include "serial_port.hpp"


class ConnectionFactory
{
public:
    static std::shared_ptr<I_Connection> make(const QString& connection_name)
    {
        std::shared_ptr<I_Connection> protocol;

        if(connection_name == QString(serial_port_connection_name))
        {
            protocol = std::make_shared<SerialPort>();
        }
        else
        {
            throw ("ConnectionFactory::make(): Unknown connection \"" + connection_name.toStdString() + "\"");
        }

        return protocol;
    }
};


#endif // CONNECTION_FACTORY_HPP
