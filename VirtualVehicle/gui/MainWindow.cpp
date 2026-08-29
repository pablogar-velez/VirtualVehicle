#include "MainWindow.h"

#include <string>
#include <vector>

#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "widgets/VehicleStateWidget.h"
#include "widgets/AbsStateWidget.h"
#include "widgets/ScenarioControlWidget.h"
#include "widgets/FaultInjectionWidget.h"
#include "widgets/EventLogWidget.h"
#include "widgets/CanMonitorWidget.h"
#include "widgets/StatisticsWidget.h"
#include "widgets/TestRunnerWidget.h"

#include "../test/TestRunner.h"

// ==================================================
// Constructor
// ==================================================

MainWindow::MainWindow(
    QWidget* parent)
    : QMainWindow(parent),
    engine(LogLevel::Quiet)
{
    setWindowTitle(
        "Virtual Vehicle Platform"
    );

    resize(
        1350,
        850
    );

    setupUi();

    setupConnections();

    // ==================================================
    // Initial scenario
    // ==================================================

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    // ==================================================
    // Simulation timer
    // ==================================================

    simulationTimer.setInterval(
        16
    );

    connect(
        &simulationTimer,
        &QTimer::timeout,
        this,
        [this]()
        {
            updateSimulation();
        }
    );

    simulationTimer.start();

    refreshUi();
}

// ==================================================
// Main UI
// ==================================================

void MainWindow::setupUi()
{
    QWidget* centralWidget =
        new QWidget(this);

    setCentralWidget(
        centralWidget
    );

    QVBoxLayout* mainLayout =
        new QVBoxLayout(
            centralWidget
        );

    mainLayout->setContentsMargins(
        24,
        20,
        24,
        24
    );

    mainLayout->setSpacing(
        14
    );

    // ==================================================
    // Title
    // ==================================================

    QLabel* titleLabel =
        new QLabel(
            "VIRTUAL VEHICLE PLATFORM"
        );

    QFont titleFont =
        titleLabel->font();

    titleFont.setPointSize(
        20
    );

    titleFont.setBold(
        true
    );

    titleLabel->setFont(
        titleFont
    );

    titleLabel->setAlignment(
        Qt::AlignCenter
    );

    mainLayout->addWidget(
        titleLabel
    );

    // ==================================================
    // Simulation controls
    // ==================================================

    QHBoxLayout* controlLayout =
        new QHBoxLayout();

    controlLayout->setSpacing(
        12
    );

    // --------------------------------------------------
    // Simulation time
    // --------------------------------------------------

    timeValueLabel =
        new QLabel(
            "0.000 ms"
        );

    QFont infoFont =
        timeValueLabel->font();

    infoFont.setPointSize(
        11
    );

    infoFont.setBold(
        true
    );

    timeValueLabel->setFont(
        infoFont
    );

    // --------------------------------------------------
    // Simulation status
    // --------------------------------------------------

    simulationStatusLabel =
        new QLabel(
            "RUNNING"
        );

    simulationStatusLabel->setFont(
        infoFont
    );

    // --------------------------------------------------
    // Buttons
    // --------------------------------------------------

    pauseResumeButton =
        new QPushButton(
            "Pause"
        );

    resetButton =
        new QPushButton(
            "Reset"
        );

    // --------------------------------------------------
    // Layout
    // --------------------------------------------------

    controlLayout->addStretch();

    controlLayout->addWidget(
        new QLabel(
            "Simulation:"
        )
    );

    controlLayout->addWidget(
        timeValueLabel
    );

    controlLayout->addSpacing(
        20
    );

    controlLayout->addWidget(
        simulationStatusLabel
    );

    controlLayout->addSpacing(
        20
    );

    controlLayout->addWidget(
        pauseResumeButton
    );

    controlLayout->addWidget(
        resetButton
    );

    controlLayout->addStretch();

    mainLayout->addLayout(
        controlLayout
    );

    // ==================================================
    // Tabs
    // ==================================================

    tabWidget =
        new QTabWidget();

    mainLayout->addWidget(
        tabWidget,
        1
    );

    createDashboardTab();

    createCanMonitorTab();

    createDiagnosticsTab();

    createStatisticsTab();

    createTestRunnerTab();
}

// ==================================================
// Dashboard Tab
// ==================================================

