#include "serial_port.hpp"

SerialPort::SerialPort(const std::string& new_device_name, const uint32_t& new_baud_rate) : io_service(), port(io_service)
{
    port.open(new_device_name);
    port.set_option(boost::asio::serial_port::baud_rate(new_baud_rate));
}

SerialPort::~SerialPort()
{
   port.get_io_service().reset();
   port.close();
}

std::shared_ptr<std::istream> SerialPort::ReceiveMeasurementData(void)
{
    port.get_io_service().reset();
    boost::asio::async_read_until(port, buffer, "<<<END>>>", boost::bind(&SerialPort::AsyncFinished, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    port.get_io_service().run();

    auto received_data = std::make_shared<std::istream>(&buffer);

    return received_data;
}

void SerialPort::AsyncFinished(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    (void) bytes_transferred;

    if(error)
    {
        std::cout << "SerialPort::AsyncFinished: The error was set: " << error.message() << std::endl;
    }
}
