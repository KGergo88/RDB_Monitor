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


#ifndef SERIAL_PORT_SETTINGS_HPP
#define SERIAL_PORT_SETTINGS_HPP


#include <QSysInfo>
#include <QSerialPort>

#include "i_connection_settings.hpp"


static constexpr char serial_port_connection_name[] = "SerialPort";

class SerialPortSettings : public I_ConnectionSettings
{
public:
    SerialPortSettings()
    {
        auto os_name = QSysInfo::productType();

        if(("winrt" == os_name) && ("windows" == os_name))
        {
            portName = "COM3";
        }
        else
        {
            portName = "/dev/ttyACM0";
        }
        baudRate = QSerialPort::BaudRate::Baud115200;
        dataBits = QSerialPort::DataBits::Data8;
        stopBits = QSerialPort::StopBits::OneStop;
        parity = QSerialPort::Parity::NoParity;
        flowControl = QSerialPort::FlowControl::NoFlowControl;
    }

    virtual ~SerialPortSettings() {};

    QString portName;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parity;
    QSerialPort::FlowControl flowControl;
};


#endif // SERIAL_PORT_SETTINGS_HPP
