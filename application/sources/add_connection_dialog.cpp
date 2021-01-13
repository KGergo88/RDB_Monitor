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



#include "add_connection_dialog.hpp"



AddConnectionDialog::AddConnectionDialog(QWidget* parent) : QDialog(parent)
{
    // Available connections
    auto pConnectionsLayout = new QVBoxLayout;
    auto pConnectionsGroupBox = new QGroupBox("Available connections");
    pConnectionsAvailableList = new QListWidget;
    pConnectionsAvailableList->setSelectionMode(QAbstractItemView::SingleSelection);
    pConnectionsLayout->addWidget(pConnectionsAvailableList);
    pConnectionsGroupBox->setLayout(pConnectionsLayout);

    // Available protocols
    auto pProtocolsLayout = new QVBoxLayout;
    auto pProtocolsGroupBox = new QGroupBox("Available protocols");
    pProtocolsAvailableList = new QListWidget;
    pProtocolsAvailableList->setSelectionMode(QAbstractItemView::SingleSelection);
    pProtocolsLayout->addWidget(pProtocolsAvailableList);
    pProtocolsGroupBox->setLayout(pProtocolsLayout);

    // Layout to contain the connections and protocols
    auto pConnectionsAndProtocolsLayout = new QHBoxLayout;
    pConnectionsAndProtocolsLayout->addWidget(pConnectionsGroupBox);
    pConnectionsAndProtocolsLayout->addWidget(pProtocolsGroupBox);

    // User defined name
    auto pUserDefinedNameGridLayout = new QGridLayout;
    auto pUserDefinedNameLabel = new QLabel("Connection name:");
    pUserDefinedNameLineEdit = new QLineEdit();
    pUserDefinedNameGridLayout->addWidget(pUserDefinedNameLabel);
    pUserDefinedNameGridLayout->addWidget(pUserDefinedNameLineEdit);

    // Connection settings
    connectionSettingsEditors = {new EmptyConnectionSettingsEditor(this), new SerialPortSettingsEditor(this)};
    auto pConnectionSettingsGroupBox = new QGroupBox("Connection settings");
    pConnectionSettingsStackedLayout = new QStackedLayout;
    pConnectionSettingsGroupBox->setLayout(pConnectionSettingsStackedLayout);
    for(auto& i : connectionSettingsEditors)
    {
        pConnectionSettingsStackedLayout->addWidget(i);
    }
    pConnectionSettingsStackedLayout->setCurrentWidget(connectionSettingsEditors[0]);

    // Instructions label
    pInstructionsLabel = new QLabel;
    instructionLabelDefaultText = "Select a connection and a protocol, choose a name and edit the connection settings!";
    pInstructionsLabel->setText(instructionLabelDefaultText);

    // Buttonbox to accept or decline the changes
    buttonBoxOkType = QDialogButtonBox::Ok;
    buttonBoxNokType = QDialogButtonBox::Cancel;
    pButtonBox = new QDialogButtonBox(buttonBoxOkType | buttonBoxNokType);

    // Main layout for the dialog
    pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pConnectionsAndProtocolsLayout);
    pMainLayout->addLayout(pUserDefinedNameGridLayout);
    pMainLayout->addWidget(pConnectionSettingsGroupBox);
    pMainLayout->addWidget(pInstructionsLabel);
    pMainLayout->addWidget(pButtonBox);
    setLayout(pMainLayout);
    setWindowTitle("Add connections");

    // Registering the connections between the signals and the slots
    QObject::connect(pConnectionsAvailableList,                         &QListWidget::itemSelectionChanged,
                     [this](void){ dialogContentChanged(); });
    QObject::connect(pProtocolsAvailableList,                           &QListWidget::itemSelectionChanged,
                     [this](void){ dialogContentChanged(); });
    QObject::connect(pUserDefinedNameLineEdit,                          &QLineEdit::textChanged,
                     [this](void){ dialogContentChanged(); });
    QObject::connect(pConnectionSettingsStackedLayout->currentWidget(), SIGNAL(settingsChanged(bool, const QString&)),
                     this,                                              SLOT(dialogContentChanged(bool, const QString&)));
    QObject::connect(pButtonBox,                                        &QDialogButtonBox::accepted,
                     this,                                              &QDialog::accept);
    QObject::connect(pButtonBox,                                        &QDialogButtonBox::rejected,
                     this,                                              &QDialog::reject);
}

