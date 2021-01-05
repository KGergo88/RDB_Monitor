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



#include "connection_manager_dialog.hpp"



ConnectionManagerDialog::ConnectionManagerDialog(QWidget* parent) : QDialog(parent)
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

    // Connection settings
    connectionSettingsEditors = {new EmptyConnectionSettingsEditor(this), new SerialConnectionSettingsEditor(this)};
    auto pConnectionSettingsGroupBox = new QGroupBox("Connection settings");
    pConnectionSettingsStackedLayout = new QStackedLayout;
    pConnectionSettingsGroupBox->setLayout(pConnectionSettingsStackedLayout);
    for(auto& i : connectionSettingsEditors)
    {
        pConnectionSettingsStackedLayout->addWidget(i);
    }
    pConnectionSettingsStackedLayout->setCurrentWidget(connectionSettingsEditors[0]);

    // Buttonbox to accept or decline the changes
    pButtonBox = new QDialogButtonBox(QDialogButtonBox::Apply | QDialogButtonBox::Cancel);

    // Main layout for the dialog
    pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pConnectionsAndProtocolsLayout);
    pMainLayout->addWidget(pConnectionSettingsGroupBox);
    pMainLayout->addWidget(pButtonBox);
    setLayout(pMainLayout);
    setWindowTitle("Add connections");

    // Registering the connections between the signals and the slots
    connect(pConnectionsAvailableList, &QListWidget::itemSelectionChanged,
            this, &ConnectionManagerDialog::listSelectionChanged);
    connect(pProtocolsAvailableList, &QListWidget::itemSelectionChanged,
            this, &ConnectionManagerDialog::listSelectionChanged);
    connect(pButtonBox, &QDialogButtonBox::accepted,
            this, &QDialog::accept);
    connect(pButtonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);
}

void ConnectionManagerDialog::popUp(const QStringList& available_connections, const QStringList& available_protocols)
{
    pConnectionsAvailableList->clear();
    pConnectionsAvailableList->clearSelection();
    pConnectionsAvailableList->addItems(available_connections);

    pProtocolsAvailableList->clear();
    pConnectionsAvailableList->clearSelection();
    pProtocolsAvailableList->addItems(available_protocols);

    updateConnectionSettingsEditor(nullptr);

    pButtonBox->button(QDialogButtonBox::Apply)->setEnabled(false);

    open();
}

void ConnectionManagerDialog::listSelectionChanged(void)
{
    auto selected_connections = pConnectionsAvailableList->selectedItems();
    auto selected_protocols = pProtocolsAvailableList->selectedItems();

    if(1 == selected_connections.size())
    {
        auto selected_connection = pConnectionsAvailableList->selectedItems()[0]->text();
        updateConnectionSettingsEditor(selected_connection);

        if(1 == selected_protocols.size())
        {
            pButtonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
        }
    }
    else
    {
        updateConnectionSettingsEditor();
        pButtonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
    }
}

void ConnectionManagerDialog::updateConnectionSettingsEditor(const QString& selected_connection)
{
    I_ConnectionSettingsEditor* selected_editor = connectionSettingsEditors[0];

    for(auto& i : connectionSettingsEditors)
    {
        auto setting_editor_name = i->getName();
        if(selected_connection == setting_editor_name)
        {
            selected_editor = i;
            break;
        }
    }

    pConnectionSettingsStackedLayout->setCurrentWidget(selected_editor);
}
