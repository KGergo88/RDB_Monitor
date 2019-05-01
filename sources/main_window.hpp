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

#include "global.hpp"
#include "gui_signal_interface.h"
#include "backend_signal_interface.hpp"
#include "diagram.hpp"
#include "network_handler.hpp"



#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP



class MainWindow : public QMainWindow, public GuiSignalInterface
{
    Q_OBJECT
    Q_INTERFACES(GuiSignalInterface)

public:
    MainWindow();

    MainWindow(const MainWindow& newGuiWindow) = delete;
    MainWindow(MainWindow&& newGuiWindow) = delete;

    ~MainWindow() override = default;

    MainWindow& operator=(const MainWindow&  newGuiWindow) = delete;
    MainWindow& operator=(MainWindow&& newGuiWindow) = delete;

    void RegisterBackendSignalInterface(BackendSignalInterface* new_backend_signal_interface);

signals:
    void StartsToRun(void) override;
    void ShuttingDown(void) override;
    void OpenNetworkConnection(const std::string& port_name) override;
    void CloseNetworkConnection(const std::string& port_name) override;
    void RequestForDiagram(const QModelIndex& model_index) override;
    void ImportFile(const std::string& path_to_file) override;

private slots:
    void DisplayStatusMessage(const std::string& message_text);
    void PushButtonWasClicked(void);
    void ProcessNetworkOperationResult(const std::string& port_name, const bool& result);
    void DisplayDiagram(const DiagramSpecialized& diagram);
    void MenuActionDiagramsImportDiagrams(void);
    void MenuActionDiagramsExportDiagrams(void);
    void TreeviewCurrentSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    static constexpr int main_window_minimum_width = 800;
    static constexpr int main_window_minimum_height = 500;

    static constexpr int chart_view_size_percentage = 90;
    static constexpr int list_widget_status_size_percentage = 10;
    static constexpr int tree_view_size_percentage = 90;
    static constexpr int line_edit_size_percentage = 5;
    static constexpr int push_button_size_percentage = 5;
    static constexpr int left_vertical_layout_size_percentage = 80;
    static constexpr int right_vertical_layout_size_percentage = 20;


    static constexpr char push_button_open_text[] = "Open Serial Port";
    static constexpr char push_button_close_text[] = "Close Serial Port";
    static constexpr char diagram_menu_text[] = "Diagrams";
    static constexpr char diagram_menu_import_diagrams_text[] = "Import Diagrams";
    static constexpr char diagram_menu_export_diagrams_text[] = "Export Diagrams";

    static constexpr qreal y_axis_range_multiplicator = 0.05;
    static constexpr int   y_axis_tick_count = 5;
    static constexpr int   y_axis_minor_tick_count = 0;

    bool network_connection_is_open;

    BackendSignalInterface* backend_signal_interface;

    QChartView*     pChartView;
    QTreeView*      pTreeView;
    QListWidget*    pListWidgetStatus;
    QLineEdit*      pLineEdit;
    QPushButton*    pPushButton;
    QMenuBar*       pMenuBar;
    QMenu*          pDiagramsMenu;
    QMenu*          pHelpMenu;
};



#endif /* MAIN_WINDOW_HPP */
