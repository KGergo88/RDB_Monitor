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
#include <QLineEdit>

#include "i_connection_settings_editor.hpp"
#include "serial_port_settings_editor.hpp"
#include "connection_request_data.hpp"



#ifndef ADD_CONNECTION_DIALOG_HPP
#define ADD_CONNECTION_DIALOG_HPP



class EmptyConnectionSettingsEditor : public I_ConnectionSettingsEditor
{
    Q_OBJECT
    Q_INTERFACES(I_ConnectionSettingsEditor)

signals:
    void settingsChanged(bool settings_valid, const QString& error_message) override;

public:
    EmptyConnectionSettingsEditor(QWidget* parent) : I_ConnectionSettingsEditor(parent) { }

    EmptyConnectionSettingsEditor(const EmptyConnectionSettingsEditor&) = delete;
    EmptyConnectionSettingsEditor(EmptyConnectionSettingsEditor&&) = delete;

    virtual ~EmptyConnectionSettingsEditor() = default;

    EmptyConnectionSettingsEditor& operator=(const EmptyConnectionSettingsEditor&) = delete;
    EmptyConnectionSettingsEditor& operator=(EmptyConnectionSettingsEditor&&) = delete;

    virtual QString getConnectionName(void) override { return ""; }

    virtual std::shared_ptr<I_ConnectionSettings> getSettings(void) override
    {
        return nullptr;
    }

    virtual std::shared_ptr<I_ConnectionSettings> getDefaultSettings(void) override
    {
        return nullptr;
    }
};

class AddConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    AddConnectionDialog(QWidget* parent);

    AddConnectionDialog(const AddConnectionDialog&) = delete;
    AddConnectionDialog(AddConnectionDialog&&) = delete;

    AddConnectionDialog& operator=(const AddConnectionDialog&) = delete;
    AddConnectionDialog& operator=(AddConnectionDialog&&) = delete;

    void popUp(const QStringList& available_connections, const QStringList& available_protocols);
    ConnectionRequestData getConnectionRequestData(void);

private slots:
    void dialogContentChanged(bool connection_settings_valid = true, const QString& connection_settings_error_message = "");

private:
    void updateConnectionSettingsEditor(const QString& selected_connection = "");

    QVBoxLayout* pMainLayout;
    QListWidget* pConnectionsAvailableList;
    QListWidget* pProtocolsAvailableList;
    QLineEdit* pUserDefinedNameLineEdit;
    QStackedLayout* pConnectionSettingsStackedLayout;
    QLabel* pInstructionsLabel;
    QString instructionLabelDefaultText;
    QDialogButtonBox* pButtonBox;
    QDialogButtonBox::StandardButton buttonBoxOkType;
    QDialogButtonBox::StandardButton buttonBoxNokType;
    QList<I_ConnectionSettingsEditor*> connectionSettingsEditors;
};



#endif // ADD_CONNECTION_DIALOG_HPP
