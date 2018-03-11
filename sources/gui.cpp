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
    if(index < diagram_container.size())
    {
        DiagramSpecialized& diagram_to_display = diagram_container[index];

        auto old_chart = pChartView->chart();
        if(!old_chart)
        {
            delete old_chart;
        }
        auto new_chart = new QChart();

        DataIndexType number_of_data_lines = diagram_to_display.GetTheNumberOfDataLines();
        for(DataIndexType data_line_counter = 0; data_line_counter < number_of_data_lines; data_line_counter++)
        {
            auto myLine = new QLineSeries();
            myLine->setName(QString::fromStdString(diagram_to_display.GetDataLineTitle(data_line_counter)));
            DataIndexType number_of_data_points = diagram_to_display.GetTheNumberOfDataPoints(data_line_counter);
            for(DataIndexType data_point_counter = 0; data_point_counter < number_of_data_points; data_point_counter++)
            {
                auto temp_data_point = diagram_to_display.GetDataPoint(data_line_counter, data_point_counter);
                myLine->append(temp_data_point.GetX(), temp_data_point.GetY());
            }
            new_chart->addSeries(myLine);
        }
        new_chart->setTitle(QString::fromStdString(diagram_to_display.GetTitle()));
        pChartView->setChart(new_chart);
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

void Gui::AddToDiagramList(DiagramSpecialized& diagram)
{
    std::lock_guard<std::mutex> lock(mutex);

    window.diagram_container.push_back(diagram);
    emit window.signalAddToDiagramList(window.diagram_container.size() - 1);
}

void Gui::ReportStatus(const std::string& message)
{
#warning "There is a new line at the end of the time string. Also the Date is not important in this string"
    auto current_date_and_time = std::time(0);
    std::string complete_message = std::string(ctime(&current_date_and_time)) + " - " + message;
    emit window.signalReportStatus(complete_message);
}
