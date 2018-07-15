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



#include "gui.hpp"



MainWindow::MainWindow() : QMainWindow()
{
    // Adding the object to the main window that will display the charts with anti-aliasing and the zooming turned off
    pChartView = new QChartView(this);
    pChartView->setRenderHint(QPainter::Antialiasing);
    pChartView->setRubberBand(QChartView::NoRubberBand);

    // Adding the object to the main window that will list the processed diagrams to be selected to display
    pListWidgetDiagrams = new QListWidget(this);

    // Adding the object to the main window that will list the status messages
    pListWidgetStatus = new QListWidget(this);

    // Adding the object to where the user can enter which serial port will be opened
    pLineEdit = new QLineEdit((SERIAL_PORT_DEFAULT_DEVICE_NAME), this);

    // Adding the object with which the user can open and close the serial port
    pPushButton = new QPushButton(push_button_open_text, this);

    // Setting the minimum size and the title of the main window and showing it maximized
    setMinimumSize(main_window_minimum_width, main_window_minimum_height);
    setWindowTitle(QString::fromStdString((APPLICATION_NAME)));
    showMaximized();
}

void MainWindow::slotDisplayDiagram(std::size_t index)
{
    // Check whether the diagram exists
    if(index < diagram_container.size())
    {
        // Accessing the requested diagram from the
        DiagramSpecialized& diagram_to_display = diagram_container[index];

        // Accessing the current pChart from the pChartView
        // and deleting it if there was one previously because we will add a new one with the new data
        auto pChart = pChartView->chart();
        if(!pChart)
        {
            delete pChart;
        }
        pChart = new QChart();

        // Setting the title with the Diagram name
        pChart->setTitle(QString::fromStdString(diagram_to_display.GetTitle()));
        // Hiding the legend because the data lines will be recognisable from their Y axis
        pChart->legend()->hide();
        // Creating the X axis, giving it a title and addig it to the chart. The ranges will only be set after analyzing the data points.
        auto pXAxis = new QValueAxis;
        pXAxis->setTitleText(QString::fromStdString(diagram_to_display.GetAxisXTitle()));
        pChart->addAxis(pXAxis, Qt::AlignBottom);

        // We will add every DataLine of the Diagram to the chart
        DataIndexType number_of_data_lines = diagram_to_display.GetTheNumberOfDataLines();
        // Variables to store the min/max X axis values
        DataPointType x_axis_minimum_value = 0;
        DataPointType x_axis_maximum_value = 0;

        for(DataIndexType data_line_counter = 0; data_line_counter < number_of_data_lines; data_line_counter++)
        {
            // Creating a line series and filling it with the data that needs to be displayed
            auto pLineSeries = new QLineSeries();
            // Setting the title with the current DataLine name
            pLineSeries->setName(QString::fromStdString(diagram_to_display.GetDataLineTitle(data_line_counter)));
            // Setting the data with the DataPoints of the DataLine
            DataIndexType number_of_data_points = diagram_to_display.GetTheNumberOfDataPoints(data_line_counter);
            // Variables to store the min/max Y axis values
            DataPointType y_axis_minimum_value = 0;
            DataPointType y_axis_maximum_value = 0;

            for(DataIndexType data_point_counter = 0; data_point_counter < number_of_data_points; data_point_counter++)
            {
                auto data_point = diagram_to_display.GetDataPoint(data_line_counter, data_point_counter);
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
            pChart->addSeries(pLineSeries);
            auto pYAxis = new QValueAxis;
            pYAxis->setTitleText(pLineSeries->name());
            qreal y_axis_range_minimum = y_axis_minimum_value - (std::abs(y_axis_minimum_value) * y_axis_range_multiplicator);
            qreal y_axis_range_maximum = y_axis_maximum_value + (std::abs(y_axis_maximum_value) * y_axis_range_multiplicator);
            pYAxis->setTickCount(y_axis_tick_count);
            pYAxis->setMinorTickCount(y_axis_minor_tick_count);
            pYAxis->setRange(y_axis_range_minimum, y_axis_range_maximum);
            pYAxis->setTitleBrush(pLineSeries->pen().color());
            pChart->addAxis(pYAxis, Qt::AlignLeft);
            pLineSeries->attachAxis(pXAxis);
            pLineSeries->attachAxis(pYAxis);
        }

        // Setting up the X axis
        pXAxis->setRange(x_axis_minimum_value, x_axis_maximum_value);
        // Adding the chart to the chart view
        pChartView->setChart(pChart);
    }
    else
    {
        // Then we will throw an exception because this case should never occur
        std::string errorMessage = "The indexed Diagram does not exist in the GuiWindow::diagram_container: /n Requested index: ";
        errorMessage += std::to_string(index);
        throw errorMessage;
    }
}

void MainWindow::slotAddToDiagramList(std::size_t index)
{
    if(index < diagram_container.size())
    {
        auto pListWidgetItem = new QListWidgetItem();
        pListWidgetItem->setText(QString::fromStdString(diagram_container[index].GetTitle()));
        pListWidgetDiagrams->addItem(pListWidgetItem);

        if(pChartView->chart()->series().empty())
        {
            pListWidgetDiagrams->setCurrentItem(pListWidgetItem);
        }
    }
}

void MainWindow::slotReportStatus(std::string message)
{
    auto pListWidgetItem = new QListWidgetItem();
    pListWidgetItem->setText(QString::fromStdString(message));
    pListWidgetStatus->insertItem(0, pListWidgetItem);
}

void MainWindow::slotListSelectionChanged(void)
{
    emit slotDisplayDiagram(pListWidgetDiagrams->currentRow());
}

void MainWindow::slotPushButtonWasClicked(void)
{
    if(!SerialPort::GetInstance().IsOpen())
    {
        std::string serial_port_device(pLineEdit->text().toStdString());

        if(SerialPort::GetInstance().Open(serial_port_device))
        {
            pLineEdit->setReadOnly(true);
            pPushButton->setText(push_button_close_text);
            Gui::ReportStatus("The serial port was opened");
        }
        else
        {
            Gui::ReportStatus("The serial port could not be opened. Maybe it is not a valid port name?");
        }
    }
    else
    {
        if(SerialPort::GetInstance().Close())
        {
            pLineEdit->setReadOnly(false);
            pPushButton->setText(push_button_open_text);
            Gui::ReportStatus("The serial port was closed");
        }
        else
        {
            Gui::ReportStatus("The serial port could not be closed");
        }
    }
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
                            (window_width * chartview_width_relative_to_main_window),
                            (window_height * chartview_height_relative_to_main_window));
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
                           (list_widget_status_height * lineedit_height_relative_to_listwidgetstatus));
    int line_edit_height = pLineEdit->height();

    pPushButton->setGeometry(chart_view_width,
                             (chart_view_height + line_edit_height),
                             list_widget_diagrams_width,
                             line_edit_height);
}

