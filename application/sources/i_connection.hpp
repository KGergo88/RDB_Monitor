//==============================================================================//
//                                                                              //
//    RDB Monitor                                                               //
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
#include "i_connection_settings.hpp"



#ifndef I_CONNECTION_HPP
#define I_CONNECTION_HPP



class I_Connection
{
public:
    virtual ~I_Connection() = default;

    virtual std::string getName(void) = 0;
    virtual bool Open(const std::shared_ptr<I_ConnectionSettings> settings) = 0;
    virtual void Close(void) = 0;
    virtual bool IsOpen(void) = 0;

signals:
    virtual void DataReceived(std::istream& received_data) = 0;
    virtual void ErrorReport(const std::string& error_message) = 0;
};

Q_DECLARE_INTERFACE(I_Connection, "ConnectionInterface")



#endif // I_CONNECTION_HPP
