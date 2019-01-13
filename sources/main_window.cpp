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



MainWindow::MainWindow() : QMainWindow()
{
    network_connection_is_open = false;

    // Adding the object to the main window that will display the charts with anti-aliasing and the zooming turned off
    pChartView = new QChartView(this);
    pChartView->setRenderHint(QPainter::Antialiasing);
    pChartView->setRubberBand(QChartView::NoRubberBand);

    // Adding the object to the main window that will list the processed diagrams to be selected to display
    pListWidgetDiagrams = new QListWidget(this);

    // Adding the object to the main window that will list the status messages
    pListWidgetStatus = new QListWidget(this);

    // Adding the object to where the user can enter which serial port will be opened
    pLineEdit = new QLineEdit((SERIAL_PORT_DEFAULT_PORT_NAME), this);

    // Adding the object with which the user can open and close the serial port
    pPushButton = new QPushButton(push_button_open_text, this);

    // Registering the types that are used in the signal and slot function prototypes
    qRegisterMetaType<std::size_t>("std::size_t");
    qRegisterMetaType<std::string>("std::string");

    // Setting the minimum size and the title of the main window and showing it maximized
    setMinimumSize(main_window_minimum_width, main_window_minimum_height);
    setWindowTitle(QString::fromStdString((APPLICATION_NAME)));
    showMaximized();
}

void MainWindow::RegisterBackendSignalInterface(BackendSignalInterface* new_backend_signal_interface)
{
    if(new_backend_signal_interface)
    {
        backend_signal_interface = new_backend_signal_interface;

        // Registering the connections between the signals and the slots
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface), SIGNAL(NewStatusMessage(const std::string&)),
                         this,                                             SLOT(DisplayStatusMessage(const std::string&)));
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface), SIGNAL(NetworkOperationFinished(const std::string&, const bool&)),
                         this,                                             SLOT(ProcessNetworkOperationResult(const std::string&, const bool&)));
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface), SIGNAL(ShowThisDiagram(const DiagramSpecialized&)),
                         this,                                             SLOT(DisplayDiagram(const DiagramSpecialized&)));
        QObject::connect(dynamic_cast<QObject*>(backend_signal_interface), SIGNAL(DiagramListHasChanged(const std::vector<std::string>&)),
                         this,                                             SLOT(UpdateDiagramList(const std::vector<std::string>&)));

        QObject::connect(pListWidgetDiagrams, &QListWidget::itemSelectionChanged, this, &MainWindow::DiagramListSelectionChanged);
        QObject::connect(pPushButton,         &QPushButton::clicked,              this, &MainWindow::PushButtonWasClicked);
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

void MainWindow::PushButtonWasClicked(void)
{
    std::string network_port_name(pLineEdit->text().toStdString());

    if(!network_connection_is_open)
    {
        emit OpenNetworkConnection(network_port_name);
    }
    else
    {
        emit CloseNetworkConnection(network_port_name);
    }
}

