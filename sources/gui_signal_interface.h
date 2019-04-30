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



#ifndef GUI_SIGNAL_INTERFACE_H
#define GUI_SIGNAL_INTERFACE_H



class GuiSignalInterface
{
signals:
    virtual void StartsToRun(void) = 0;
    virtual void ShuttingDown(void) = 0;
    virtual void OpenNetworkConnection(const std::string& port_name) = 0;
    virtual void CloseNetworkConnection(const std::string& port_name) = 0;
    virtual void RequestForDiagram(const QModelIndex& model_index) = 0;
    virtual void OpenFile(const std::string& path_to_file) = 0;

protected:
    ~GuiSignalInterface() {}
};

Q_DECLARE_INTERFACE(GuiSignalInterface, "GuiSignalInterface")

#endif // GUI_SIGNAL_INTERFACE_H