void AddConnectionDialog::popUp(const QStringList& available_connections, const QStringList& available_protocols)
{
    pConnectionsAvailableList->clear();
    pConnectionsAvailableList->clearSelection();
    pConnectionsAvailableList->addItems(available_connections);

    pProtocolsAvailableList->clear();
    pConnectionsAvailableList->clearSelection();
    pProtocolsAvailableList->addItems(available_protocols);

    pUserDefinedNameLineEdit->setText("");

    updateConnectionSettingsEditor(nullptr);

    pButtonBox->button(buttonBoxOkType)->setEnabled(false);

    open();
}

ConnectionRequestData AddConnectionDialog::getConnectionRequestData(void)
{
    ConnectionRequestData request_data;

    request_data.user_defined_name = pUserDefinedNameLineEdit->text();
    request_data.connection = pConnectionsAvailableList->selectedItems()[0]->text();
    request_data.protocol = pProtocolsAvailableList->selectedItems()[0]->text();
    auto editor_index = pConnectionSettingsStackedLayout->currentIndex();
    request_data.connection_settings = connectionSettingsEditors[editor_index]->getSettings();

    return request_data;
}

void AddConnectionDialog::dialogContentChanged(bool connection_settings_valid, const QString& connection_settings_error_message)
{
    bool enable_ok_type_button = false;
    QString instruction_label_text = instructionLabelDefaultText;

    if(connection_settings_valid)
    {
        auto selected_connections = pConnectionsAvailableList->selectedItems();
        auto selected_protocols = pProtocolsAvailableList->selectedItems();
        auto user_defined_name = pUserDefinedNameLineEdit->text();

        if(1 == selected_connections.size())
        {
            auto selected_connection = selected_connections[0]->text();
            updateConnectionSettingsEditor(selected_connection);

            if(1 == selected_protocols.size())
            {
                if(!user_defined_name.isEmpty())
                {
                    instruction_label_text = "";
                    enable_ok_type_button = true;
                }
            }
            else
            {
                enable_ok_type_button = false;
            }
        }
        else
        {
            updateConnectionSettingsEditor();
        }
    }
    else
    {
        instruction_label_text = "Invalid connection settings: " + connection_settings_error_message;
    }

    pInstructionsLabel->setText(instruction_label_text);

    if(enable_ok_type_button)
    {
        pButtonBox->button(buttonBoxOkType)->setEnabled(true);
    }
    else
    {
        pButtonBox->button(buttonBoxOkType)->setEnabled(false);
    }
}

void AddConnectionDialog::updateConnectionSettingsEditor(const QString& selected_connection)
{
    I_ConnectionSettingsEditor* selected_editor = connectionSettingsEditors[0];

    for(auto& i : connectionSettingsEditors)
    {
        auto setting_editor_name = i->getConnectionName();
        if(selected_connection == setting_editor_name)
        {
            selected_editor = i;
            break;
        }
    }

    QObject::disconnect(pConnectionSettingsStackedLayout->currentWidget(), SIGNAL(settingsChanged(bool, const QString&)),
                        this,                                              SLOT(dialogContentChanged(bool, const QString&)));

    pConnectionSettingsStackedLayout->setCurrentWidget(selected_editor);

    QObject::connect(pConnectionSettingsStackedLayout->currentWidget(), SIGNAL(settingsChanged(bool, const QString&)),
                     this,                                              SLOT(dialogContentChanged(bool, const QString&)));
}
