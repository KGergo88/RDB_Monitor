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


#include "main_window.hpp"
#include "i_backend_signal.hpp"
#include "add_connection_dialog.hpp"
#include "connection_request_data.hpp"


extern const QString application_name;
extern const QString application_version;

MainWindow::MainWindow() : QMainWindow(),
                           backend_signal_interface(nullptr)
{
    network_connection_is_open = false;

    // AddConnectionDialog
    pAddConnectionDialog = new AddConnectionDialog(this);

    // QChartView to display the charts containing the diagrams
    pChartView = new QChartView();
    pChartView->setRenderHint(QPainter::Antialiasing);
    pChartView->setRubberBand(QChartView::NoRubberBand);

    // QTreeView to list the available diagrams
    pTreeView = new QTreeView();
    pTreeView->setAnimated(true);

    // QListWidget to display the status messages
    pListWidgetStatus = new QListWidget();

    // QVBoxLayout to order the QChartview and QTreeView
    QVBoxLayout *pLeftVerticalLayout = new QVBoxLayout();
    pLeftVerticalLayout->addWidget(pChartView, chart_view_size_percentage);
    pLeftVerticalLayout->addWidget(pListWidgetStatus, list_widget_status_size_percentage);

    pWidgetConnectionManager = new ConnectionManagerWidget();
    pWidgetDiagramExport = new DiagramExportWidget();

    // QStackedLayout to order the ConnectionManagerWidget and the DiagramExportWidget
    pStackedLayout = new QStackedLayout();
    pStackedLayout->addWidget(pWidgetConnectionManager);
    pStackedLayout->addWidget(pWidgetDiagramExport);
    pStackedLayout->setCurrentWidget(pWidgetConnectionManager);

    // QVBoxLayout to order the QTreeView and QStackedLayout
    QVBoxLayout *pRightVerticalLayout = new QVBoxLayout();
    pRightVerticalLayout->addWidget(pTreeView, tree_view_size_percentage);
    pRightVerticalLayout->addLayout(pStackedLayout, stacked_layout_size_percentage);

    // QHBoxLayout to order the two QVBoxLayouts
    QHBoxLayout *pHorizontalLayout = new QHBoxLayout();
    pHorizontalLayout->addLayout(pLeftVerticalLayout, left_vertical_layout_size_percentage);
    pHorizontalLayout->addLayout(pRightVerticalLayout, right_vertical_layout_size_percentage);

    // QWidget to contain the QHBoxLayout
    QWidget *pCentralWidget = new QWidget(this);
    pCentralWidget->setLayout(pHorizontalLayout);
    setCentralWidget(pCentralWidget);

    pDiagramsMenu = menuBar()->addMenu(diagram_menu_text);
    pDiagramsMenu->addAction(diagram_menu_import_diagrams_text, this, &MainWindow::MenuActionDiagramsImportDiagrams);
    pDiagramsMenu->addAction(diagram_menu_export_diagrams_text, this, &MainWindow::MenuActionDiagramsExportDiagrams);

    // Registering the connections between the signals and the slots
    QObject::connect(pWidgetConnectionManager->add_connection_button,    &QPushButton::clicked,
                     this,                                               &MainWindow::ConnectionManagerAddConnectionButtonWasClicked);
    QObject::connect(pWidgetConnectionManager->remove_connection_button, &QPushButton::clicked,
                     this,                                               &MainWindow::ConnectionManagerRemoveConnectionButtonWasClicked);
    QObject::connect(pAddConnectionDialog,                               &QDialog::accepted,
                     this,                                               &MainWindow::AddConnectionDialogAccepted);
    QObject::connect(pWidgetDiagramExport->button_export,                &QPushButton::clicked,
                     this,                                               &MainWindow::DiagramExportButtonExportWasClicked);
    QObject::connect(pWidgetDiagramExport->button_cancel,                &QPushButton::clicked,
                     this,                                               &MainWindow::DiagramExportButtonCancelWasClicked);

    // Setting the minimum size, and the title of the window
    setMinimumSize(main_window_minimum_width, main_window_minimum_height);
    setWindowTitle(QString(application_name) + "" + QString(application_version));

    // Setting the window icon
    QString window_icon_image_path = ":/images/Icon.png";
    if(QFileInfo::exists(window_icon_image_path))
    {
        QIcon window_icon(window_icon_image_path);
        setWindowIcon(window_icon);
    }
    else
    {
        throw("The window icon can not be found! Check path: " + window_icon_image_path.toStdString());
    }

    // The main window will be opened as maximized
    showMaximized();
}

