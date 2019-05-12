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



#include "main_window.hpp"



MainWindow::MainWindow() : QMainWindow(),
                           backend_signal_interface(nullptr)
{
    network_connection_is_open = false;

    // Adding the object to the main window that will display the charts with anti-aliasing and the zooming turned off
    pChartView = new QChartView();
    pChartView->setRenderHint(QPainter::Antialiasing);
    pChartView->setRubberBand(QChartView::NoRubberBand);

    // Adding the object to the main window that will list the processed diagrams to be selected to display
    pTreeView = new QTreeView();
    pTreeView->setAnimated(true);

    // Adding the object to the main window that will list the status messages
    pListWidgetStatus = new QListWidget();

    pWidgetConnectionManager = new ConnectionManagerWidget();
    pWidgetDiagramExport = new DiagramExportWidget();

    QVBoxLayout *pLeftVerticalLayout = new QVBoxLayout();
    pLeftVerticalLayout->addWidget(pChartView, chart_view_size_percentage);
    pLeftVerticalLayout->addWidget(pListWidgetStatus, list_widget_status_size_percentage);

    pStackedLayout = new QStackedLayout();
    pStackedLayout->addWidget(pWidgetConnectionManager);
    pStackedLayout->addWidget(pWidgetDiagramExport);
    pStackedLayout->setCurrentWidget(pWidgetConnectionManager);

    QVBoxLayout *pRightVerticalLayout = new QVBoxLayout();
    pRightVerticalLayout->addWidget(pTreeView, tree_view_size_percentage);
    pRightVerticalLayout->addLayout(pStackedLayout, stacked_layout_size_percentage);

    QHBoxLayout *pHorizontalLayout = new QHBoxLayout();
    pHorizontalLayout->addLayout(pLeftVerticalLayout, left_vertical_layout_size_percentage);
    pHorizontalLayout->addLayout(pRightVerticalLayout, right_vertical_layout_size_percentage);

    QWidget *pCentralWidget = new QWidget(this);
    pCentralWidget->setLayout(pHorizontalLayout);
    setCentralWidget(pCentralWidget);

    pDiagramsMenu = menuBar()->addMenu(diagram_menu_text);
    pDiagramsMenu->addAction(diagram_menu_import_diagrams_text, this, &MainWindow::MenuActionDiagramsImportDiagrams);
    pDiagramsMenu->addAction(diagram_menu_export_diagrams_text, this, &MainWindow::MenuActionDiagramsExportDiagrams);

    // Setting the minimum size and the title of the main window and showing it maximized
    setMinimumSize(main_window_minimum_width, main_window_minimum_height);
    setWindowTitle(QString::fromStdString((APPLICATION_NAME)));

    showMaximized();
}

void MainWindow::MenuActionDiagramsImportDiagrams(void)
{
#warning "Magic strings..."
    QFileDialog *pImportFileSelectorDialog = new QFileDialog(this, diagram_menu_import_diagrams_text, "/home/", "Diagram Files: .mdp .jfst (*.mdp *.jfst)");
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

void MainWindow::RegisterBackendSignalInterface(BackendSignalInterface* new_backend_signal_interface)
{
    // If the backend signal interface is valid
    if(new_backend_signal_interface)
    {
        // Registering the interface
        backend_signal_interface = new_backend_signal_interface;
        // Setting up the TreeView with the Model from the interface (this needs to be done before establishing the signal connections)
        pTreeView->setModel(backend_signal_interface->GetDiagramContainerModel());

        // Registering the connections between the signals and the slots
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface),       SIGNAL(NewStatusMessage(const std::string&)),
                         this,                                                   SLOT(DisplayStatusMessage(const std::string&)));
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface),       SIGNAL(NetworkOperationFinished(const std::string&, const bool&)),
                         this,                                                   SLOT(ProcessNetworkOperationResult(const std::string&, const bool&)));
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface),       SIGNAL(ShowThisDiagram(const DiagramSpecialized&)),
                         this,                                                   SLOT(DisplayDiagram(const DiagramSpecialized&)));
        QObject::connect(pWidgetConnectionManager->button_open_close_connection, &QPushButton::clicked,
                         this,                                                   &MainWindow::ConnectionManagerButtonOpenCloseWasClicked);
        QObject::connect(pWidgetDiagramExport->button_export,                    &QPushButton::clicked,
                         this,                                                   &MainWindow::DiagramExportButtonExportWasClicked);
        QObject::connect(pWidgetDiagramExport->button_cancel,                    &QPushButton::clicked,
                         this,                                                   &MainWindow::DiagramExportButtonCancelWasClicked);
        QObject::connect(pTreeView->selectionModel(),                            &QItemSelectionModel::currentChanged,
                         this,                                                   &MainWindow::TreeviewCurrentSelectionChanged);
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

void MainWindow::ConnectionManagerButtonOpenCloseWasClicked(void)
{
    std::string network_port_name(pWidgetConnectionManager->line_edit_port_name->text().toStdString());

    if(!network_connection_is_open)
    {
        emit OpenNetworkConnection(network_port_name);
    }
    else
    {
        emit CloseNetworkConnection(network_port_name);
    }
}

void MainWindow::DiagramExportButtonExportWasClicked(void)
{
    QFileDialog *pExportFileSelectorDialog = new QFileDialog(this, diagram_menu_export_diagrams_text, "/home/", "Diagram Files: .mdp .jfst (*.mdp *.jfst)");
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

void MainWindow::ProcessNetworkOperationResult(const std::string& port_name, const bool& result)
{
    if(port_name == pWidgetConnectionManager->line_edit_port_name->text().toStdString())
    {
        if(result)
        {
            if(!network_connection_is_open)
            {
                pWidgetConnectionManager->line_edit_port_name->setReadOnly(true);
                pWidgetConnectionManager->line_edit_port_name->setText(ConnectionManagerWidget::button_close_connection_text);
                network_connection_is_open = true;
            }
            else
            {
                pWidgetConnectionManager->line_edit_port_name->setReadOnly(false);
                pWidgetConnectionManager->line_edit_port_name->setText(ConnectionManagerWidget::button_open_connection_text);
                network_connection_is_open = false;
            }
        }
    }
    else
    {
        std::string errorMessage = "A signal was received by MainWindow::ProcessNetworkOperationResult with a wrong port_name: " + port_name;
        throw errorMessage;
    }
}

void MainWindow::DisplayDiagram(const DiagramSpecialized& diagram)
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
    DataIndexType number_of_data_lines = diagram.GetTheNumberOfDataLines();
    // Variables to store the min/max values of the diagram
    auto diagram_extreme_values = diagram.GetExtremeValues();

    for(DataIndexType data_line_counter = 0; data_line_counter < number_of_data_lines; ++data_line_counter)
    {
        // Creating a line series and filling it with the data that needs to be displayed
        auto pLineSeries = new QLineSeries();
        // Setting the title with the current DataLine name
        pLineSeries->setName(QString::fromStdString(diagram.GetDataLineTitle(data_line_counter)));
        // Setting the data with the DataPoints of the DataLine
        DataIndexType number_of_data_points = diagram.GetTheNumberOfDataPoints(data_line_counter);
        // Variable to store the min/max values of the DataLine
        auto data_line_extreme_values = diagram.GetExtremeValues(data_line_counter);

        for(DataIndexType data_point_counter = 0; data_point_counter < number_of_data_points; ++data_point_counter)
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
