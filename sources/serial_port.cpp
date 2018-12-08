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



#include "serial_port.hpp"



SerialPort::SerialPort() : QObject()
{

}

SerialPort::~SerialPort()
{
    std::lock_guard<std::mutex> lock_open_close(mutex_open_close);
    std::lock_guard<std::mutex> lock_listener(mutex_listener);

    if(port)
    {
        port->close();
        port.reset();
    }
}

bool SerialPort::Open(const std::string& port_name = SERIAL_PORT_DEFAULT_PORT_NAME)
{
    std::lock_guard<std::mutex> lock_open_close(mutex_open_close);
    std::lock_guard<std::mutex> lock_listener(mutex_listener);

    bool result = false;

    if(!port)
    {
        try
        {
            port = std::make_unique<QSerialPort>();
            port->setPortName(QString::fromStdString(port_name));
            port->setBaudRate(SERIAL_PORT_DEFAULT_BAUDRATE);
            port->setDataBits(QSerialPort::Data8);
            port->setStopBits(QSerialPort::OneStop);
            port->setParity(QSerialPort::NoParity);
            port->setFlowControl(QSerialPort::NoFlowControl);

            if(port->open(QIODevice::ReadOnly))
            {
                result = true;
            }
            else
            {
                std::cerr << "Could not open the SerialPort. Device: " << port_name << std::endl;
                port.reset();
            }
        }
        catch(...)
        {
            std::cerr << "Could not open the SerialPort. Device: " << port_name << std::endl;
            port.reset();
        }
    }
    else
    {
        if(port_name == port->portName().toStdString())
        {
            result = true;
        }
        else
        {
            std::cerr << "Another serial port was already openend with this object: " << port_name << std::endl;
        }
    }

    return result;
}

void SerialPort::Close()
{
    std::lock_guard<std::mutex> lock_open_close(mutex_open_close);
    std::lock_guard<std::mutex> lock_listener(mutex_listener);

    if(port)
    {
        port->close();
        port.reset();
    }

    QObject::disconnect(port.get(), &QSerialPort::readyRead, this, &SerialPort::ReadDataFromPort);
}

bool SerialPort::IsOpen()
{
    std::lock_guard<std::mutex> lock_open_close(mutex_open_close);

    return (nullptr != port);
}

bool SerialPort::StartListening(void)
{
    std::lock_guard<std::mutex> lock_listener(mutex_listener);

    bool result = false;

    if(IsOpen())
    {
        QObject::connect(port.get(), &QSerialPort::readyRead,       this, &SerialPort::ReadDataFromPort);
        QObject::connect(port.get(), &QSerialPort::errorOccurred,   this, &SerialPort::HandleErrors);
        result = true;
    }

    return result;
}

void SerialPort::ReadDataFromPort(void)
{
    std::lock_guard<std::mutex> lock_listener(mutex_listener);

    std::stringstream received_data_stream;

    received_data_stream << port->readAll().toStdString();
    emit DataReceived(received_data_stream);
}

void SerialPort::HandleErrors(QSerialPort::SerialPortError error)
{
    if(QSerialPort::ReadError == error)
    {
        std::cerr << (QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(port->portName()).arg(port->errorString())).toStdString() << std::endl;
    }
}