void MainWindow::createDashboardTab()
{
    QWidget* dashboardPage =
        new QWidget();

    QVBoxLayout* dashboardLayout =
        new QVBoxLayout(
            dashboardPage
        );

    dashboardLayout->setContentsMargins(
        16,
        16,
        16,
        16
    );

    dashboardLayout->setSpacing(
        16
    );

    // ==================================================
    // Vehicle + ABS
    // ==================================================

    QHBoxLayout* stateLayout =
        new QHBoxLayout();

    stateLayout->setSpacing(
        16
    );

    vehicleStateWidget =
        new VehicleStateWidget();

    absStateWidget =
        new AbsStateWidget();

    stateLayout->addWidget(
        vehicleStateWidget
    );

    stateLayout->addWidget(
        absStateWidget
    );

    dashboardLayout->addLayout(
        stateLayout
    );

    // ==================================================
    // Scenario Control
    // ==================================================

    scenarioControlWidget =
        new ScenarioControlWidget();

    dashboardLayout->addWidget(
        scenarioControlWidget
    );

    // ==================================================
    // Event Log
    // ==================================================

    eventLogWidget =
        new EventLogWidget();

    dashboardLayout->addWidget(
        eventLogWidget,
        1
    );

    // ==================================================
    // Add Tab
    // ==================================================

    tabWidget->addTab(
        dashboardPage,
        "Dashboard"
    );
}

// ==================================================
// CAN Monitor Tab
// ==================================================

void MainWindow::createCanMonitorTab()
{
    QWidget* canPage =
        new QWidget();

    QVBoxLayout* canLayout =
        new QVBoxLayout(
            canPage
        );

    canLayout->setContentsMargins(
        16,
        16,
        16,
        16
    );

    canMonitorWidget =
        new CanMonitorWidget();

    canLayout->addWidget(
        canMonitorWidget,
        1
    );

    tabWidget->addTab(
        canPage,
        "CAN Monitor"
    );
}

// ==================================================
// Diagnostics Tab
// ==================================================

void MainWindow::createDiagnosticsTab()
{
    QWidget* diagnosticsPage =
        new QWidget();

    QVBoxLayout* diagnosticsLayout =
        new QVBoxLayout(
            diagnosticsPage
        );

    diagnosticsLayout->setContentsMargins(
        16,
        16,
        16,
        16
    );

    diagnosticsLayout->setSpacing(
        16
    );

    // ==================================================
    // Fault Injection
    // ==================================================

    faultInjectionWidget =
        new FaultInjectionWidget();

    diagnosticsLayout->addWidget(
        faultInjectionWidget
    );

    diagnosticsLayout->addStretch();

    // ==================================================
    // Add Tab
    // ==================================================

    tabWidget->addTab(
        diagnosticsPage,
        "Diagnostics"
    );
}

// ==================================================
// Statistics Tab
// ==================================================

void MainWindow::createStatisticsTab()
{
    QWidget* statisticsPage =
        new QWidget();

    QVBoxLayout* statisticsLayout =
        new QVBoxLayout(
            statisticsPage
        );

    statisticsLayout->setContentsMargins(
        16,
        16,
        16,
        16
    );

    statisticsLayout->setSpacing(
        16
    );

    // ==================================================
    // Statistics Widget
    // ==================================================

    statisticsWidget =
        new StatisticsWidget();

    statisticsLayout->addWidget(
        statisticsWidget,
        1
    );

    // ==================================================
    // Add Tab
    // ==================================================

    tabWidget->addTab(
        statisticsPage,
        "Statistics"
    );
}

// ==================================================
// Test Runner Tab
// ==================================================

void MainWindow::createTestRunnerTab()
{
    QWidget* testRunnerPage =
        new QWidget();

    QVBoxLayout* testRunnerLayout =
        new QVBoxLayout(
            testRunnerPage
        );

    testRunnerLayout->setContentsMargins(
        16,
        16,
        16,
        16
    );

    testRunnerLayout->setSpacing(
        16
    );

    // ==================================================
    // Automated Validation Widget
    // ==================================================

    testRunnerWidget =
        new TestRunnerWidget();

    testRunnerLayout->addWidget(
        testRunnerWidget,
        1
    );

    // ==================================================
    // Add Tab
    // ==================================================

    tabWidget->addTab(
        testRunnerPage,
        "Test Runner"
    );
}

// ==================================================
// Connections
// ==================================================

