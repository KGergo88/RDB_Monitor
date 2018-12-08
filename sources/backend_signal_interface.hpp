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



#include <vector>
#include <string>

#include <QtPlugin>

#include "global.hpp"



#ifndef BACKEND_SIGNAL_INTERFACE_HPP
#define BACKEND_SIGNAL_INTERFACE_HPP


#warning "Maybe you could inherit the inteface classes (removing the \"signal\" here and adding them as public functions) by the emitter and the reciever as well and so both sides would be forced to have this set of functions..."
class BackendSignalInterface
{

protected:
    ~BackendSignalInterface() {}

signals:
    virtual void NewStatusMessage(const std::string& message_text) = 0;
    virtual void NetworkOperationFinished(const std::string& port_name, bool result) = 0;
    virtual void ShowThisDiagram(const DiagramSpecialized& diagram) = 0;
    virtual void DiagramListHasChanged(const std::vector<std::string>& available_diagrams) = 0;

};

Q_DECLARE_INTERFACE(BackendSignalInterface, "BackendSignalInterface")



#endif // BACKEND_SIGNAL_INTERFACE_HPP
