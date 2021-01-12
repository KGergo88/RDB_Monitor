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



#include <memory>

#include <QWidget>

#include "i_connection_settings.hpp"



#ifndef I_CONNECTION_SETTINGS_EDITOR_HPP
#define I_CONNECTION_SETTINGS_EDITOR_HPP



class I_ConnectionSettingsEditor : public QWidget
{
signals:
    virtual void settingsChanged(bool settings_valid, const QString& error_message) = 0;

public:
    I_ConnectionSettingsEditor(QWidget* parent) : QWidget(parent) { }
    virtual ~I_ConnectionSettingsEditor() = default;

    // Returns a the name of the connection (eg. SerialPort)
    virtual QString getConnectionName(void) = 0;
    // Returns the current settings
    virtual std::shared_ptr<I_ConnectionSettings> getSettings(void) = 0;
    // Returns the default settings
    virtual std::shared_ptr<I_ConnectionSettings> getDefaultSettings(void) = 0;    
};

Q_DECLARE_INTERFACE(I_ConnectionSettingsEditor, "I_ConnectionSettingsEditor")

#endif // I_CONNECTION_SETTINGS_EDITOR_HPP