void MainWindow::MenuActionDiagramsImportDiagrams(void)
{
    auto default_folder = backend_signal_interface->GetFileImportDefaultFolder();
    auto filter_string = CreateFileDialogFilterString();
    QFileDialog *pImportFileSelectorDialog = new QFileDialog(this, diagram_menu_export_diagrams_text, QString::fromStdString(default_folder), QString::fromStdString(filter_string));
    pImportFileSelectorDialog->setAcceptMode(QFileDialog::AcceptOpen);
    pImportFileSelectorDialog->setModal(true);
    pImportFileSelectorDialog->show();

    QObject::connect(pImportFileSelectorDialog, &QFileDialog::fileSelected, [=](const QString &file){emit ImportFile(file.toStdString());});
}

void MainWindow::MenuActionDiagramsExportDiagrams(void)
{
    pDiagramsMenu->setEnabled(false);
    pStackedLayout->setCurrentWidget(pWidgetDiagramExport);
    emit ExportFileShowCheckBoxes();
}

void MainWindow::TreeviewCurrentSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    (void) previous;

    emit RequestForDiagram(current);
}

void MainWindow::RegisterBackendSignalInterface(I_BackendSignal* new_backend_signal_interface)
{
    // If the backend signal interface is valid
    if(new_backend_signal_interface)
    {
        // Registering the interface
        backend_signal_interface = new_backend_signal_interface;

        // Registering the connections between the signals and the slots
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface), SIGNAL(NewStatusMessage(const std::string&)),
                         this,                                             SLOT(DisplayStatusMessage(const std::string&)));
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface), SIGNAL(ListOfActiveConnectionsChanged(const QStringList&)),
                         this,                                             SLOT(ListOfActiveConnectionsChanged(const QStringList&)));
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface), SIGNAL(ShowThisDiagram(const DefaultDiagram&)),
                         this,                                             SLOT(DisplayDiagram(const DefaultDiagram&)));

        // Setting up the TreeView with the Model from the interface (this needs to be done before establishing the signal connections for the pTreeView)
        pTreeView->setModel(backend_signal_interface->GetDiagramContainerModel());
        QObject::connect(pTreeView->selectionModel(),                      &QItemSelectionModel::currentChanged,
                         this,                                             &MainWindow::TreeviewCurrentSelectionChanged);
    }
    else
    {
        std::string errorMessage = "There was no backend_signal_interface set in MainWindow::RegisterBackendSignalInterface!";
        throw errorMessage;
    }
}

void MainWindow::DisplayStatusMessage(const std::string& message_text)
{
    auto pListWidgetItem = new QListWidgetItem();
    pListWidgetItem->setText(QString::fromStdString(message_text));
    pListWidgetStatus->insertItem(0, pListWidgetItem);
}

void MainWindow::AddConnectionDialogAccepted(void)
{
    ConnectionRequestData request_data = pAddConnectionDialog->getConnectionRequestData();
    emit OpenNetworkConnection(request_data);
}

void MainWindow::ConnectionManagerAddConnectionButtonWasClicked(void)
{
    // Get the list of available connections and protocols from the Backend
    auto connections = backend_signal_interface->GetAvailableConnections();
    auto protocols = backend_signal_interface->GetAvailableProtocols();

    // Displaying the connection manager dialog
    pAddConnectionDialog->popUp(connections, protocols);
}

void MainWindow::ConnectionManagerRemoveConnectionButtonWasClicked(void)
{
    auto selected_connections = pWidgetConnectionManager->active_connections_list->selectedItems();
    for(const auto& i: selected_connections)
    {
        auto connection_name = i->text();
        emit CloseNetworkConnection(connection_name);
    }
}

void MainWindow::ListOfActiveConnectionsChanged(const QStringList& active_connections)
{
    pWidgetConnectionManager->active_connections_list->clear();
    for(const auto& i : active_connections)
    {
        pWidgetConnectionManager->active_connections_list->addItem(i);
    }
}

void MainWindow::DiagramExportButtonExportWasClicked(void)
{
    auto default_folder = backend_signal_interface->GetFileExportDefaultFolder();
    auto filter_string = CreateFileDialogFilterString();

    QFileDialog *pExportFileSelectorDialog = new QFileDialog(this, diagram_menu_export_diagrams_text, QString::fromStdString(default_folder), QString::fromStdString(filter_string));
    pExportFileSelectorDialog->setAcceptMode(QFileDialog::AcceptSave);
    pExportFileSelectorDialog->setModal(true);
    pExportFileSelectorDialog->show();

    QObject::connect(pExportFileSelectorDialog, &QFileDialog::fileSelected,
                     [=](const QString &file)
                        {
                            pDiagramsMenu->setEnabled(true);
                            pStackedLayout->setCurrentWidget(pWidgetConnectionManager);
                            emit ExportFileStoreCheckedDiagrams(file.toStdString());
                            emit ExportFileHideCheckBoxes();
                        });
}

