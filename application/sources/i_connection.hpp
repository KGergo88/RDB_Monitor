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


#ifndef I_CONNECTION_HPP
#define I_CONNECTION_HPP


#include <istream>
#include <memory>
#include <string>


class I_ConnectionSettings;

class I_Connection
{
public:
    using data_collector_t = std::function<void(std::istream& received_data)>;
    using error_reporter_t = std::function<void(const std::string&)>;

    virtual ~I_Connection() = default;

    virtual void RegisterCallbacks(data_collector_t&& data_collector,
                                   error_reporter_t&& error_reporter)
    {
        m_data_collector = data_collector;
        m_error_reporter = error_reporter;
    }

    virtual std::string getName(void) = 0;
    virtual bool Open(const std::shared_ptr<I_ConnectionSettings> settings) = 0;
    virtual void Close(void) = 0;
    virtual bool IsOpen(void) = 0;

protected:
    data_collector_t m_data_collector;
    error_reporter_t m_error_reporter;
};


#endif // I_CONNECTION_HPP
