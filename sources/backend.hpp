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



#include <iostream>
#include <functional>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <filesystem>

#include <QApplication>

#include "global.hpp"
#include "backend_signal_interface.hpp"
#include "gui_signal_interface.hpp"
#include "diagram.hpp"
#include "serial_port.hpp"
#include "measurement_data_protocol.hpp"
#include "network_handler.hpp"
#include "diagram_container.hpp"
#include "configuration.hpp"



#ifndef BACKEND_HPP
#define BACKEND_HPP



class Backend : public QObject, public BackendSignalInterface
{
    Q_OBJECT
    Q_INTERFACES(BackendSignalInterface)

public:
    Backend();
    Backend(const Backend& new_backend) = delete;
    Backend(Backend&& new_backend) = delete;

    virtual ~Backend() override = default;

    Backend& operator=(const Backend&  new_backend) = delete;
    Backend& operator=(Backend&& new_backend) = delete;

    void RegisterGuiSignalInterface(GuiSignalInterface* new_gui_signal_interface);

    void ReportStatus(const std::string& message);

    void StoreNetworkDiagrams(const std::string& connection_name, std::vector<DiagramSpecialized>& new_diagrams);
    void StoreFileDiagrams(const std::string& file_name, const std::string& file_path, std::vector<DiagramSpecialized>& new_diagrams);

    QAbstractItemModel* GetDiagramContainerModel(void) override {return &diagram_container;}
    std::string GetFileImportDefaultFolder(void) override {return configuration.ImportFolder();}
    std::string GetFileExportDefaultFolder(void) override {return configuration.ExportFolder();}
    std::vector<std::string> GetSupportedFileExtensions(void) override;

signals:
    void NewStatusMessage(const std::string& message_text) override;
    void NetworkOperationFinished(const std::string& port_name, bool result) override;
    void ShowThisDiagram(const DiagramSpecialized& diagram) override;

private slots:
    void OpenNetwokConnection(const std::string&);
    void CloseNetworkConnection(const std::string&);
    void RequestForDiagram(const QModelIndex& model_index);
    void ImportFile(const std::string& path_to_file);
    void ExportFileShowCheckBoxes(void);
    void ExportFileHideCheckBoxes(void);
    void ExportFileStoreCheckedDiagrams(const std::string& path_to_file);

private:
    void StoreDiagrams(std::vector<DiagramSpecialized>& new_diagrams, const std::function<QModelIndex(const DiagramSpecialized&)> storage_logic);

    SerialPort serial_port;
    MeasurementDataProtocol measurement_data_protocol;
    NetworkHandler serial_network_handler;

    GuiSignalInterface *gui_signal_interface;

    DiagramContainer diagram_container;
    Configuration configuration;
};



#endif /* BACKEND_HPP */
