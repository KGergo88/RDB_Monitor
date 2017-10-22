#include <iostream>
#include <memory>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

class SerialPort
{
public:
    SerialPort(const std::string& new_device_name, const uint32_t& new_baud_rate);
    ~SerialPort();

    SerialPort(const SerialPort&) = delete;
    SerialPort(SerialPort&&) = delete;

    SerialPort& operator=(const SerialPort&) = delete;
    SerialPort& operator=(SerialPort&&) = delete;

    std::shared_ptr<std::istream> ReceiveMeasurementData(void);

private:
    boost::asio::io_service  io_service;
    boost::asio::serial_port port;
    boost::asio::streambuf buffer;

    void AsyncFinished(const boost::system::error_code& error, std::size_t bytes_transferred);
};

#endif // SERIAL_PORT_HPP
