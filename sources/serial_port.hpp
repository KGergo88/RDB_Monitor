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



#include <iostream>
#include <memory>
#include <mutex>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

#include "global.hpp"

#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

class SerialPort
{
public:
    static inline SerialPort& GetInstance(void)
    {
        static SerialPort Singleton;
        return Singleton;
    }

    ~SerialPort();

    SerialPort(const SerialPort&) = delete;
    SerialPort(SerialPort&&) = delete;

    SerialPort& operator=(const SerialPort&) = delete;
    SerialPort& operator=(SerialPort&&) = delete;

    bool Open(const std::string& device_to_open);
    bool Close();
    bool IsOpen(void);

    std::shared_ptr<std::istream> ReceiveMeasurementData(void);

    static constexpr uint32_t baud_rate = 115200;

private:
    SerialPort();

    bool ShutdownWasRequested = false;
    std::string device_name;
    boost::asio::io_service  io_service;
    boost::asio::serial_port port;
    boost::asio::streambuf buffer;

    void AsyncFinished(const boost::system::error_code& error, std::size_t bytes_transferred);
};

#endif // SERIAL_PORT_HPP