void MainWindow::DiagramExportButtonCancelWasClicked(void)
{
    pDiagramsMenu->setEnabled(true);
    pStackedLayout->setCurrentWidget(pWidgetConnectionManager);
    emit ExportFileHideCheckBoxes();
}

void MainWindow::DisplayDiagram(const DefaultDiagram& diagram)
{
   // Creating a new chart that will be displayed in the chartview after loading it with data
    auto pNewChart = new QChart();

    // Setting the title with the Diagram name
    pNewChart->setTitle(QString::fromStdString(diagram.GetTitle()));
    // Hiding the legend because the data lines will be recognisable from their Y axis
    pNewChart->legend()->hide();
    // Creating the X axis, giving it a title and addig it to the chart. The ranges will only be set after analyzing the data points.
    auto pXAxis = new QValueAxis;
    pXAxis->setTitleText(QString::fromStdString(diagram.GetAxisXTitle()));
    pNewChart->addAxis(pXAxis, Qt::AlignBottom);

    // We will add every DataLine of the Diagram to the chart
    DefaultDiagram::index_t number_of_data_lines = diagram.GetTheNumberOfDataLines();
    // Variables to store the min/max values of the diagram
    auto diagram_extreme_values = diagram.GetExtremeValues();

    for(DefaultDiagram::index_t data_line_counter = 0; data_line_counter < number_of_data_lines; ++data_line_counter)
    {
        // Creating a line series and filling it with the data that needs to be displayed
        auto pLineSeries = new QLineSeries();
        // Setting the title with the current DataLine name
        pLineSeries->setName(QString::fromStdString(diagram.GetDataLineTitle(data_line_counter)));
        // Setting the data with the DataPoints of the DataLine
        DefaultDiagram::index_t number_of_data_points = diagram.GetTheNumberOfDataPoints(data_line_counter);
        // Variable to store the min/max values of the DataLine
        auto data_line_extreme_values = diagram.GetExtremeValues(data_line_counter);

        for(DefaultDiagram::index_t data_point_counter = 0; data_point_counter < number_of_data_points; ++data_point_counter)
        {
            auto data_point = diagram.GetDataPoint(data_line_counter, data_point_counter);
            pLineSeries->append(data_point.GetX(), data_point.GetY());
        }

        // Adding the line series to the chart
        pNewChart->addSeries(pLineSeries);
        auto pYAxis = new QValueAxis;
        pYAxis->setTitleText(pLineSeries->name());
        qreal y_axis_range_minimum = data_line_extreme_values.first.GetY() - (std::abs(data_line_extreme_values.first.GetY()) * y_axis_range_multiplicator);
        qreal y_axis_range_maximum = data_line_extreme_values.second.GetY() + (std::abs(data_line_extreme_values.second.GetY()) * y_axis_range_multiplicator);
        pYAxis->setTickCount(y_axis_tick_count);
        pYAxis->setMinorTickCount(y_axis_minor_tick_count);
        pYAxis->setRange(y_axis_range_minimum, y_axis_range_maximum);
        pYAxis->setTitleBrush(pLineSeries->pen().color());
        pNewChart->addAxis(pYAxis, Qt::AlignLeft);
        pLineSeries->attachAxis(pXAxis);
        pLineSeries->attachAxis(pYAxis);
    }

    // Setting up the X axis
    pXAxis->setRange(diagram_extreme_values.first.GetX(), diagram_extreme_values.second.GetX());

    // Saving the the current pChart from the pChartView
    auto pOldChart = pChartView->chart();
    // Adding the new chart to the chart view
    pChartView->setChart(pNewChart);
    // Deleting the old chart (if there was one) because chart view object is not its parent anymore
    if(pOldChart)
    {
        delete pOldChart;
    }
}

std::string MainWindow::CreateFileDialogFilterString(void)
{
    std::string filter_string = file_dialog_filter_string_constant_part;
    auto file_extensions = backend_signal_interface->GetSupportedFileExtensions();

    for(auto const& extension : file_extensions)
    {
        filter_string += extension;
        filter_string += " ";
    }

    filter_string += "(";
    for(auto const& extension : file_extensions)
    {
        filter_string += " ";
        filter_string += "*";
        filter_string += extension;
        filter_string += " ";
    }
    filter_string += ")";

    return filter_string;
}
