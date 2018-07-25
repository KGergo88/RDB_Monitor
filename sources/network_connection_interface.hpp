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



#include <memory>
#include <string>

#include "global.hpp"



#ifndef NETWORK_CONNECTION_INTERFACE_H
#define NETWORK_CONNECTION_INTERFACE_H



class NetworkConnectionInterface
{
public:
    virtual ~NetworkConnectionInterface();

    virtual bool Open(const std::string& port_name) = 0;

    virtual void Close(void) = 0;

    virtual bool IsOpen(void) = 0;

    virtual std::unique_ptr<std::istream> Listen(const std::string& delimiter, const std::size_t& max_line_length) = 0;
};



#endif // NETWORK_CONNECTION_INTERFACE_H
