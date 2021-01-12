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
#include "connection_request_data.hpp"



#ifndef I_GUI_SIGNAL_HPP
#define I_GUI_SIGNAL_HPP



class I_GuiSignal
{
signals:
    virtual void StartsToRun(void) = 0;
    virtual void ShuttingDown(void) = 0;
    virtual void OpenNetworkConnection(const ConnectionRequestData& request_data) = 0;
    virtual void CloseNetworkConnection(const QString& user_defined_name) = 0;
    virtual void RequestForDiagram(const QModelIndex& model_index) = 0;
    virtual void ImportFile(const std::string& path_to_file) = 0;
    virtual void ExportFileShowCheckBoxes(void) = 0;
    virtual void ExportFileHideCheckBoxes(void) = 0;
    virtual void ExportFileStoreCheckedDiagrams(const std::string& path_to_file) = 0;

protected:
    ~I_GuiSignal() {}
};

Q_DECLARE_INTERFACE(I_GuiSignal, "I_GuiSignal")

#endif // I_GUI_SIGNAL_HPP
