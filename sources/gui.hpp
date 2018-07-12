/*
 * File:   gui.hpp
 * Author: gergo
 *
 * Created on August 27, 2017, 12:37 PM
 */

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
