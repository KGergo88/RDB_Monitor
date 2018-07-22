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



SerialPort::~SerialPort()
{
    Close();
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
            if(port->open(QIODevice::ReadWrite))
            {
                port->setBaudRate(SERIAL_PORT_DEFAULT_BAUDRATE);
                port->setDataBits(QSerialPort::Data8);
                port->setStopBits(QSerialPort::OneStop);
                port->setParity(QSerialPort::NoParity);
                port->setFlowControl(QSerialPort::NoFlowControl);
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

    bAboutToClose = true;

    std::lock_guard<std::mutex> lock_listener(mutex_listener);

    if(port)
    {
        port->close();
        port.reset();
        bAboutToClose = false;
    }
}

bool SerialPort::IsOpen()
{
    std::lock_guard<std::mutex> lock_open_close(mutex_open_close);

    return (nullptr != port);
}

std::unique_ptr<std::istream> SerialPort::Listen(const std::string& delimiter,
                                                 const std::size_t& max_length = SERIAL_PORT_MAX_READ_LENGTH_IN_BYTES)
{
    std::lock_guard<std::mutex> lock_listener(mutex_listener);

    bool delimiter_was_found = false;
    bool maximal_data_size_was_reached = false;
    bool error_was_detected = false;

    std::size_t total_received_data_length = 0;
    auto data_stream = std::make_unique<std::stringstream>();

    while(port && !bAboutToClose &&
          !delimiter_was_found &&
          !maximal_data_size_was_reached &&
          !error_was_detected)
    {        
#warning "Magic number..."

        QByteArray received_data;
        do
        {
            received_data.append(port->readAll());

            if(received_data.size() <= max_length)
            {
                if(received_data.contains(QByteArray::fromStdString(delimiter)))
                {
                    delimiter_was_found = true;
                }
            }
            else
            {
                maximal_data_size_was_reached = true;
            }

        }
        while(port->waitForReadyRead(10));

//        }
//        else
//        {
//            auto port_error = port->error();
//            if(port_error == QSerialPort::NoError)
//            {
//                // Everything is OK but there is no data to read...
//            }
//            else if(port_error == QSerialPort::TimeoutError)
//            {
//                // Timeout happened, this means that no data was available...
//            }
//            else if(port_error == QSerialPort::ReadError)
//            {
//                std::cerr << QObject::tr("Failed to read from serial port, error: %1").arg(port->errorString()).toStdString() << std::endl;
//                error_was_detected = true;
//            }
//            else
//            {
//                std::cerr << "Unknown error happened, error value: " << port_error << std::endl;
//                error_was_detected = true;
//            }
//        }
    }

    // In these cases we will not return a valid data stream...
    if(!port || bAboutToClose)
    {
        data_stream.reset();
    }

    return data_stream;
}

#warning "Saved code..."

//std::unique_ptr<std::istream> SerialPort::Listen(const std::string& delimiter,
//                                                 const std::size_t& max_length = SERIAL_PORT_MAX_READ_LENGTH_IN_BYTES)
//{
//    std::lock_guard<std::mutex> lock_listener(mutex_listener);

//    bool delimiter_was_found = false;
//    bool maximal_data_size_was_reached = false;
//    bool error_was_detected = false;

//    std::size_t total_received_data_length = 0;
//    auto data_stream = std::make_unique<std::stringstream>();

//    while(port && !bAboutToClose &&
//          !delimiter_was_found &&
//          !maximal_data_size_was_reached &&
//          !error_was_detected)
//    {
//#warning "Magic number..."

//        auto responseData = port->readAll();
//        while (serial.waitForReadyRead(10))
//        {
//            responseData += serial.readAll();
//        }


//        if(port->waitForReadyRead(100) && port->canReadLine())
//        {
//            auto received_data = port->readLine(max_length - total_received_data_length);

//#warning "Only debug..."
//std::cout << received_data.toStdString() << std::flush;
//#warning "Only debug..."

//            total_received_data_length += received_data.size();
//            if(total_received_data_length <= max_length)
//            {
//                *data_stream << received_data.toStdString();

//                if(received_data.contains(QByteArray::fromStdString(delimiter)))
//                {
//                    delimiter_was_found = true;
//                }
//            }
//            else
//            {
//                maximal_data_size_was_reached = true;
//            }
//        }
//        else
//        {
//            auto port_error = port->error();
//            if(port_error == QSerialPort::NoError)
//            {
//                // Everything is OK but there is no data to read...
//            }
//            else if(port_error == QSerialPort::TimeoutError)
//            {
//                // Timeout happened, this means that no data was available...
//            }
//            else if(port_error == QSerialPort::ReadError)
//            {
//                std::cerr << QObject::tr("Failed to read from serial port, error: %1").arg(port->errorString()).toStdString() << std::endl;
//                error_was_detected = true;
//            }
//            else
//            {
//                std::cerr << "Unknown error happened, error value: " << port_error << std::endl;
//                error_was_detected = true;
//            }
//        }
//    }

//    // In these cases we will not return a valid data stream...
//    if(!port || bAboutToClose)
//    {
//        data_stream.reset();
//    }

//    return data_stream;
//}

#warning "Saved code..."
