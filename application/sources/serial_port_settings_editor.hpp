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



#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

#include "i_connection_settings_editor.hpp"
#include "serial_port_settings.hpp"


#ifndef SERIAL_PORT_SETTINGS_EDITOR_HPP
#define SERIAL_PORT_SETTINGS_EDITOR_HPP



class SerialPortSettingsEditor : public I_ConnectionSettingsEditor
{
    Q_OBJECT
    Q_INTERFACES(I_ConnectionSettingsEditor)

signals:
    void settingsChanged(bool settings_valid, const QString& error_message) override;

public:
    SerialPortSettingsEditor(QWidget* parent);

    SerialPortSettingsEditor(const SerialPortSettingsEditor&) = delete;
    SerialPortSettingsEditor(SerialPortSettingsEditor&&) = delete;

    virtual ~SerialPortSettingsEditor() = default;

    SerialPortSettingsEditor& operator=(const SerialPortSettingsEditor&) = delete;
    SerialPortSettingsEditor& operator=(SerialPortSettingsEditor&&) = delete;

    QString getConnectionName(void) override { return serial_port_connection_name; }

    std::shared_ptr<I_ConnectionSettings> getSettings(void) override;

    std::shared_ptr<I_ConnectionSettings> getDefaultSettings(void) override;

private slots:
    void reviewSettings(void);

private:
    QGridLayout* pGridLayout;
    QLineEdit* pPortNameLineEdit;
    QComboBox* pBaudRateComboBox;
    QComboBox* pDataBitsComboBox;
    QComboBox* pStopBitsComboBox;
    QComboBox* pParityComboBox;
    QComboBox* pFlowControlComboBox;
};



#endif // SERIAL_PORT_SETTINGS_EDITOR_HPP
