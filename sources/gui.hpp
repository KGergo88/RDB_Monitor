/*
 * File:   gui.hpp
 * Author: gergo
 *
 * Created on August 27, 2017, 12:37 PM
 */

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
QT_CHARTS_USE_NAMESPACE

#include <memory>

#include "global.hpp"
#include "diagram.hpp"

#ifndef GUI_H
#define GUI_H

class Gui
{
public:
    Gui(int &argc, char **argv) : QtApplication(argc, argv)
    {

    }

    Gui(Gui&  newGui) = delete;
    Gui(Gui&& newGui) = delete;

    Gui& operator=(Gui&  newGui) = delete;
    Gui& operator=(Gui&& newGui) = delete;

    void SetData(Diagram<DataPointType, DataIndexType>& diagramData)
    {
        Title = diagramData.GetTitle();
        DataIndexType numberOfDataLines = diagramData.GetTheNumberOfDataLines();
        for(DataIndexType dataLineCounter = 0; dataLineCounter < numberOfDataLines; dataLineCounter++)
        {
            displayedDataLines.push_back(std::make_unique<QSplineSeries>());
            displayedDataLines.back()->setName(QString::fromStdString(diagramData.GetDataLineTitle(dataLineCounter)));
            DataIndexType dataPointCounter;
            DataIndexType numberofDataPoints = diagramData.GetTheNumberOfDataPoints(dataLineCounter);
            for(dataPointCounter = 0; dataPointCounter < numberofDataPoints; dataPointCounter++)
            {
                const DataPoint<DataPointType> tempDataPoint = diagramData.GetDataPoint(dataLineCounter, dataPointCounter);
                displayedDataLines.back()->append(tempDataPoint.GetX(), tempDataPoint.GetY());
            }
        }
    }

    void StartGui(void)
    {
        QChart *displayedChart = new QChart();
        for(DataIndexType lineCounter = 0; lineCounter < displayedDataLines.size(); lineCounter++)
        {
            displayedChart->addSeries(displayedDataLines[lineCounter].get());
        }
        displayedChart->setTitle(QString::fromStdString(Title));
        displayedChart->createDefaultAxes();
        displayedChart->axisY()->setRange(-100, 5500);
        QChartView *myChartView = new QChartView(displayedChart);
        myChartView->setRenderHint(QPainter::Antialiasing);
        QMainWindow window;
        window.setCentralWidget(myChartView);
        window.resize(400, 300);
        window.show();
        QtApplication.exec();
    }

private:
    QApplication QtApplication;
    std::string Title;
    std::vector<std::unique_ptr<QSplineSeries> > displayedDataLines;
};

#endif /* GUI_HPP */
