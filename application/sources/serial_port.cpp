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


#include "serial_port.hpp"
#include "serial_port_settings.hpp"


constexpr char serial_port_connection_name[] = "SerialPort";


std::string SerialPort::getName(void)
{
    return std::string(serial_port_connection_name);
}

bool SerialPort::Open(const std::shared_ptr<I_ConnectionSettings> settings)
{
    bool result = false;
    std::shared_ptr<SerialPortSettings> serial_port_settings = std::dynamic_pointer_cast<SerialPortSettings>(settings);

    if(!port)
    {
        try
        {
            std::shared_ptr<SerialPortSettings> serial_port_settings = std::dynamic_pointer_cast<SerialPortSettings>(settings);
            port = std::make_unique<QSerialPort>();
            port->setPortName(serial_port_settings->portName);
            port->setBaudRate(serial_port_settings->baudRate);
            port->setDataBits(serial_port_settings->dataBits);
            port->setStopBits(serial_port_settings->stopBits);
            port->setParity(serial_port_settings->parity);
            port->setFlowControl(serial_port_settings->flowControl);

            if(port->open(QIODevice::ReadOnly))
            {
                QObject::connect(port.get(), &QSerialPort::readyRead,       this, &SerialPort::ReadLineFromPort);
                QObject::connect(port.get(), &QSerialPort::errorOccurred,   this, &SerialPort::HandleErrors);
                result = true;
            }
            else
            {
                port.reset();
            }
        }
        catch(...)
        {
            port.reset();
            throw("SerialPort::Open: Could not open port! (" + serial_port_settings->portName + "), probably a bad allocation in std::make_unique().");
        }
    }
    else
    {
        if(serial_port_settings->portName == port->portName())
        {
            result = true;
        }
        else
        {
            throw("SerialPort::Open: Another serial port was already openend with this object: " + serial_port_settings->portName);
        }
    }

    return result;
}

void SerialPort::Close()
{
    if(port)
    {
        QObject::disconnect(port.get(), &QSerialPort::readyRead, this, &SerialPort::ReadLineFromPort);
        port->close();
        port.reset();
    }
}

bool SerialPort::IsOpen()
{
    return (nullptr != port);
}

void SerialPort::ReadLineFromPort(void)
{
    bool at_least_one_line_was_received = false;
    std::string received_lines;
    while(port->canReadLine())
    {
        at_least_one_line_was_received = true;
        received_lines += port->readLine().toStdString();
    }
    if(at_least_one_line_was_received)
    {
        std::stringstream received_data_stream(received_lines);
        emit DataReceived(received_data_stream);
    }
}

void SerialPort::HandleErrors(QSerialPort::SerialPortError error)
{
    if(QSerialPort::ReadError == error)
    {
        emit ErrorReport((QObject::tr("SerialPort::HandleErrors: An I/O error occurred while reading the data from port %1, error: %2").arg(port->portName()).arg(port->errorString())).toStdString());
    }
}
