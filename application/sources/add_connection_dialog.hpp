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

#include <QList>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>

#include <QSysInfo>
#include <QGridLayout>
#include <QLabel>


#include "i_gui_signal.hpp"
#include "ordered_dict.h"



#ifndef CONNECTION_MANAGER_DIALOG_HPP
#define CONNECTION_MANAGER_DIALOG_HPP



class I_ConnectionSettingsEditor : public QWidget
{
public:
    using settings_t = OrderedDict<QString, std::size_t, QString>;

    I_ConnectionSettingsEditor(QWidget* parent) : QWidget(parent) { }
    virtual ~I_ConnectionSettingsEditor() = default;

    virtual QString getName(void) = 0;
    virtual settings_t getSettings(void) = 0;
    virtual settings_t getDefaultSettings(void) = 0;
};



class SerialConnectionSettingsEditor : public I_ConnectionSettingsEditor
{
public:
    SerialConnectionSettingsEditor(QWidget* parent) : I_ConnectionSettingsEditor(parent)
    {
        auto default_settings = getDefaultSettings();
        pGridLayout = new QGridLayout(this);
        int actual_row = 0;
        for(auto i = default_settings.begin(); i != default_settings.end(); i++)
        {
            pGridLayout->addWidget(new QLabel(i->first), actual_row, 0);
            pGridLayout->addWidget(new QLabel(i->second), actual_row, 1);
            actual_row++;
        }
    }

    SerialConnectionSettingsEditor(const SerialConnectionSettingsEditor&) = delete;
    SerialConnectionSettingsEditor(SerialConnectionSettingsEditor&&) = delete;

    virtual ~SerialConnectionSettingsEditor() = default;

    SerialConnectionSettingsEditor& operator=(const SerialConnectionSettingsEditor&) = delete;
    SerialConnectionSettingsEditor& operator=(SerialConnectionSettingsEditor&&) = delete;

    virtual QString getName(void) override
    {
        return QString("Serial");
    }

    virtual settings_t getSettings(void) override
    {
        settings_t settings;

        return settings;
    }

    virtual settings_t getDefaultSettings(void) override
    {
        settings_t default_settings;
        auto os_name = QSysInfo::productType();

        default_settings["Name"] = getName();
        QString device_key = "Device";
        if(("winrt" == os_name) && ("windows" == os_name))
        {
            default_settings[device_key] = "COM3";
        }
        else
        {
            default_settings[device_key] = "/dev/ttyACM0";
        }
        default_settings["Baudrate"] = "115200";
        default_settings["Data bits"] =	"8";
        default_settings["Stop bits"] =	"1";
        default_settings["Parity"] = "none";
        default_settings["Flow control"] = "none";

        return default_settings;
    }

private:
    QGridLayout* pGridLayout;
};


class EmptyConnectionSettingsEditor : public I_ConnectionSettingsEditor
{
public:
    EmptyConnectionSettingsEditor(QWidget* parent) : I_ConnectionSettingsEditor(parent) { }

    EmptyConnectionSettingsEditor(const EmptyConnectionSettingsEditor&) = delete;
    EmptyConnectionSettingsEditor(EmptyConnectionSettingsEditor&&) = delete;

    virtual ~EmptyConnectionSettingsEditor() = default;

    EmptyConnectionSettingsEditor& operator=(const EmptyConnectionSettingsEditor&) = delete;
    EmptyConnectionSettingsEditor& operator=(EmptyConnectionSettingsEditor&&) = delete;

    virtual QString getName(void) override
    {
        return QString("");
    }

    virtual settings_t getSettings(void) override
    {
        settings_t settings;

        return settings;
    }

    virtual settings_t getDefaultSettings(void) override
    {
        settings_t default_settings;
        return default_settings;
    }
};


class ConnectionManagerDialog : private QDialog
{
    Q_OBJECT

public:
    // This is needed because we inherit from QDialog non-public
    using QDialog::QObject;

    ConnectionManagerDialog(QWidget* parent);

    ConnectionManagerDialog(const ConnectionManagerDialog&) = delete;
    ConnectionManagerDialog(ConnectionManagerDialog&&) = delete;

    ConnectionManagerDialog& operator=(const ConnectionManagerDialog&) = delete;
    ConnectionManagerDialog& operator=(ConnectionManagerDialog&&) = delete;

    void popUp(const QStringList& available_connections, const QStringList& available_protocols);

private slots:
    void listSelectionChanged(void);

private:
    void updateConnectionSettingsEditor(const QString& selected_connection = "");

    QVBoxLayout* pMainLayout;
    QListWidget* pConnectionsAvailableList;
    QListWidget* pProtocolsAvailableList;
    QStackedLayout* pConnectionSettingsStackedLayout;
    QDialogButtonBox* pButtonBox;
    QList<I_ConnectionSettingsEditor*> connectionSettingsEditors;
};



#endif // CONNECTION_MANAGER_DIALOG_HPP
