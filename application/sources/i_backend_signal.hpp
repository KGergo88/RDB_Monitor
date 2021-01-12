//==============================================================================//
//                                                                              //
//    RDB Monitor                                                               //
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
#include <QAbstractItemModel>

#include "global.hpp"



#ifndef I_BACKEND_SIGNAL_HPP
#define I_BACKEND_SIGNAL_HPP



class I_BackendSignal
{
protected:
    ~I_BackendSignal() {}

public:
    virtual QAbstractItemModel* GetDiagramContainerModel(void) = 0;
    virtual std::string GetFileImportDefaultFolder(void) = 0;
    virtual std::string GetFileExportDefaultFolder(void) = 0;
    virtual std::vector<std::string> GetSupportedFileExtensions(void) = 0;
    virtual QStringList GetAvailableConnections(void) = 0;
    virtual QStringList GetAvailableProtocols(void) = 0;

signals:
    virtual void NewStatusMessage(const std::string& message_text) = 0;
    virtual void ListOfActiveConnectionsChanged(const QStringList& active_connections) = 0;
    virtual void ShowThisDiagram(const DiagramSpecialized& diagram) = 0;
};

Q_DECLARE_INTERFACE(I_BackendSignal, "I_BackendSignal")



#endif // I_BACKEND_SIGNAL_HPP
