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



#include <istream>
#include <memory>
#include <string>

#include <QtPlugin>

#include "global.hpp"



#ifndef I_NETWORK_CONNECTION_HPP
#define I_NETWORK_CONNECTION_HPP



class I_NetworkConnection
{
public:
    virtual std::string getNetworkConnectionName(void) = 0;
    virtual bool Open(const std::string& port_name) = 0;
    virtual void Close(void) = 0;
    virtual bool IsOpen(void) = 0;
    virtual bool StartListening(void) = 0;

protected:
    ~I_NetworkConnection() {}

signals:
    virtual void DataReceived(std::istream& received_data) = 0;
    virtual void ErrorReport(const std::string& error_message) = 0;
};

Q_DECLARE_INTERFACE(I_NetworkConnection, "NetworkConnectionInterface")



#endif // NETWORK_CONNECTION_INTERFACE_HPP
