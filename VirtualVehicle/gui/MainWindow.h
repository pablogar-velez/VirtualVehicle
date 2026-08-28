#pragma once

#include <QMainWindow>
#include <QTimer>

#include "../simulation/SimulationEngine.h"

class QLabel;
class QPushButton;
class QTabWidget;

class VehicleStateWidget;
class AbsStateWidget;
class ScenarioControlWidget;
class FaultInjectionWidget;
class EventLogWidget;
class CanMonitorWidget;
class StatisticsWidget;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(
        QWidget* parent = nullptr
    );

private:
    // ==================================================
    // Backend
    // ==================================================

    SimulationEngine engine;

    QTimer simulationTimer;

    bool simulationPaused{ false };

    // ==================================================
    // Header
    // ==================================================

    QLabel* timeValueLabel{};
    QLabel* simulationStatusLabel{};

    QPushButton* pauseResumeButton{};
    QPushButton* resetButton{};

    // ==================================================
    // Navigation
    // ==================================================

    QTabWidget* tabWidget{};

    // ==================================================
    // Dashboard
    // ==================================================

    VehicleStateWidget* vehicleStateWidget{};
    AbsStateWidget* absStateWidget{};
    ScenarioControlWidget* scenarioControlWidget{};
    EventLogWidget* eventLogWidget{};

    // ==================================================
    // CAN Monitor
    // ==================================================

    CanMonitorWidget* canMonitorWidget{};

    // ==================================================
    // Diagnostics
    // ==================================================

    FaultInjectionWidget* faultInjectionWidget{};

    // ==================================================
    // Statistics
    // ==================================================

    StatisticsWidget* statisticsWidget{};

    // ==================================================
    // Setup
    // ==================================================

    void setupUi();

    void setupConnections();

    void createDashboardTab();

    void createCanMonitorTab();

    void createDiagnosticsTab();

    void createStatisticsTab();

    // ==================================================
    // Runtime
    // ==================================================

    void updateSimulation();

    void refreshUi();

    void togglePauseResume();

    void resetSimulation();
};