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


#ifndef BACKEND_HPP
#define BACKEND_HPP


#include <iostream>
#include <functional>
#include <cstdio>
#include <ctime>
#include <fstream>

#include <QApplication>
#include <QFileInfo>

#include "i_backend_signal.hpp"
#include "diagram_container.hpp"
#include "configuration.hpp"


class I_GuiSignal;
class I_Protocol;
class ConnectionRequestData;
class NetworkHandler;

class Backend : public QObject, public I_BackendSignal
{
    Q_OBJECT
    Q_INTERFACES(I_BackendSignal)

public:
    Backend();
    Backend(const Backend& new_backend) = delete;
    Backend(Backend&& new_backend) = delete;

    virtual ~Backend() override = default;

    Backend& operator=(const Backend&  new_backend) = delete;
    Backend& operator=(Backend&& new_backend) = delete;

    void RegisterGuiSignalInterface(I_GuiSignal* new_gui_signal_interface);

    void ReportStatus(const std::string& message);

    void StoreNetworkDiagrams(const QString& connection_name, std::vector<DiagramSpecialized>& new_diagrams);
    void StoreFileDiagrams(const std::string& file_name, const std::string& file_path, std::vector<DiagramSpecialized>& new_diagrams);

    QAbstractItemModel* GetDiagramContainerModel(void) override {return &diagram_container;}
    std::string GetFileImportDefaultFolder(void) override {return configuration.ImportFolder();}
    std::string GetFileExportDefaultFolder(void) override {return configuration.ExportFolder();}
    std::vector<std::string> GetSupportedFileExtensions(void) override;
    QStringList GetAvailableConnections(void) override;
    QStringList GetAvailableProtocols(void) override;

signals:
    void NewStatusMessage(const std::string& message_text) override;
    void ListOfActiveConnectionsChanged(const QStringList& active_connections) override;
    void ShowThisDiagram(const DiagramSpecialized& diagram) override;

private slots:
    void OpenNetworkConnection(const ConnectionRequestData& request_data);
    void CloseNetworkConnection(const QString& user_defined_name);
    void RequestForDiagram(const QModelIndex& model_index);
    void ImportFile(const std::string& path_to_file);
    void ExportFileShowCheckBoxes(void);
    void ExportFileHideCheckBoxes(void);
    void ExportFileStoreCheckedDiagrams(const std::string& path_to_file);

private:
    void StoreDiagrams(std::vector<DiagramSpecialized>& new_diagrams, const std::function<QModelIndex(const DiagramSpecialized&)> storage_logic);
    QStringList getActiveConnections(void);
    QString makeUserDefinedConnectionNameUnique(const QString& user_defined_name);

    QStringList available_connection_handlers;
    QStringList available_protocol_handlers;
    std::vector<std::shared_ptr<I_Protocol> > file_handlers;
    std::map<QString, std::shared_ptr<NetworkHandler> > network_handlers;

    I_GuiSignal *gui_signal_interface;

    DiagramContainer diagram_container;
    Configuration configuration;
};


#endif /* BACKEND_HPP */
