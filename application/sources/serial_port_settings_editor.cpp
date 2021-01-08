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



#include "serial_port_settings_editor.hpp"



SerialPortSettingsEditor::SerialPortSettingsEditor(QWidget* parent) : I_ConnectionSettingsEditor(parent)
{
    SerialPortSettings default_settings;

    // Layout for the editor
    pGridLayout = new QGridLayout(this);

    // Port name
    auto pPortNameLabel = new QLabel("Port name");
    pPortNameLineEdit = new QLineEdit(default_settings.portName);
    pGridLayout->addWidget(pPortNameLabel, 0, 0);
    pGridLayout->addWidget(pPortNameLineEdit, 0, 1);
    QObject::connect(pPortNameLineEdit, &QLineEdit::textChanged, this, &SerialPortSettingsEditor::reviewSettings);

    // Baud rate
    auto pBaudRateLabel = new QLabel("Baud rate");
    pBaudRateComboBox = new QComboBox;
    pBaudRateComboBox->addItem("1200 bps", QSerialPort::BaudRate::Baud1200);
    pBaudRateComboBox->addItem("2400 bps", QSerialPort::BaudRate::Baud2400);
    pBaudRateComboBox->addItem("4800 bps", QSerialPort::BaudRate::Baud4800);
    pBaudRateComboBox->addItem("9600 bps", QSerialPort::BaudRate::Baud9600);
    pBaudRateComboBox->addItem("19200 bps", QSerialPort::BaudRate::Baud19200);
    pBaudRateComboBox->addItem("38400 bps", QSerialPort::BaudRate::Baud38400);
    pBaudRateComboBox->addItem("57600 bps", QSerialPort::BaudRate::Baud57600);
    pBaudRateComboBox->addItem("115200 bps", QSerialPort::BaudRate::Baud115200);
    pGridLayout->addWidget(pBaudRateLabel, 1, 0);
    pGridLayout->addWidget(pBaudRateComboBox, 1, 1);
    QObject::connect(pBaudRateComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int){ reviewSettings(); });

    // Data bits
    auto pDataBitsLabel = new QLabel("Data bits");
    pDataBitsComboBox = new QComboBox;
    pDataBitsComboBox->addItem("5 bit", QSerialPort::DataBits::Data5);
    pDataBitsComboBox->addItem("6 bit", QSerialPort::DataBits::Data6);
    pDataBitsComboBox->addItem("7 bit", QSerialPort::DataBits::Data7);
    pDataBitsComboBox->addItem("8 bit", QSerialPort::DataBits::Data8);
    pGridLayout->addWidget(pDataBitsLabel, 2, 0);
    pGridLayout->addWidget(pDataBitsComboBox, 2, 1);
    QObject::connect(pDataBitsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int){ reviewSettings(); });

    // Stop bits
    auto pStopBitsLabel = new QLabel("Stop bits");
    pStopBitsComboBox = new QComboBox;
    pStopBitsComboBox->addItem("One bit", QSerialPort::StopBits::OneStop);
    pStopBitsComboBox->addItem("One and a half bits", QSerialPort::StopBits::OneAndHalfStop);
    pStopBitsComboBox->addItem("Two bits", QSerialPort::StopBits::TwoStop);
    pGridLayout->addWidget(pStopBitsLabel, 3, 0);
    pGridLayout->addWidget(pStopBitsComboBox, 3, 1);
    QObject::connect(pStopBitsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int){ reviewSettings(); });

    // Parity
    auto pParityLabel = new QLabel("Parity");
    pParityComboBox = new QComboBox;
    pParityComboBox->addItem("None", QSerialPort::Parity::NoParity);
    pParityComboBox->addItem("Even", QSerialPort::Parity::EvenParity);
    pParityComboBox->addItem("Odd", QSerialPort::Parity::OddParity);
    pParityComboBox->addItem("Space", QSerialPort::Parity::SpaceParity);
    pParityComboBox->addItem("Mark", QSerialPort::Parity::MarkParity);
    pGridLayout->addWidget(pParityLabel, 4, 0);
    pGridLayout->addWidget(pParityComboBox, 4, 1);
    QObject::connect(pParityComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int){ reviewSettings(); });

    // Flow control
    auto pFlowControlLabel = new QLabel("Flow control");
    pFlowControlComboBox = new QComboBox;
    pFlowControlComboBox->addItem("None", QSerialPort::FlowControl::NoFlowControl);
    pFlowControlComboBox->addItem("Hardware", QSerialPort::FlowControl::HardwareControl);
    pGridLayout->addWidget(pFlowControlLabel, 5, 0);
    pGridLayout->addWidget(pFlowControlComboBox, 5, 1);
    QObject::connect(pFlowControlComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int){ reviewSettings(); });
}

std::shared_ptr<I_ConnectionSettings> SerialPortSettingsEditor::getSettings(void)
{
    auto settings = std::make_shared<SerialPortSettings>();
    settings->portName = pPortNameLineEdit->text();
    settings->baudRate = pBaudRateComboBox->currentData().value<QSerialPort::BaudRate>();
    settings->dataBits = pBaudRateComboBox->currentData().value<QSerialPort::DataBits>();
    settings->stopBits = pBaudRateComboBox->currentData().value<QSerialPort::StopBits>();
    settings->parity = pBaudRateComboBox->currentData().value<QSerialPort::Parity>();
    settings->flowControl = pBaudRateComboBox->currentData().value<QSerialPort::FlowControl>();
    return settings;
}

std::shared_ptr<I_ConnectionSettings> SerialPortSettingsEditor::getDefaultSettings(void)
{
    return std::make_shared<SerialPortSettings>();
}

void SerialPortSettingsEditor::reviewSettings(void)
{
    bool settings_valid = false;
    QString error_message;

    if(!pPortNameLineEdit->text().isEmpty())
    {
        settings_valid = true;
    }
    else
    {
        error_message = "The port name cannot be empty!";
    }

    emit settingsChanged(settings_valid, error_message);
}
