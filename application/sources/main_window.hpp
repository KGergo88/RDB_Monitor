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



#include <vector>
#include <string>
#include <memory>

#include <QObject>
#include <QtWidgets>
#include <QtCharts>
#include <QString>
#include <QFileDialog>
#include <QTreeView>
#include <QDialog>

#include "global.hpp"
#include "i_gui_signal.hpp"
#include "i_backend_signal.hpp"
#include "diagram.hpp"
#include "add_connection_dialog.hpp"



#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP



class MainWindow : public QMainWindow, public I_GuiSignal
{
    Q_OBJECT
    Q_INTERFACES(I_GuiSignal)

public:
    MainWindow();

    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;

    ~MainWindow() override = default;

    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

    void RegisterBackendSignalInterface(I_BackendSignal* new_backend_signal_interface);

signals:
    void StartsToRun(void) override;
    void ShuttingDown(void) override;
    void OpenNetworkConnection(const ConnectionRequestData& request_data) override;
    void CloseNetworkConnection(const QString& user_defined_name) override;
    void RequestForDiagram(const QModelIndex& model_index) override;
    void ImportFile(const std::string& path_to_file) override;
    void ExportFileShowCheckBoxes(void) override;
    void ExportFileHideCheckBoxes(void) override;
    void ExportFileStoreCheckedDiagrams(const std::string& path_to_file) override;

private slots:
    void DisplayStatusMessage(const std::string& message_text);
    void AddConnectionDialogAccepted(void);
    void ConnectionManagerAddConnectionButtonWasClicked(void);
    void ConnectionManagerRemoveConnectionButtonWasClicked(void);
    void ListOfActiveConnectionsChanged(const QStringList& active_connections);
    void DiagramExportButtonExportWasClicked(void);
    void DiagramExportButtonCancelWasClicked(void);
    void DisplayDiagram(const DiagramSpecialized& diagram);
    void MenuActionDiagramsImportDiagrams(void);
    void MenuActionDiagramsExportDiagrams(void);
    void TreeviewCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

private:
    static constexpr int main_window_minimum_width = 800;
    static constexpr int main_window_minimum_height = 500;

    static constexpr int chart_view_size_percentage = 90;
    static constexpr int list_widget_status_size_percentage = 10;
    static constexpr int tree_view_size_percentage = 80;
    static constexpr int stacked_layout_size_percentage = 30;
    static constexpr int left_vertical_layout_size_percentage = 80;
    static constexpr int right_vertical_layout_size_percentage = 20;

    static constexpr char diagram_menu_text[] = "Import / Export Diagrams";
    static constexpr char diagram_menu_import_diagrams_text[] = "Import Diagrams";
    static constexpr char diagram_menu_export_diagrams_text[] = "Export Diagrams";

    static constexpr char connections_menu_text[] = "Connections";
    static constexpr char connections_menu_manage_connections_text[] = "Manage Connections";

    static constexpr char file_dialog_filter_string_constant_part[] = "Diagram Files: ";

    static constexpr qreal y_axis_range_multiplicator = 0.05;
    static constexpr int   y_axis_tick_count = 5;
    static constexpr int   y_axis_minor_tick_count = 0;

    std::string CreateFileDialogFilterString(void);

    class ConnectionManagerWidget : public QWidget
    {
    public:
        ConnectionManagerWidget(QWidget* parent = nullptr) : QWidget(parent)
        {
            auto vertical_layout = new QVBoxLayout;
            active_connections_list = new QListWidget();
            active_connections_list->setSelectionMode(QAbstractItemView::SingleSelection);
            vertical_layout->addWidget(active_connections_list);

            auto horizontal_layout = new QHBoxLayout;
            add_connection_button = new QPushButton("Add connection");
            remove_connection_button = new QPushButton("Remove connection");
            horizontal_layout->addWidget(add_connection_button);
            horizontal_layout->addWidget(remove_connection_button);
            vertical_layout->addLayout(horizontal_layout);

            auto groupbox = new QGroupBox("Connections", this);
            groupbox->setLayout(vertical_layout);
        }

        ConnectionManagerWidget(const ConnectionManagerWidget&) = delete;
        ConnectionManagerWidget(ConnectionManagerWidget&&) = delete;

        virtual ~ConnectionManagerWidget() = default;

        ConnectionManagerWidget& operator=(const ConnectionManagerWidget&) = delete;
        ConnectionManagerWidget& operator=(ConnectionManagerWidget&&) = delete;

        QListWidget* active_connections_list;
        QPushButton* add_connection_button;
        QPushButton* remove_connection_button;
    };

    class DiagramExportWidget : public QWidget
    {
    public:
        DiagramExportWidget(QWidget* parent = nullptr) : QWidget(parent)
        {
            layout = new QVBoxLayout(this);
            button_export = new QPushButton(button_export_text, this);
            button_cancel = new QPushButton(button_cancel_text, this);
            layout->addWidget(button_export);
            layout->addWidget(button_cancel);
        }

        DiagramExportWidget(const DiagramExportWidget&) = delete;
        DiagramExportWidget(DiagramExportWidget&&) = delete;

        virtual ~DiagramExportWidget() = default;

        DiagramExportWidget& operator=(const DiagramExportWidget&) = delete;
        DiagramExportWidget& operator=(DiagramExportWidget&&) = delete;

        static constexpr char button_export_text[] = "Export";
        static constexpr char button_cancel_text[] = "Cancel";

        QVBoxLayout* layout;
        QPushButton* button_export;
        QPushButton* button_cancel;
    };

    bool network_connection_is_open;

    I_BackendSignal* backend_signal_interface;

    QMenu*                   pDiagramsMenu;
    QMenu*                   pConnectionsMenu;
    QChartView*              pChartView;
    QTreeView*               pTreeView;
    QListWidget*             pListWidgetStatus;
    ConnectionManagerWidget* pWidgetConnectionManager;
    DiagramExportWidget*     pWidgetDiagramExport;
    QStackedLayout*          pStackedLayout;
    AddConnectionDialog*     pAddConnectionDialog;
};



#endif /* MAIN_WINDOW_HPP */
