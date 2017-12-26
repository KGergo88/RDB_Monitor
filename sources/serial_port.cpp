#include "serial_port.hpp"

SerialPort::SerialPort() : io_service(), port(io_service)
{

}

SerialPort::~SerialPort()
{
    Close();
}

bool SerialPort::Open(const std::string& device_to_open)
{
    bool result = false;

    if(!port.is_open())
    {
        try
        {
            port.open(device_to_open);
            port.set_option(boost::asio::serial_port::baud_rate(baud_rate));

            if(port.is_open())
            {
                device_name = device_to_open;
                result = true;
            }
            else
            {
                std::cerr << "Could not open the SerialPort. Device: " << device_to_open << std::endl;
            }
        }
        catch(...)
        {
            std::cerr << "Could not open the SerialPort. Device: " << device_to_open << std::endl;
        }
    }
    else
    {
        if(device_name == device_to_open)
        {
            result = true;
        }
        else
        {
            std::cerr << "Another serial port was already open: " << device_name << std::endl;
        }
    }

    return result;
}

bool SerialPort::Close()
{
    bool result = false;

    if(port.is_open())
    {
        //This flag is needed to prevent starting new reads and to suppress error messages...
        ShutdownWasRequested = true;
        port.close();
        if(!port.is_open())
        {
            result = true;
        }
        else
        {
            std::cerr << "Could not close the serial port. Device name: " << device_name << std::endl;
        }
        ShutdownWasRequested = false;
    }
    else
    {
        result = true;
    }

    return result;
}

bool SerialPort::IsOpen()
{
    return (port.is_open() && !ShutdownWasRequested);
}

std::shared_ptr<std::istream> SerialPort::ReceiveMeasurementData(void)
{
    std::shared_ptr<std::istream> received_data(nullptr);

    if(port.is_open() && !ShutdownWasRequested)
    {
        port.get_io_service().reset();
        boost::asio::async_read_until(port, buffer, DATA_END_LINE, boost::bind(&SerialPort::AsyncFinished, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        port.get_io_service().run();

        //This is needed for the case when we are waiting for a transmission, but the serial port closed in the meantime.
        if(port.is_open() && !ShutdownWasRequested)
        {
            received_data = std::make_shared<std::istream>(&buffer);
        }
    }

    return received_data;
}

void SerialPort::AsyncFinished(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    //The finished event is only interesting for us if the port is open, so it was not raised because of a cancelled operation.
    if(port.is_open() && !ShutdownWasRequested)
    {
        (void) bytes_transferred;

        if(error)
        {
            std::cerr << "SerialPort::AsyncFinished: The error was set: " << error.message() << std::endl;
        }
    }
}