void MainWindow::ProcessNetworkOperationResult(const std::string& port_name, const bool& result)
{
    if(port_name == pLineEdit->text().toStdString())
    {
        if(result)
        {
            if(!network_connection_is_open)
            {
                pLineEdit->setReadOnly(true);
                pPushButton->setText(push_button_close_text);
                network_connection_is_open = true;
            }
            else
            {
                pLineEdit->setReadOnly(false);
                pPushButton->setText(push_button_open_text);
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
    // Variables to store the min/max X axis values
    DataPointType x_axis_minimum_value = 0;
    DataPointType x_axis_maximum_value = 0;

    for(DataIndexType data_line_counter = 0; data_line_counter < number_of_data_lines; ++data_line_counter)
    {
        // Creating a line series and filling it with the data that needs to be displayed
        auto pLineSeries = new QLineSeries();
        // Setting the title with the current DataLine name
        pLineSeries->setName(QString::fromStdString(diagram.GetDataLineTitle(data_line_counter)));
        // Setting the data with the DataPoints of the DataLine
        DataIndexType number_of_data_points = diagram.GetTheNumberOfDataPoints(data_line_counter);
        // Variables to store the min/max Y axis values
        DataPointType y_axis_minimum_value = 0;
        DataPointType y_axis_maximum_value = 0;


        #warning "These could be maybe method of the diagram class..."
        for(DataIndexType data_point_counter = 0; data_point_counter < number_of_data_points; ++data_point_counter)
        {
            auto data_point = diagram.GetDataPoint(data_line_counter, data_point_counter);
            pLineSeries->append(data_point.GetX(), data_point.GetY());

            // Updating the min/max axis values
            if(data_point.GetX() < x_axis_minimum_value)
            {
                x_axis_minimum_value = data_point.GetX();
            }
            if(x_axis_maximum_value < data_point.GetX())
            {
                x_axis_maximum_value = data_point.GetX();
            }
            if(data_point.GetY() < y_axis_minimum_value)
            {
                y_axis_minimum_value = data_point.GetY();
            }
            if(y_axis_maximum_value < data_point.GetY())
            {
                y_axis_maximum_value = data_point.GetY();
            }
        }

        // Adding the line series to the chart
        pNewChart->addSeries(pLineSeries);
        auto pYAxis = new QValueAxis;
        pYAxis->setTitleText(pLineSeries->name());
        qreal y_axis_range_minimum = y_axis_minimum_value - (std::abs(y_axis_minimum_value) * y_axis_range_multiplicator);
        qreal y_axis_range_maximum = y_axis_maximum_value + (std::abs(y_axis_maximum_value) * y_axis_range_multiplicator);
        pYAxis->setTickCount(y_axis_tick_count);
        pYAxis->setMinorTickCount(y_axis_minor_tick_count);
        pYAxis->setRange(y_axis_range_minimum, y_axis_range_maximum);
        pYAxis->setTitleBrush(pLineSeries->pen().color());
        pNewChart->addAxis(pYAxis, Qt::AlignLeft);
        pLineSeries->attachAxis(pXAxis);
        pLineSeries->attachAxis(pYAxis);
    }

    // Setting up the X axis
    pXAxis->setRange(x_axis_minimum_value, x_axis_maximum_value);

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

void MainWindow::UpdateDiagramList(const std::vector<std::string>& available_diagrams)
{
    pListWidgetDiagrams->clear();

    for(const auto& i : available_diagrams)
    {
        auto pListWidgetItem = new QListWidgetItem();
        pListWidgetItem->setText(QString::fromStdString(i));
        pListWidgetDiagrams->addItem(pListWidgetItem);
    }
}

void MainWindow::DiagramListSelectionChanged(void)
{
    emit RequestForDiagram(static_cast<DataIndexType>(pListWidgetDiagrams->currentRow()));
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    (void) event;

    SetSizes();
}

void MainWindow::SetSizes(void)
{
    int window_width = size().width();
    int window_height = size().height();

    pChartView->setGeometry(chartview_fixposition_x,
                            chartview_fixposition_y,
                            (static_cast<int>(static_cast<qreal>(window_width) * chartview_width_relative_to_main_window)),
                            (static_cast<int>(static_cast<qreal>(window_height) * chartview_height_relative_to_main_window)));
    int chart_view_width = pChartView->width();
    int chart_view_height = pChartView->height();

    pListWidgetDiagrams->setGeometry(chart_view_width,
                                     listwidgetdiagrams_fixposition_y,
                                     (window_width - chart_view_width),
                                     chart_view_height);
    int list_widget_diagrams_width = pListWidgetDiagrams->width();
    int list_widget_diagrams_height = pListWidgetDiagrams->height();

    pListWidgetStatus->setGeometry(listwidgetstatus_fixposition_x,
                                   chart_view_height,
                                   chart_view_width,
                                   (window_height - list_widget_diagrams_height));
    int list_widget_status_height = pListWidgetStatus->height();

    pLineEdit->setGeometry(chart_view_width,
                           chart_view_height,
                           list_widget_diagrams_width,
                           (static_cast<int>(static_cast<qreal>(list_widget_status_height) * lineedit_height_relative_to_listwidgetstatus)));
    int line_edit_height = pLineEdit->height();

    pPushButton->setGeometry(chart_view_width,
                             (chart_view_height + line_edit_height),
                             list_widget_diagrams_width,
                             line_edit_height);
}
