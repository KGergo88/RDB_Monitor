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

class GuiWindow : public QWidget
{
    Q_OBJECT

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
    void resizeEvent(QResizeEvent* event)
    {
        (void) event;

        SetSizes();
    }

public:
    QChartView*     pChartView;
    QListWidget*    pListWidgetDiagrams;
    QListWidget*    pListWidgetStatus;
    QPushButton*    pPushButton;
    QLineEdit*      pLineEdit;

    std::vector<DiagramObject> diagram_container;

    GuiWindow(QWidget *parent = nullptr) : QWidget(parent) {}

    GuiWindow(const GuiWindow&  newGuiWindow) = delete;
    GuiWindow(GuiWindow&& newGuiWindow) = delete;

    GuiWindow& operator=(const GuiWindow&  newGuiWindow) = delete;
    GuiWindow& operator=(GuiWindow&& newGuiWindow) = delete;

    void SetSizes(void);
};

    Q_DECLARE_METATYPE(std::size_t)

class Gui
{
private:
    bool is_running = false;
    std::mutex mutex;
    QApplication QtApplication;
    GuiWindow window;

    Gui(int argc, char **argv);

public:
    Gui(const Gui&  newGui) = delete;
    Gui(Gui&& newGui) = delete;

    Gui& operator=(const Gui&  newGui) = delete;
    Gui& operator=(Gui&& newGui) = delete;

    static Gui& GetInstance(void)
    {
        static Gui Singleton(0, nullptr);
        return Singleton;
    }

    void Run(void);
    bool IsRunning(void);
    void AddToDiagramList(DiagramObject& diagram);
    void ReportStatus(const std::string& message);
};

#endif /* GUI_HPP */