void MainWindow::setupConnections()
{
    // ==================================================
    // Pause / Resume
    // ==================================================

    connect(
        pauseResumeButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            togglePauseResume();
        }
    );

    // ==================================================
    // Reset
    // ==================================================

    connect(
        resetButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            resetSimulation();
        }
    );

    // ==================================================
    // Scenario - Acceleration
    // ==================================================

    connect(
        scenarioControlWidget->getAccelerationButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.startScenario(
                DrivingScenario::Acceleration
            );
        }
    );

    // ==================================================
    // Scenario - Cruise
    // ==================================================

    connect(
        scenarioControlWidget->getCruiseButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.startScenario(
                DrivingScenario::Cruise
            );
        }
    );

    // ==================================================
    // Scenario - Hard Braking
    // ==================================================

    connect(
        scenarioControlWidget->getHardBrakingButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.startScenario(
                DrivingScenario::HardBraking
            );
        }
    );

    // ==================================================
    // Scenario - Recovery
    // ==================================================

    connect(
        scenarioControlWidget->getRecoveryButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.startScenario(
                DrivingScenario::Recovery
            );
        }
    );

    // ==================================================
    // Front Left Sensor Dropout
    // ==================================================

    connect(
        faultInjectionWidget->getFrontLeftDropoutButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.setFrontLeftWheelSensorFault(
                SensorFault::Dropout
            );
        }
    );

    // ==================================================
    // Front Right Sensor Dropout
    // ==================================================

    connect(
        faultInjectionWidget->getFrontRightDropoutButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.setFrontRightWheelSensorFault(
                SensorFault::Dropout
            );
        }
    );

    // ==================================================
    // Clear Front Left Fault
    // ==================================================

    connect(
        faultInjectionWidget->getClearFrontLeftFaultButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.clearFrontLeftWheelSensorFault();
        }
    );

    // ==================================================
    // Clear Front Right Fault
    // ==================================================

    connect(
        faultInjectionWidget->getClearFrontRightFaultButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.clearFrontRightWheelSensorFault();
        }
    );

    // ==================================================
    // Run Selected Test
    // ==================================================

    connect(
        testRunnerWidget->getRunSelectedButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            runSelectedTest();
        }
    );

    // ==================================================
    // Run All Tests
    // ==================================================

    connect(
        testRunnerWidget->getRunAllButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            runAllTests();
        }
    );
}

// ==================================================
// Simulation Update
// ==================================================

void MainWindow::updateSimulation()
{
    if (!simulationPaused)
    {
        engine.update(
            16.0
        );
    }

    refreshUi();
}

// ==================================================
// Pause / Resume
// ==================================================

void MainWindow::togglePauseResume()
{
    simulationPaused =
        !simulationPaused;

    if (simulationPaused)
    {
        simulationStatusLabel->setText(
            "PAUSED"
        );

        pauseResumeButton->setText(
            "Resume"
        );
    }
    else
    {
        simulationStatusLabel->setText(
            "RUNNING"
        );

        pauseResumeButton->setText(
            "Pause"
        );
    }
}

// ==================================================
// Reset Simulation
// ==================================================

void MainWindow::resetSimulation()
{
    // ==================================================
    // Backend
    // ==================================================

    engine.reset();

    // ==================================================
    // Frontend
    // ==================================================

    eventLogWidget->clear();

    canMonitorWidget->clear();

    simulationPaused =
        false;

    simulationStatusLabel->setText(
        "RUNNING"
    );

    pauseResumeButton->setText(
        "Pause"
    );

    // ==================================================
    // Restart initial scenario
    // ==================================================

    engine.startScenario(
        DrivingScenario::Acceleration
    );

    refreshUi();
}

// ==================================================
// Run Selected Test
// ==================================================

void MainWindow::runSelectedTest()
{
    const std::string testId =
        testRunnerWidget
        ->getSelectedTestId();

    TestRunner runner;

    const TestCase result =
        runner.runTest(
            testId
        );

    testRunnerWidget->setSingleResult(
        result
    );
}

// ==================================================
// Run All Tests
// ==================================================

void MainWindow::runAllTests()
{
    TestRunner runner;

    const std::vector<TestCase> results =
        runner.runAll();

    testRunnerWidget->setResults(
        results
    );
}

// ==================================================
// Refresh UI
// ==================================================

void MainWindow::refreshUi()
{
    // ==================================================
    // Simulation Time
    // ==================================================

    timeValueLabel->setText(
        QString::number(
            engine.getCurrentTimeMs(),
            'f',
            3
        ) +
        " ms"
    );

    // ==================================================
    // Vehicle State
    // ==================================================

    vehicleStateWidget->updateState(
        engine.getVehicleState()
    );

    // ==================================================
    // ABS State
    // ==================================================

    absStateWidget->updateState(
        engine.getAbsState()
    );

    // ==================================================
    // Event Log
    // ==================================================

    eventLogWidget->updateEvents(
        engine.getEvents()
    );

    // ==================================================
    // CAN Monitor
    // ==================================================

    canMonitorWidget->updateTrace(
        engine.getCanTrace()
    );

    // ==================================================
    // CAN Statistics
    // ==================================================

    statisticsWidget->updateStatistics(
        engine.getCanStatistics(),
        engine.getCanBitrate(),
        engine.getCurrentTimeMs()
    );
}