///---------------------------------------------------------------------------------------------------------------------------------------------///

QApplication* Gui::pQtApplication;
MainWindow* Gui::pMainWindow;
std::mutex Gui::mutex;

void Gui::Run(int argc, char** argv)
{
    pQtApplication = new QApplication(argc, argv);
    pMainWindow = new MainWindow();

    // Registering the types that are used in the signal and slot function prototypes
    qRegisterMetaType<std::size_t>("std::size_t");
    qRegisterMetaType<std::string>("std::string");

    // Registering the connections between the signals and the slots
    QObject::connect(pMainWindow, SIGNAL(signalDisplayDiagram(std::size_t)), pMainWindow, SLOT(slotDisplayDiagram(std::size_t)));
    QObject::connect(pMainWindow, SIGNAL(signalAddToDiagramList(std::size_t)), pMainWindow, SLOT(slotAddToDiagramList(std::size_t)));
    QObject::connect(pMainWindow, SIGNAL(signalReportStatus(std::string)), pMainWindow, SLOT(slotReportStatus(std::string)));
    QObject::connect(pMainWindow->pListWidgetDiagrams, SIGNAL(itemSelectionChanged()), pMainWindow, SLOT(slotListSelectionChanged()));
    QObject::connect(pMainWindow->pPushButton, SIGNAL(clicked()), pMainWindow, SLOT(slotPushButtonWasClicked()));

    auto qtapplication_return_value = pQtApplication->exec();

    delete pMainWindow;
    delete pQtApplication;

    std::cout << "The QtApplication has returned with: " << qtapplication_return_value << std::endl;
}

bool Gui::IsRunning(void)
{
    std::lock_guard<std::mutex> lock(mutex);

    bool result = false;

    if(pQtApplication && pMainWindow)
    {
        result = !pQtApplication->startingUp();
    }

    return result;
}

void Gui::AddToDiagramList(DiagramSpecialized&& diagram)
{
    std::lock_guard<std::mutex> lock(mutex);

    // Adding the diagram to the container that stores them for the GUI
    pMainWindow->diagram_container.emplace_back(diagram);

    // Emitting a signal to add this diagram to the end of the diagram list
    emit pMainWindow->signalAddToDiagramList(pMainWindow->diagram_container.size() - 1);
}

void Gui::ReportStatus(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex);

    time_t rawtime;
    tm* timeinfo;
    std::string date_and_time_string;

    // Getting the current time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Resizing the string containing the current time to an appropriate size
    date_and_time_string.resize(report_date_and_time_string_size);

    // Assembling the status message from the time and the input text
    strftime(&date_and_time_string[0], date_and_time_string.size(), "%T", timeinfo);
    std::string complete_message = date_and_time_string + " - " + message;

    // Emitting the signal
    emit pMainWindow->signalReportStatus(complete_message);
}
