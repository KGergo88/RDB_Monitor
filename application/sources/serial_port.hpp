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



#include <iostream>
#include <sstream>
#include <memory>

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "global.hpp"
#include "i_connection.hpp"
#include "i_connection_settings.hpp"
#include "serial_port_settings.hpp"



#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP



class SerialPort : public QObject, public I_Connection
{
    Q_OBJECT
    Q_INTERFACES(I_Connection)

public:
    SerialPort() = default;
    ~SerialPort();

    SerialPort(const SerialPort&) = delete;
    SerialPort(SerialPort&&) = delete;

    SerialPort& operator=(const SerialPort&) = delete;
    SerialPort& operator=(SerialPort&&) = delete;

    std::string getName(void) override { return std::string(serial_port_connection_name); }
    bool Open(const std::shared_ptr<I_ConnectionSettings> settings) override;
    void Close(void) override;
    bool IsOpen(void) override;

signals:
    void DataReceived(std::istream& received_data) override;
    void ErrorReport(const std::string& error_message) override;

private slots:
    void ReadLineFromPort(void);
    void HandleErrors(QSerialPort::SerialPortError error);

private:
    std::unique_ptr<QSerialPort> port;
};



#endif // SERIAL_PORT_HPP
