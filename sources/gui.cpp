/*
 * File:   gui.cpp
 * Author: gergo
 *
 * Created on August 27, 2017, 12:37 PM
 */

#include "gui.hpp"

int Gui::argc_value = 1;
char* default_argv_value = ((char*)"");
char** Gui::argv_value = &default_argv_value;

void GuiWindow::slotDisplayDiagram(std::size_t index)
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
            pYAxis->setTickCount(5);
            pYAxis->setMinorTickCount(0);
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

void GuiWindow::slotAddToDiagramList(std::size_t index)
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

void GuiWindow::slotReportStatus(std::string message)
{
    auto pListWidgetItem = new QListWidgetItem();
    pListWidgetItem->setText(QString::fromStdString(message));
    pListWidgetStatus->insertItem(0, pListWidgetItem);
}

void GuiWindow::slotListSelectionChanged(void)
{
    emit slotDisplayDiagram(pListWidgetDiagrams->currentRow());
}

void GuiWindow::slotPushButtonWasClicked(void)
{
    if(!SerialPort::GetInstance().IsOpen())
    {
        std::string serial_port_device(pLineEdit->text().toStdString());

        if(SerialPort::GetInstance().Open(serial_port_device))
        {
            pLineEdit->setReadOnly(true);
            pPushButton->setText("Close Serial Port");
            Gui::GetInstance().ReportStatus("The serial port was opened");
        }
        else
        {
            Gui::GetInstance().ReportStatus("The serial could not be opened. Maybe it is not a valid port name?");
        }
    }
    else
    {
        if(SerialPort::GetInstance().Close())
        {
            pLineEdit->setReadOnly(false);
            pPushButton->setText("Open Serial Port");
            Gui::GetInstance().ReportStatus("The serial port was closed");
        }
        else
        {
            Gui::GetInstance().ReportStatus("The serial could not be closed");
        }
    }
}
#warning "The magic numbers!!!"
void GuiWindow::SetSizes(void)
{
    setMinimumSize(500, 500);
    int window_width = size().width();
    int window_height = size().height();

    pChartView->setGeometry(0, 0, window_width * 0.8, window_height * 0.9);
    int chart_view_width = pChartView->width();
    int chart_view_height = pChartView->height();

    pListWidgetDiagrams->setGeometry(chart_view_width, 0, window_width - chart_view_width, chart_view_height);
    int list_widget_diagrams_width = pListWidgetDiagrams->width();
    int list_widget_diagrams_height = pListWidgetDiagrams->height();

    pListWidgetStatus->setGeometry(0, chart_view_height, chart_view_width, window_height - list_widget_diagrams_height);
    int list_widget_status_height = pListWidgetStatus->height();

    pLineEdit->setGeometry(chart_view_width, chart_view_height, list_widget_diagrams_width, list_widget_status_height / 2);
    int line_edit_height = pLineEdit->height();

    pPushButton->setGeometry(chart_view_width, chart_view_height + line_edit_height, list_widget_diagrams_width, line_edit_height);
}

///---------------------------------------------------------------------------------------------------------------------------------------------///

Gui::Gui() : QtApplication(argc_value, argv_value), window()
{
    window.pChartView = new QChartView(&window);
    window.pChartView->setRenderHint(QPainter::Antialiasing);
    window.pChartView->setRubberBand(QChartView::HorizontalRubberBand);

    window.pListWidgetDiagrams = new QListWidget(&window);

    window.pListWidgetStatus = new QListWidget(&window);

    window.pLineEdit = new QLineEdit((SERIAL_PORT_DEFAULT_DEVICE_NAME), &window);

    window.pPushButton= new QPushButton("Open Serial Port", &window);

    window.setWindowTitle(QString::fromStdString((APPLICATION_NAME)));
    window.showMaximized();
}

void Gui::Run(void)
{
    qRegisterMetaType<std::size_t>("std::size_t");
    qRegisterMetaType<std::string>("std::string");

    QObject::connect(&window, SIGNAL(signalDisplayDiagram(std::size_t)), &window, SLOT(slotDisplayDiagram(std::size_t)));
    QObject::connect(&window, SIGNAL(signalAddToDiagramList(std::size_t)), &window, SLOT(slotAddToDiagramList(std::size_t)));
    QObject::connect(&window, SIGNAL(signalReportStatus(std::string)), &window, SLOT(slotReportStatus(std::string)));
    QObject::connect(window.pListWidgetDiagrams, SIGNAL(itemSelectionChanged()), &window, SLOT(slotListSelectionChanged()));
    QObject::connect(window.pPushButton, SIGNAL(clicked()), &window, SLOT(slotPushButtonWasClicked()));

    is_running = true;
    QtApplication.exec();
}

bool Gui::IsRunning(void)
{
    return is_running;
}

void Gui::AddToDiagramList(DiagramSpecialized&& diagram)
{
    std::lock_guard<std::mutex> lock(mutex);
    window.diagram_container.emplace_back(diagram);
    emit window.signalAddToDiagramList(window.diagram_container.size() - 1);
}

void Gui::ReportStatus(const std::string& message)
{
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
    emit window.signalReportStatus(complete_message);
}
