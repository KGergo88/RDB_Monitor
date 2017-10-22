/*
 * File:   gui.cpp
 * Author: gergo
 *
 * Created on August 27, 2017, 12:37 PM
 */

#include "gui.hpp"



void GuiWindow::slotDisplayDiagram(std::size_t index)
{
    if(index < diagram_container.size())
    {
        DiagramObject& diagram_to_display = diagram_container[index];

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
        pListWidget->addItem(pListWidgetItem);

        std::cout << "GuiWindow::slotAddToDiagramList() was called!" << std::endl;
        std::cout << "The diagram_container content actually is: ";
        for(const auto i : diagram_container)
        {
            std::cout << i.GetTitle() << ", ";
        }
        std::cout << std::endl;

        if(pChartView->chart()->series().empty())
        {
            pListWidget->setCurrentItem(pListWidgetItem);
        }
    }
}

void GuiWindow::slotListSelectionChanged(void)
{
    auto selected_items = pListWidget->currentItem();

    std::cout << "The selected item now is: " << selected_items->text().toStdString()
              << ". The row of this item is: " << pListWidget->currentRow() << std::endl;
    emit slotDisplayDiagram(pListWidget->currentRow());
}

///---------------------------------------------------------------------------------------------------------------------------------------------///

void GuiWindow::SetSizes(void)
{
    setMinimumSize(500, 500);
    int window_width = size().width();
    int window_height = size().height();

    pChartView->setGeometry(0, 0, window_width * 0.8, window_height * 0.9);
    int chart_view_width = pChartView->width();
    int chart_view_height = pChartView->height();

    pListWidget->setGeometry(chart_view_width, 0, window_width - chart_view_width, chart_view_height);
    int list_widget_width = pListWidget->width();
    int list_widget_height = pListWidget->height();

    pLineEdit->setGeometry(0, chart_view_height, chart_view_width, window_height - chart_view_height);

    pPushButton->setGeometry(chart_view_width, list_widget_height, list_widget_width, window_height - list_widget_height);
}

Gui::Gui(int argc, char **argv) : QtApplication(argc, argv), window()
{
    std::cout << "Constructing GUI! Thread ID: " << std::this_thread::get_id() << std::endl;

    window.pChartView = new QChartView(&window);
    window.pChartView->setRenderHint(QPainter::Antialiasing);
    window.pChartView->setRubberBand(QChartView::HorizontalRubberBand);

    window.pListWidget = new QListWidget(&window);

    window.pPushButton= new QPushButton("LoadFile", &window);

    window.pLineEdit = new QLineEdit("Insert path of the file to be loaded here...", &window);
#warning "Solve this problem as well..."
//    window.setWindowTitle(QString::fromStdString("RDB Diplomaterv Monitor V1.0"));
    window.showMaximized();
}

Gui& Gui::Get(void)
{
    static Gui Singleton(0, nullptr);

    return Singleton;
}

void Gui::Run(void)
{
    qRegisterMetaType<std::size_t>("std::size_t");
    QObject::connect(&window, SIGNAL(signalDisplayDiagram(std::size_t)), &window, SLOT(slotDisplayDiagram(std::size_t)));
    QObject::connect(&window, SIGNAL(signalAddToDiagramList(std::size_t)), &window, SLOT(slotAddToDiagramList(std::size_t)));

    QObject::connect(window.pListWidget, SIGNAL(itemSelectionChanged()), &window, SLOT(slotListSelectionChanged()));
    is_running = true;
    QtApplication.exec();
}

bool Gui::IsRunning(void)
{
    return is_running;
}

void Gui::AddToDiagramList(const DiagramObject& diagram)
{
    std::lock_guard<std::mutex> lock(mutex);

    std::cout << "Gui::AddToDiagramList( " << diagram.GetTitle() << " );" << std::endl;

    window.diagram_container.emplace_back(diagram);
    emit window.signalAddToDiagramList(window.diagram_container.size() - 1);
}
