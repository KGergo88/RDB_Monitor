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



#include <QtWidgets>
#include <QtCharts>

#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#include "global.hpp"
#include "diagram.hpp"
#include "serial_port.hpp"

#ifndef GUI_HPP
#define GUI_HPP



class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class Gui;

signals:
    void signalDisplayDiagram(std::size_t index);
    void signalAddToDiagramList(std::size_t index);
    void signalReportStatus(std::string message);

private slots:
    void slotDisplayDiagram(std::size_t index);
    void slotAddToDiagramList(std::size_t index);
    void slotReportStatus(std::string message);
    void slotListSelectionChanged(void);
    void slotPushButtonWasClicked(void);

private:
    MainWindow();

    void resizeEvent(QResizeEvent* event);
    void SetSizes(void);

public:
    static constexpr qreal y_axis_range_multiplicator = 0.05;

    QChartView*     pChartView;
    QListWidget*    pListWidgetDiagrams;
    QListWidget*    pListWidgetStatus;
    QLineEdit*      pLineEdit;
    QPushButton*    pPushButton;

    std::vector<DiagramSpecialized> diagram_container;

    MainWindow(const MainWindow&  newGuiWindow) = delete;
    MainWindow(MainWindow&& newGuiWindow) = delete;

    MainWindow& operator=(const MainWindow&  newGuiWindow) = delete;
    MainWindow& operator=(MainWindow&& newGuiWindow) = delete;
};



class Gui final
{
private:
    static QApplication* pQtApplication;
    static MainWindow* pMainWindow;
    static std::mutex mutex;

    Gui();
    ~Gui();

public:
    static constexpr std::size_t report_date_and_time_string_size = 10;

    Gui(const Gui&  newGui) = delete;
    Gui(Gui&& newGui) = delete;

    Gui& operator=(const Gui&  newGui) = delete;
    Gui& operator=(Gui&& newGui) = delete;

    static void Run(int argc, char **argv);
    static bool IsRunning(void);
    static void AddToDiagramList(DiagramSpecialized&& diagram);
    static void ReportStatus(const std::string& message);
};

#endif /* GUI_HPP */
