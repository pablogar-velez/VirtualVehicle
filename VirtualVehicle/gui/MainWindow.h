#pragma once

#include <QMainWindow>
#include <QTimer>

#include "../simulation/SimulationEngine.h"

class QLabel;
class QPushButton;
class QStackedWidget;
class QWidget;

class VehicleStateWidget;
class AbsStateWidget;
class ScenarioControlWidget;
class FaultInjectionWidget;
class EventLogWidget;
class CanMonitorWidget;
class StatisticsWidget;
class TestRunnerWidget;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    SimulationEngine engine;
    QTimer simulationTimer;
    QTimer uiRefreshTimer;

    bool simulationPaused{ false };
    bool darkModeEnabled{ false };

    QLabel* timeValueLabel{};
    QLabel* simulationStatusLabel{};

    QPushButton* autoDriveButton{};
    QPushButton* pauseResumeButton{};
    QPushButton* resetButton{};
    QPushButton* themeToggleButton{};

    QWidget* sidebarWidget{};

    QPushButton* dashboardNavigationButton{};
    QPushButton* canNavigationButton{};
    QPushButton* diagnosticsNavigationButton{};
    QPushButton* networkNavigationButton{};
    QPushButton* testRunnerNavigationButton{};

    QStackedWidget* pageStack{};

    VehicleStateWidget* vehicleStateWidget{};
    AbsStateWidget* absStateWidget{};
    ScenarioControlWidget* scenarioControlWidget{};
    EventLogWidget* eventLogWidget{};

    QLabel* powertrainEcuStatusLabel{};
    QLabel* absEcuStatusLabel{};
    QLabel* steeringEcuStatusLabel{};
    QLabel* dashboardEcuStatusLabel{};

    QLabel* canBusHealthLabel{};
    QLabel* ethernetHealthLabel{};
    QLabel* diagnosticsHealthLabel{};
    QLabel* driveModeHealthLabel{};

    CanMonitorWidget* canMonitorWidget{};
    FaultInjectionWidget* faultInjectionWidget{};
    StatisticsWidget* statisticsWidget{};
    TestRunnerWidget* testRunnerWidget{};

    void setupUi();
    void setupConnections();

    QWidget* createHeader();
    QWidget* createSidebar();

    QWidget* createDashboardPage();
    QWidget* createEcuStatusCard();
    QWidget* createSystemHealthCard();

    QWidget* createCanMonitorPage();
    QWidget* createDiagnosticsPage();
    QWidget* createNetworkPage();
    QWidget* createTestRunnerPage();

    void showPage(int pageIndex);
    void updateNavigationSelection();

    void toggleTheme();
    void applyTheme();

    void updateSimulation();
    void refreshUi();

    void togglePauseResume();
    void resetSimulation();

    void runAllTests();
    void runSelectedTest();
};
