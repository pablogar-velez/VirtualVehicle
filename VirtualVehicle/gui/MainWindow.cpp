#include "MainWindow.h"

#include <array>
#include <string>
#include <vector>

#include <QAbstractItemView>
#include <QCheckBox>
#include <QFrame>
#include <QGroupBox>
#include <QHeaderView>
#include <QIcon>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QSizePolicy>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableWidget>
#include <QStyle>
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

namespace
{
    enum class UiIcon
    {
        Dashboard,
        Can,
        Diagnostics,
        Network,
        Tests,
        Acceleration,
        Cruise,
        Braking,
        Recovery,
        Ecu,
        Health
    };

    QIcon makeUiIcon(
        UiIcon iconType,
        const QColor& color = QColor("#AFC7DD"))
    {
        QPixmap pixmap(
            24,
            24
        );

        pixmap.fill(
            Qt::transparent
        );

        QPainter painter(
            &pixmap
        );

        painter.setRenderHint(
            QPainter::Antialiasing,
            true
        );

        QPen pen(
            color,
            1.8,
            Qt::SolidLine,
            Qt::RoundCap,
            Qt::RoundJoin
        );

        painter.setPen(
            pen
        );

        painter.setBrush(
            Qt::NoBrush
        );

        switch (iconType)
        {
        case UiIcon::Dashboard:
            painter.drawRoundedRect(
                QRectF(4, 4, 7, 7),
                1.5,
                1.5
            );
            painter.drawRoundedRect(
                QRectF(13, 4, 7, 7),
                1.5,
                1.5
            );
            painter.drawRoundedRect(
                QRectF(4, 13, 7, 7),
                1.5,
                1.5
            );
            painter.drawRoundedRect(
                QRectF(13, 13, 7, 7),
                1.5,
                1.5
            );
            break;

        case UiIcon::Can:
            painter.drawLine(
                QPointF(3, 12),
                QPointF(7, 12)
            );
            painter.drawLine(
                QPointF(7, 12),
                QPointF(9, 7)
            );
            painter.drawLine(
                QPointF(9, 7),
                QPointF(12, 17)
            );
            painter.drawLine(
                QPointF(12, 17),
                QPointF(15, 9)
            );
            painter.drawLine(
                QPointF(15, 9),
                QPointF(17, 12)
            );
            painter.drawLine(
                QPointF(17, 12),
                QPointF(21, 12)
            );
            break;

        case UiIcon::Diagnostics:
            painter.drawEllipse(
                QRectF(7, 4, 10, 10)
            );
            painter.drawLine(
                QPointF(12, 14),
                QPointF(12, 20)
            );
            painter.drawLine(
                QPointF(9, 20),
                QPointF(15, 20)
            );
            painter.drawLine(
                QPointF(9, 8),
                QPointF(15, 8)
            );
            painter.drawLine(
                QPointF(12, 5),
                QPointF(12, 11)
            );
            break;

        case UiIcon::Network:
            painter.drawEllipse(
                QRectF(10, 3, 4, 4)
            );
            painter.drawEllipse(
                QRectF(3, 16, 4, 4)
            );
            painter.drawEllipse(
                QRectF(17, 16, 4, 4)
            );
            painter.drawLine(
                QPointF(12, 7),
                QPointF(5, 16)
            );
            painter.drawLine(
                QPointF(12, 7),
                QPointF(19, 16)
            );
            painter.drawLine(
                QPointF(7, 18),
                QPointF(17, 18)
            );
            break;

        case UiIcon::Tests:
            painter.drawRoundedRect(
                QRectF(5, 3, 14, 18),
                2,
                2
            );
            painter.drawLine(
                QPointF(8, 12),
                QPointF(11, 15)
            );
            painter.drawLine(
                QPointF(11, 15),
                QPointF(16, 9)
            );
            break;

        case UiIcon::Acceleration:
            painter.drawLine(
                QPointF(5, 18),
                QPointF(18, 5)
            );
            painter.drawLine(
                QPointF(11, 5),
                QPointF(18, 5)
            );
            painter.drawLine(
                QPointF(18, 5),
                QPointF(18, 12)
            );
            break;

        case UiIcon::Cruise:
            painter.drawArc(
                QRectF(4, 4, 16, 16),
                30 * 16,
                120 * 16
            );
            painter.drawLine(
                QPointF(12, 12),
                QPointF(17, 8)
            );
            painter.drawEllipse(
                QPointF(12, 12),
                1.2,
                1.2
            );
            break;

        case UiIcon::Braking:
            painter.drawEllipse(
                QRectF(5, 5, 14, 14)
            );
            painter.drawEllipse(
                QRectF(9, 9, 6, 6)
            );
            painter.drawLine(
                QPointF(3, 6),
                QPointF(3, 18)
            );
            painter.drawLine(
                QPointF(21, 6),
                QPointF(21, 18)
            );
            break;

        case UiIcon::Recovery:
            painter.drawArc(
                QRectF(4, 4, 16, 16),
                40 * 16,
                250 * 16
            );
            painter.drawLine(
                QPointF(5, 8),
                QPointF(5, 14)
            );
            painter.drawLine(
                QPointF(5, 8),
                QPointF(11, 8)
            );
            break;

        case UiIcon::Ecu:
            painter.drawRect(
                QRectF(6, 6, 12, 12)
            );
            for (int i = 8; i <= 16; i += 4)
            {
                painter.drawLine(
                    QPointF(i, 3),
                    QPointF(i, 6)
                );
                painter.drawLine(
                    QPointF(i, 18),
                    QPointF(i, 21)
                );
                painter.drawLine(
                    QPointF(3, i),
                    QPointF(6, i)
                );
                painter.drawLine(
                    QPointF(18, i),
                    QPointF(21, i)
                );
            }
            break;

        case UiIcon::Health:
            painter.drawRoundedRect(
                QRectF(5, 5, 14, 14),
                4,
                4
            );
            painter.drawLine(
                QPointF(8, 12),
                QPointF(11, 12)
            );
            painter.drawLine(
                QPointF(11, 12),
                QPointF(12.5, 9)
            );
            painter.drawLine(
                QPointF(12.5, 9),
                QPointF(14.5, 15)
            );
            painter.drawLine(
                QPointF(14.5, 15),
                QPointF(16, 12)
            );
            break;
        }

        return QIcon(
            pixmap
        );
    }

    QString formatSimulationTime(
        double milliseconds)
    {
        const qint64 totalMilliseconds =
            static_cast<qint64>(
                milliseconds
                );

        const qint64 hours =
            totalMilliseconds / 3600000;

        const qint64 minutes =
            (totalMilliseconds / 60000) % 60;

        const qint64 seconds =
            (totalMilliseconds / 1000) % 60;

        const qint64 millis =
            totalMilliseconds % 1000;

        return QString("%1:%2:%3.%4")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
            .arg(millis, 3, 10, QChar('0'));
    }

    QWidget* createStatusRow(
        const QString& name,
        QLabel*& valueLabel)
    {
        QFrame* row =
            new QFrame();

        row->setObjectName(
            "StatusRow"
        );

        QHBoxLayout* layout =
            new QHBoxLayout(
                row
            );

        layout->setContentsMargins(
            10,
            7,
            10,
            7
        );

        QLabel* iconLabel =
            new QLabel();

        iconLabel->setObjectName(
            "StatusIcon"
        );

        iconLabel->setFixedSize(
            22,
            22
        );

        const bool isSystemHealth =
            name.contains("CAN") ||
            name.contains("Ethernet") ||
            name.contains("Diagnostics") ||
            name.contains("Drive Mode");

        iconLabel->setPixmap(
            makeUiIcon(
                isSystemHealth
                ? UiIcon::Health
                : UiIcon::Ecu,
                QColor("#7796B1")
            )
            .pixmap(
                18,
                18
            )
        );

        QLabel* nameLabel =
            new QLabel(
                name
            );

        nameLabel->setObjectName(
            "StatusName"
        );

        valueLabel =
            new QLabel(
                "READY"
            );

        valueLabel->setObjectName(
            "StatusBadge"
        );

        valueLabel->setAlignment(
            Qt::AlignRight |
            Qt::AlignVCenter
        );

        valueLabel->setProperty(
            "status",
            "healthy"
        );

        layout->addWidget(
            iconLabel
        );

        layout->addWidget(
            nameLabel
        );

        layout->addStretch();

        layout->addWidget(
            valueLabel
        );

        return row;
    }

    void setStatus(
        QLabel* label,
        const QString& text,
        const char* status)
    {
        if (label == nullptr)
        {
            return;
        }

        label->setText(
            QString("●  ") +
            text
        );

        label->setProperty(
            "status",
            status
        );

        label->style()->unpolish(
            label
        );

        label->style()->polish(
            label
        );

        label->update();
    }
}

MainWindow::MainWindow(
    QWidget* parent)
    : QMainWindow(parent),
    engine(LogLevel::Quiet)
{
    setWindowTitle(
        "Virtual Vehicle Platform"
    );

    resize(
        1500,
        900
    );

    setMinimumSize(
        1180,
        720
    );

    setupUi();
    setupConnections();
    applyTheme();

    engine.startAutomaticDriveCycle();

    // ==================================================
    // Simulation timer
    //
    // The backend continues to advance at approximately
    // 60 Hz. UI rendering is intentionally decoupled so
    // large trace tables are not rebuilt every 16 ms.
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

    // ==================================================
    // UI refresh timer
    //
    // 10 Hz is responsive for telemetry while avoiding
    // unnecessary table/layout work.
    // ==================================================

    uiRefreshTimer.setInterval(
        100
    );

    connect(
        &uiRefreshTimer,
        &QTimer::timeout,
        this,
        [this]()
        {
            refreshUi();
        }
    );

    simulationTimer.start();
    uiRefreshTimer.start();

    refreshUi();
}

void MainWindow::setupUi()
{
    QWidget* centralWidget =
        new QWidget(this);

    centralWidget->setObjectName(
        "AppRoot"
    );

    setCentralWidget(
        centralWidget
    );

    QVBoxLayout* rootLayout =
        new QVBoxLayout(
            centralWidget
        );

    rootLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    rootLayout->setSpacing(
        0
    );

    rootLayout->addWidget(
        createHeader()
    );

    QWidget* contentWidget =
        new QWidget();

    contentWidget->setObjectName(
        "ContentArea"
    );

    QHBoxLayout* contentLayout =
        new QHBoxLayout(
            contentWidget
        );

    contentLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    contentLayout->setSpacing(
        0
    );

    sidebarWidget =
        createSidebar();

    contentLayout->addWidget(
        sidebarWidget
    );

    pageStack =
        new QStackedWidget();

    pageStack->setObjectName(
        "PageStack"
    );

    pageStack->addWidget(
        createDashboardPage()
    );

    pageStack->addWidget(
        createCanMonitorPage()
    );

    pageStack->addWidget(
        createDiagnosticsPage()
    );

    pageStack->addWidget(
        createNetworkPage()
    );

    pageStack->addWidget(
        createTestRunnerPage()
    );

    contentLayout->addWidget(
        pageStack,
        1
    );

    rootLayout->addWidget(
        contentWidget,
        1
    );

    showPage(
        0
    );
}

QWidget* MainWindow::createHeader()
{
    QWidget* headerWidget =
        new QWidget();

    headerWidget->setObjectName(
        "TopHeader"
    );

    headerWidget->setFixedHeight(
        92
    );

    QHBoxLayout* headerLayout =
        new QHBoxLayout(
            headerWidget
        );

    headerLayout->setContentsMargins(
        28,
        14,
        28,
        14
    );

    headerLayout->setSpacing(
        18
    );

    QVBoxLayout* brandLayout =
        new QVBoxLayout();

    brandLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    brandLayout->setSpacing(
        3
    );

    QLabel* titleLabel =
        new QLabel(
            "Virtual Vehicle Platform"
        );

    titleLabel->setObjectName(
        "ApplicationTitle"
    );

    QLabel* subtitleLabel =
        new QLabel(
            "Simulation  |  CAN  |  UDS  |  Ethernet"
        );

    subtitleLabel->setObjectName(
        "ApplicationSubtitle"
    );

    brandLayout->addWidget(
        titleLabel
    );

    brandLayout->addWidget(
        subtitleLabel
    );

    headerLayout->addLayout(
        brandLayout
    );

    headerLayout->addStretch();

    QVBoxLayout* timeLayout =
        new QVBoxLayout();

    timeLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    timeLayout->setSpacing(
        2
    );

    QLabel* timeCaptionLabel =
        new QLabel(
            "Simulation Time"
        );

    timeCaptionLabel->setObjectName(
        "HeaderCaption"
    );

    timeValueLabel =
        new QLabel(
            "0.000 ms"
        );

    timeValueLabel->setObjectName(
        "HeaderValue"
    );

    timeLayout->addWidget(
        timeCaptionLabel
    );

    timeLayout->addWidget(
        timeValueLabel
    );

    headerLayout->addLayout(
        timeLayout
    );

    simulationStatusLabel =
        new QLabel(
            "●  Running"
        );

    simulationStatusLabel->setObjectName(
        "RunningStatus"
    );

    headerLayout->addWidget(
        simulationStatusLabel
    );

    autoDriveButton =
        new QPushButton(
            "Auto Drive"
        );

    autoDriveButton->setObjectName(
        "AutoDriveButton"
    );

    autoDriveButton->setCheckable(
        true
    );

    autoDriveButton->setMinimumWidth(
        110
    );

    pauseResumeButton =
        new QPushButton(
            "Pause"
        );

    pauseResumeButton->setObjectName(
        "SecondaryButton"
    );

    pauseResumeButton->setMinimumWidth(
        110
    );

    resetButton =
        new QPushButton(
            "Reset"
        );

    resetButton->setObjectName(
        "PrimaryButton"
    );

    resetButton->setMinimumWidth(
        110
    );

    QWidget* themeControl =
        new QWidget();

    themeControl->setObjectName(
        "ThemeControl"
    );

    QHBoxLayout* themeControlLayout =
        new QHBoxLayout(
            themeControl
        );

    themeControlLayout->setContentsMargins(
        8,
        0,
        4,
        0
    );

    themeControlLayout->setSpacing(
        7
    );

    themeModeLabel =
        new QLabel(
            "☾  Dark Mode"
        );

    themeModeLabel->setObjectName(
        "ThemeModeLabel"
    );

    themeToggleSwitch =
        new QCheckBox();

    themeToggleSwitch->setObjectName(
        "ThemeSwitch"
    );

    themeToggleSwitch->setChecked(
        false
    );

    themeToggleSwitch->setCursor(
        Qt::PointingHandCursor
    );

    themeControlLayout->addWidget(
        themeModeLabel
    );

    themeControlLayout->addWidget(
        themeToggleSwitch
    );

    headerLayout->addWidget(
        autoDriveButton
    );

    headerLayout->addWidget(
        pauseResumeButton
    );

    headerLayout->addWidget(
        resetButton
    );

    headerLayout->addWidget(
        themeControl
    );

    return headerWidget;
}

QWidget* MainWindow::createSidebar()
{
    QWidget* sidebar =
        new QWidget();

    sidebar->setObjectName(
        "Sidebar"
    );

    sidebar->setFixedWidth(
        190
    );

    QVBoxLayout* sidebarLayout =
        new QVBoxLayout(
            sidebar
        );

    sidebarLayout->setContentsMargins(
        14,
        18,
        14,
        18
    );

    sidebarLayout->setSpacing(
        8
    );

    auto createNavigationButton =
        [sidebar](
            const QString& text,
            UiIcon iconType)
        {
            QPushButton* button =
                new QPushButton(
                    text,
                    sidebar
                );

            button->setIcon(
                makeUiIcon(
                    iconType
                )
            );

            button->setIconSize(
                QSize(
                    20,
                    20
                )
            );

            button->setCheckable(
                true
            );

            button->setAutoExclusive(
                true
            );

            button->setMinimumHeight(
                46
            );

            button->setObjectName(
                "NavigationButton"
            );

            return button;
        };

    dashboardNavigationButton =
        createNavigationButton(
            "Dashboard",
            UiIcon::Dashboard
        );

    canNavigationButton =
        createNavigationButton(
            "CAN Monitor",
            UiIcon::Can
        );

    diagnosticsNavigationButton =
        createNavigationButton(
            "Diagnostics",
            UiIcon::Diagnostics
        );

    networkNavigationButton =
        createNavigationButton(
            "Network",
            UiIcon::Network
        );

    testRunnerNavigationButton =
        createNavigationButton(
            "Test Runner",
            UiIcon::Tests
        );

    sidebarLayout->addWidget(
        dashboardNavigationButton
    );

    sidebarLayout->addWidget(
        canNavigationButton
    );

    sidebarLayout->addWidget(
        diagnosticsNavigationButton
    );

    sidebarLayout->addWidget(
        networkNavigationButton
    );

    sidebarLayout->addWidget(
        testRunnerNavigationButton
    );

    sidebarLayout->addStretch();

    QLabel* versionLabel =
        new QLabel(
            "v1.8.0"
        );

    versionLabel->setObjectName(
        "SidebarVersion"
    );

    sidebarLayout->addWidget(
        versionLabel
    );

    return sidebar;
}

QWidget* MainWindow::createDashboardPage()
{
    QWidget* dashboardPage =
        new QWidget();

    dashboardPage->setObjectName(
        "Page"
    );

    QVBoxLayout* dashboardLayout =
        new QVBoxLayout(
            dashboardPage
        );

    dashboardLayout->setContentsMargins(
        24,
        20,
        24,
        24
    );

    dashboardLayout->setSpacing(
        14
    );

    // ==================================================
    // Title
    // ==================================================

    QLabel* titleLabel =
        new QLabel(
            "Dashboard"
        );

    titleLabel->setObjectName(
        "PageTitle"
    );

    QLabel* subtitleLabel =
        new QLabel(
            "Overview of vehicle state, ECUs and system health"
        );

    subtitleLabel->setObjectName(
        "PageSubtitle"
    );

    dashboardLayout->addWidget(
        titleLabel,
        0
    );

    dashboardLayout->addWidget(
        subtitleLabel,
        0
    );

    // ==================================================
    // Scenario Control
    // ==================================================

    scenarioControlWidget =
        new ScenarioControlWidget();

    scenarioControlWidget
        ->getAccelerationButton()
        ->setText(
            "Acceleration\nIncrease speed"
        );

    scenarioControlWidget
        ->getAccelerationButton()
        ->setIcon(
            makeUiIcon(
                UiIcon::Acceleration,
                QColor("#6F8EAA")
            )
        );

    scenarioControlWidget
        ->getCruiseButton()
        ->setText(
            "Cruise\nMaintain speed"
        );

    scenarioControlWidget
        ->getCruiseButton()
        ->setIcon(
            makeUiIcon(
                UiIcon::Cruise,
                QColor("#6F8EAA")
            )
        );

    scenarioControlWidget
        ->getHardBrakingButton()
        ->setText(
            "Hard Braking\nEmergency braking"
        );

    scenarioControlWidget
        ->getHardBrakingButton()
        ->setIcon(
            makeUiIcon(
                UiIcon::Braking,
                QColor("#6F8EAA")
            )
        );

    scenarioControlWidget
        ->getRecoveryButton()
        ->setText(
            "Recovery / Coast\nNatural deceleration"
        );

    scenarioControlWidget
        ->getRecoveryButton()
        ->setIcon(
            makeUiIcon(
                UiIcon::Recovery,
                QColor("#6F8EAA")
            )
        );

    const std::array<QPushButton*, 4> dashboardScenarioButtons =
    {
        scenarioControlWidget->getAccelerationButton(),
        scenarioControlWidget->getCruiseButton(),
        scenarioControlWidget->getHardBrakingButton(),
        scenarioControlWidget->getRecoveryButton()
    };

    for (QPushButton* button : dashboardScenarioButtons)
    {
        button->setIconSize(
            QSize(
                22,
                22
            )
        );

        button->setMinimumHeight(
            62
        );
    }

    scenarioControlWidget->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Fixed
    );

    scenarioControlWidget->setMinimumHeight(
        96
    );

    dashboardLayout->addWidget(
        scenarioControlWidget,
        0
    );

    // ==================================================
    // Main telemetry row
    // ==================================================

    QWidget* telemetryContainer =
        new QWidget();

    telemetryContainer->setObjectName(
        "TelemetryContainer"
    );

    QHBoxLayout* telemetryLayout =
        new QHBoxLayout(
            telemetryContainer
        );

    telemetryLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    telemetryLayout->setSpacing(
        14
    );

    vehicleStateWidget =
        new VehicleStateWidget();

    absStateWidget =
        new AbsStateWidget();

    QWidget* ecuStatusCard =
        createEcuStatusCard();

    vehicleStateWidget->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
    );

    absStateWidget->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
    );

    ecuStatusCard->setSizePolicy(
        QSizePolicy::Preferred,
        QSizePolicy::Expanding
    );

    vehicleStateWidget->setMinimumHeight(
        365
    );

    absStateWidget->setMinimumHeight(
        365
    );

    ecuStatusCard->setMinimumHeight(
        365
    );

    telemetryLayout->addWidget(
        vehicleStateWidget,
        6
    );

    telemetryLayout->addWidget(
        absStateWidget,
        6
    );

    telemetryLayout->addWidget(
        ecuStatusCard,
        4
    );

    telemetryContainer->setMinimumHeight(
        365
    );

    dashboardLayout->addWidget(
        telemetryContainer,
        5
    );

    // ==================================================
    // Lower information row
    // ==================================================

    QWidget* lowerContainer =
        new QWidget();

    lowerContainer->setObjectName(
        "LowerDashboardContainer"
    );

    QHBoxLayout* lowerLayout =
        new QHBoxLayout(
            lowerContainer
        );

    lowerLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    lowerLayout->setSpacing(
        14
    );

    QWidget* healthCard =
        createSystemHealthCard();

    healthCard->setMinimumWidth(
        320
    );

    healthCard->setMinimumHeight(
        205
    );

    healthCard->setSizePolicy(
        QSizePolicy::Preferred,
        QSizePolicy::Expanding
    );

    eventLogWidget =
        new EventLogWidget();

    eventLogWidget->setObjectName(
        "DashboardEventLog"
    );

    if (
        QTableWidget* eventTable =
        eventLogWidget
        ->findChild<QTableWidget*>()
        )
    {
        eventTable->setAlternatingRowColors(
            true
        );

        eventTable->verticalHeader()
            ->setDefaultSectionSize(
                28
            );

        eventTable->setShowGrid(
            false
        );

        eventTable->setSelectionBehavior(
            QAbstractItemView::SelectRows
        );
    }

    eventLogWidget->setMinimumHeight(
        205
    );

    eventLogWidget->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
    );

    lowerLayout->addWidget(
        healthCard,
        3
    );

    lowerLayout->addWidget(
        eventLogWidget,
        7
    );

    lowerContainer->setMinimumHeight(
        205
    );

    dashboardLayout->addWidget(
        lowerContainer,
        3
    );

    dashboardLayout->setStretchFactor(
        telemetryContainer,
        5
    );

    dashboardLayout->setStretchFactor(
        lowerContainer,
        3
    );

    return dashboardPage;
}

QWidget* MainWindow::createEcuStatusCard()
{
    QGroupBox* card =
        new QGroupBox(
            "ECU Status"
        );

    card->setObjectName(
        "DashboardStatusCard"
    );

    QVBoxLayout* layout =
        new QVBoxLayout(
            card
        );

    layout->setContentsMargins(
        12,
        18,
        12,
        12
    );

    layout->setSpacing(
        6
    );

    layout->addWidget(
        createStatusRow(
            "Powertrain ECU",
            powertrainEcuStatusLabel
        )
    );

    layout->addWidget(
        createStatusRow(
            "ABS ECU",
            absEcuStatusLabel
        )
    );

    layout->addWidget(
        createStatusRow(
            "Steering ECU",
            steeringEcuStatusLabel
        )
    );

    layout->addWidget(
        createStatusRow(
            "Dashboard ECU",
            dashboardEcuStatusLabel
        )
    );

    return card;
}

QWidget* MainWindow::createSystemHealthCard()
{
    QGroupBox* card =
        new QGroupBox(
            "System Health"
        );

    card->setObjectName(
        "DashboardStatusCard"
    );

    QVBoxLayout* layout =
        new QVBoxLayout(
            card
        );

    layout->setContentsMargins(
        12,
        18,
        12,
        12
    );

    layout->setSpacing(
        6
    );

    layout->addWidget(
        createStatusRow(
            "CAN Bus",
            canBusHealthLabel
        )
    );

    layout->addWidget(
        createStatusRow(
            "Automotive Ethernet",
            ethernetHealthLabel
        )
    );

    layout->addWidget(
        createStatusRow(
            "Diagnostics",
            diagnosticsHealthLabel
        )
    );

    layout->addWidget(
        createStatusRow(
            "Drive Mode",
            driveModeHealthLabel
        )
    );

    layout->addStretch();

    return card;
}

QWidget* MainWindow::createCanMonitorPage()
{
    QWidget* canPage =
        new QWidget();

    canPage->setObjectName(
        "Page"
    );

    QVBoxLayout* canLayout =
        new QVBoxLayout(
            canPage
        );

    canLayout->setContentsMargins(
        24,
        20,
        24,
        24
    );

    canLayout->setSpacing(
        12
    );

    QLabel* titleLabel =
        new QLabel(
            "CAN Monitor"
        );

    titleLabel->setObjectName(
        "PageTitle"
    );

    QLabel* subtitleLabel =
        new QLabel(
            "Live CAN traffic, arbitration timing, identifiers and payload data"
        );

    subtitleLabel->setObjectName(
        "PageSubtitle"
    );

    canLayout->addWidget(
        titleLabel
    );

    canLayout->addWidget(
        subtitleLabel
    );

    canMonitorWidget =
        new CanMonitorWidget();

    canLayout->addWidget(
        canMonitorWidget,
        1
    );

    return canPage;
}

QWidget* MainWindow::createDiagnosticsPage()
{
    QWidget* diagnosticsPage =
        new QWidget();

    diagnosticsPage->setObjectName(
        "Page"
    );

    QVBoxLayout* diagnosticsLayout =
        new QVBoxLayout(
            diagnosticsPage
        );

    diagnosticsLayout->setContentsMargins(
        24,
        20,
        24,
        24
    );

    diagnosticsLayout->setSpacing(
        14
    );

    QLabel* titleLabel =
        new QLabel(
            "Diagnostics"
        );

    titleLabel->setObjectName(
        "PageTitle"
    );

    QLabel* subtitleLabel =
        new QLabel(
            "DTC management, fault injection and UDS communication"
        );

    subtitleLabel->setObjectName(
        "PageSubtitle"
    );

    diagnosticsLayout->addWidget(
        titleLabel
    );

    diagnosticsLayout->addWidget(
        subtitleLabel
    );

    faultInjectionWidget =
        new FaultInjectionWidget();

    faultInjectionWidget->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
    );

    diagnosticsLayout->addWidget(
        faultInjectionWidget,
        1
    );

    return diagnosticsPage;
}

QWidget* MainWindow::createNetworkPage()
{
    QWidget* networkPage =
        new QWidget();

    networkPage->setObjectName(
        "Page"
    );

    QVBoxLayout* networkLayout =
        new QVBoxLayout(
            networkPage
        );

    networkLayout->setContentsMargins(
        24,
        20,
        24,
        24
    );

    networkLayout->setSpacing(
        16
    );

    QLabel* titleLabel =
        new QLabel(
            "Network"
        );

    titleLabel->setObjectName(
        "PageTitle"
    );

    QLabel* subtitleLabel =
        new QLabel(
            "Automotive Ethernet network topology and traffic monitoring"
        );

    subtitleLabel->setObjectName(
        "PageSubtitle"
    );

    networkLayout->addWidget(
        titleLabel
    );

    networkLayout->addWidget(
        subtitleLabel
    );

    statisticsWidget =
        new StatisticsWidget();

    networkLayout->addWidget(
        statisticsWidget,
        1
    );

    return networkPage;
}

QWidget* MainWindow::createTestRunnerPage()
{
    QWidget* testRunnerPage =
        new QWidget();

    testRunnerPage->setObjectName(
        "Page"
    );

    QVBoxLayout* testRunnerLayout =
        new QVBoxLayout(
            testRunnerPage
        );

    testRunnerLayout->setContentsMargins(
        24,
        20,
        24,
        24
    );

    testRunnerLayout->setSpacing(
        16
    );

    QLabel* titleLabel =
        new QLabel(
            "Test Runner"
        );

    titleLabel->setObjectName(
        "PageTitle"
    );

    QLabel* subtitleLabel =
        new QLabel(
            "Requirements-based automated verification"
        );

    subtitleLabel->setObjectName(
        "PageSubtitle"
    );

    testRunnerLayout->addWidget(
        titleLabel
    );

    testRunnerLayout->addWidget(
        subtitleLabel
    );

    testRunnerWidget =
        new TestRunnerWidget();

    testRunnerLayout->addWidget(
        testRunnerWidget,
        1
    );

    return testRunnerPage;
}

void MainWindow::setupConnections()
{
    const std::array<QPushButton*, 4> scenarioButtons =
    {
        scenarioControlWidget->getAccelerationButton(),
        scenarioControlWidget->getCruiseButton(),
        scenarioControlWidget->getHardBrakingButton(),
        scenarioControlWidget->getRecoveryButton()
    };

    for (QPushButton* button : scenarioButtons)
    {
        button->setObjectName(
            "ScenarioButton"
        );

        button->setCheckable(
            true
        );

        button->setAutoExclusive(
            true
        );
    }

    connect(
        autoDriveButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.startAutomaticDriveCycle();

            const std::array<QPushButton*, 4> scenarioButtons =
            {
                scenarioControlWidget->getAccelerationButton(),
                scenarioControlWidget->getCruiseButton(),
                scenarioControlWidget->getHardBrakingButton(),
                scenarioControlWidget->getRecoveryButton()
            };

            for (QPushButton* button : scenarioButtons)
            {
                button->setAutoExclusive(
                    false
                );

                button->setChecked(
                    false
                );
            }

            for (QPushButton* button : scenarioButtons)
            {
                button->setAutoExclusive(
                    true
                );
            }

            refreshUi();
        }
    );

    connect(
        pauseResumeButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            togglePauseResume();
        }
    );

    connect(
        resetButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            resetSimulation();
        }
    );

    connect(
        themeToggleSwitch,
        &QCheckBox::toggled,
        this,
        [this](bool checked)
        {
            darkModeEnabled =
                checked;

            applyTheme();
        }
    );

    connect(
        dashboardNavigationButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            showPage(0);
        }
    );

    connect(
        canNavigationButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            showPage(1);
        }
    );

    connect(
        diagnosticsNavigationButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            showPage(2);
        }
    );

    connect(
        networkNavigationButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            showPage(3);
        }
    );

    connect(
        testRunnerNavigationButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            showPage(4);
        }
    );

    connect(
        scenarioControlWidget->getAccelerationButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.startScenario(
                DrivingScenario::Acceleration
            );

            scenarioControlWidget
                ->getAccelerationButton()
                ->setChecked(true);
        }
    );

    connect(
        scenarioControlWidget->getCruiseButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.startScenario(
                DrivingScenario::Cruise
            );

            scenarioControlWidget
                ->getCruiseButton()
                ->setChecked(true);
        }
    );

    connect(
        scenarioControlWidget->getHardBrakingButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.startScenario(
                DrivingScenario::HardBraking
            );

            scenarioControlWidget
                ->getHardBrakingButton()
                ->setChecked(true);
        }
    );

    connect(
        scenarioControlWidget->getRecoveryButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.startScenario(
                DrivingScenario::Recovery
            );

            scenarioControlWidget
                ->getRecoveryButton()
                ->setChecked(true);
        }
    );

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

    connect(
        faultInjectionWidget->getClearFrontLeftFaultButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.clearFrontLeftWheelSensorFault();
        }
    );

    connect(
        faultInjectionWidget->getClearFrontRightFaultButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.clearFrontRightWheelSensorFault();
        }
    );

    connect(
        faultInjectionWidget->getAbsCanDropoutButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.injectAbsCanCommunicationFault();
        }
    );

    connect(
        faultInjectionWidget->getClearAbsCanFaultButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.clearAbsCanCommunicationFault();
        }
    );

    connect(
        faultInjectionWidget->getPowertrainCanDropoutButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.injectPowertrainCanCommunicationFault();
        }
    );

    connect(
        faultInjectionWidget->getClearPowertrainCanFaultButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.clearPowertrainCanCommunicationFault();
        }
    );

    connect(
        faultInjectionWidget->getSteeringCanDropoutButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.injectSteeringCanCommunicationFault();
        }
    );

    connect(
        faultInjectionWidget->getClearSteeringCanFaultButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.clearSteeringCanCommunicationFault();
        }
    );

    connect(
        faultInjectionWidget->getEthernetNodeADropoutButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.injectEthernetNodeACommunicationFault();
        }
    );

    connect(
        faultInjectionWidget->getClearEthernetNodeAFaultButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.clearEthernetNodeACommunicationFault();
        }
    );

    connect(
        faultInjectionWidget->getClearAllFaultsButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            engine.clearFrontLeftWheelSensorFault();
            engine.clearFrontRightWheelSensorFault();

            engine.clearAbsCanCommunicationFault();
            engine.clearPowertrainCanCommunicationFault();
            engine.clearSteeringCanCommunicationFault();
            engine.clearEthernetNodeACommunicationFault();
        }
    );

    connect(
        faultInjectionWidget->getSendUdsRequestButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (
                engine.hasPendingUdsTransaction()
                )
            {
                return;
            }

            const UdsRequest request =
                faultInjectionWidget
                ->buildSelectedUdsRequest();

            faultInjectionWidget
                ->showSubmittedRequest(
                    request
                );

            engine.submitUdsRequest(
                request
            );
        }
    );

    // ==================================================
    // Network / Automotive Ethernet
    // ==================================================

    connect(
        statisticsWidget->getSendAToBButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            const MacAddress& nodeA =
                engine
                .getEthernetNodeA()
                .getMacAddress();

            const MacAddress& nodeB =
                engine
                .getEthernetNodeB()
                .getMacAddress();

            engine.submitEthernetFrame(
                nodeA,
                nodeB,
                0x88B5,
                {
                    0x56,
                    0x56,
                    0x41,
                    0x32,
                    0x42
                }
            );
        }
    );

    connect(
        statisticsWidget->getSendBToAButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            const MacAddress& nodeA =
                engine
                .getEthernetNodeA()
                .getMacAddress();

            const MacAddress& nodeB =
                engine
                .getEthernetNodeB()
                .getMacAddress();

            engine.submitEthernetFrame(
                nodeB,
                nodeA,
                0x88B5,
                {
                    0x56,
                    0x56,
                    0x42,
                    0x32,
                    0x41
                }
            );
        }
    );

    connect(
        testRunnerWidget->getRunSelectedButton(),
        &QPushButton::clicked,
        this,
        [this]()
        {
            runSelectedTest();
        }
    );

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

void MainWindow::showPage(
    int pageIndex)
{
    if (pageStack == nullptr)
    {
        return;
    }

    if (pageIndex < 0 ||
        pageIndex >= pageStack->count())
    {
        return;
    }

    pageStack->setCurrentIndex(
        pageIndex
    );

    updateNavigationSelection();
    refreshUi();
}

void MainWindow::updateNavigationSelection()
{
    if (pageStack == nullptr)
    {
        return;
    }

    const int pageIndex =
        pageStack->currentIndex();

    const std::array<QPushButton*, 5> buttons =
    {
        dashboardNavigationButton,
        canNavigationButton,
        diagnosticsNavigationButton,
        networkNavigationButton,
        testRunnerNavigationButton
    };

    for (std::size_t index = 0;
        index < buttons.size();
        ++index)
    {
        if (buttons[index] != nullptr)
        {
            buttons[index]->setChecked(
                static_cast<int>(index) ==
                pageIndex
            );
        }
    }
}

void MainWindow::toggleTheme()
{
    darkModeEnabled =
        !darkModeEnabled;

    if (themeToggleSwitch != nullptr)
    {
        themeToggleSwitch->setChecked(
            darkModeEnabled
        );
    }

    applyTheme();
}

void MainWindow::applyTheme()
{
    if (themeToggleSwitch != nullptr)
    {
        themeToggleSwitch->setChecked(
            darkModeEnabled
        );
    }

    if (themeModeLabel != nullptr)
    {
        themeModeLabel->setText(
            darkModeEnabled
            ? "☾  Dark Mode"
            : "☀  Light Mode"
        );
    }

    if (darkModeEnabled)
    {
        setStyleSheet(
            R"(
                QMainWindow,
                QWidget#AppRoot,
                QWidget#ContentArea,
                QWidget#Page,
                QStackedWidget#PageStack
                {
                    background-color: #10161D;
                    color: #E8EDF3;
                }

                QWidget#TopHeader
                {
                    background-color: #141C25;
                    border-bottom: 1px solid #26313D;
                }

                QWidget#Sidebar
                {
                    background-color: #17212C;
                    border-right: 1px solid #26313D;
                }

                QWidget#TelemetryContainer,
                QWidget#LowerDashboardContainer
                {
                    background-color: transparent;
                }

                QLabel#ApplicationTitle
                {
                    color: #F3F6F9;
                    font-size: 20px;
                    font-weight: 700;
                }

                QLabel#ApplicationSubtitle,
                QLabel#HeaderCaption,
                QLabel#PageSubtitle,
                QLabel#SidebarVersion
                {
                    color: #95A3B3;
                }

                QLabel#HeaderValue
                {
                    color: #F3F6F9;
                    font-size: 15px;
                    font-weight: 700;
                }

                QLabel#RunningStatus
                {
                    color: #42C77A;
                    font-size: 14px;
                    font-weight: 700;
                }

                QLabel#PageTitle
                {
                    color: #F3F6F9;
                    font-size: 28px;
                    font-weight: 700;
                }

                QLabel#PageSubtitle
                {
                    font-size: 14px;
                }

                QPushButton
                {
                    min-height: 34px;
                    padding: 6px 14px;
                    border-radius: 7px;
                    border: 1px solid #334151;
                    background-color: #1D2732;
                    color: #E6ECF2;
                }

                QPushButton:hover
                {
                    background-color: #253241;
                    border-color: #45576A;
                }

                QWidget#ThemeControl
                {
                    background: transparent;
                }

                QLabel#ThemeModeLabel
                {
                    color: #DCE5EE;
                    font-weight: 600;
                }

                QCheckBox#ThemeSwitch
                {
                    spacing: 0px;
                    min-width: 44px;
                    max-width: 44px;
                    min-height: 24px;
                    max-height: 24px;
                }

                QCheckBox#ThemeSwitch::indicator
                {
                    width: 42px;
                    height: 22px;
                    border-radius: 11px;
                    border: 1px solid #536476;
                    background-color: #2A3542;
                }

                QCheckBox#ThemeSwitch::indicator:checked
                {
                    border: 1px solid #5E8AB4;
                    background-color: #456E97;
                }

                QPushButton#AutoDriveButton:checked,
                QPushButton#ScenarioButton:checked
                {
                    background-color: #355D86;
                    border-color: #355D86;
                    color: #FFFFFF;
                    font-weight: 600;
                }

                QPushButton#ScenarioButton
                {
                    text-align: left;
                    padding: 8px 14px;
                    font-size: 13px;
                    border-radius: 8px;
                }

                QPushButton#ScenarioButton:checked
                {
                    background-color: #294F73;
                    border-color: #4F7FA9;
                }

                QWidget#DashboardEventLog QTableWidget
                {
                    border: none;
                    alternate-background-color: #18222C;
                }

                QWidget#DashboardEventLog QTableWidget::item
                {
                    padding-left: 8px;
                    padding-right: 8px;
                }

                QFrame#CanSummaryCard,
                QFrame#CanControlBar,
                QFrame#CanInspectorPanel,
                QFrame#CanInspectorField
                {
                    background-color: #18232D;
                    border: 1px solid #2B3947;
                    border-radius: 8px;
                }

                QLabel#CanSummaryLabel,
                QLabel#CanInspectorLabel,
                QLabel#CanInspectorSubtitle,
                QLabel#CanControlLabel,
                QLabel#CanFrameCount
                {
                    color: #93A4B5;
                }

                QLabel#CanSummaryValue,
                QLabel#CanInspectorValue,
                QLabel#CanSectionTitle
                {
                    color: #EAF0F5;
                    font-weight: 600;
                }

                QPushButton#CanActionButton
                {
                    background-color: #1A2530;
                    border: 1px solid #344453;
                    border-radius: 7px;
                    padding: 7px 12px;
                }

                QPushButton#CanActionButton[active="true"]
                {
                    background-color: #6B4D2B;
                    border-color: #A2743A;
                }

                QPushButton#CanPrimaryButton
                {
                    background-color: #355D86;
                    border: 1px solid #456F99;
                    color: #FFFFFF;
                    border-radius: 7px;
                    padding: 7px 12px;
                    font-weight: 600;
                }

                QTableWidget#CanTraceTable
                {
                    border: 1px solid #2B3947;
                    border-radius: 8px;
                    alternate-background-color: #17212B;
                }

                QLabel#StatusIcon
                {
                    background: transparent;
                }

                QPushButton#PrimaryButton
                {
                    background-color: #355D86;
                    border-color: #355D86;
                    color: white;
                    font-weight: 600;
                }

                QPushButton#NavigationButton
                {
                    text-align: left;
                    padding-left: 14px;
                    border: none;
                    background-color: transparent;
                    color: #C4CED9;
                    font-size: 14px;
                }

                QPushButton#NavigationButton:hover
                {
                    background-color: #202D3A;
                }

                QPushButton#NavigationButton:checked
                {
                    background-color: #29445F;
                    color: #FFFFFF;
                    font-weight: 600;
                    border-left: 3px solid #6A9BC7;
                }

                QGroupBox
                {
                    background-color: #151D26;
                    border: 1px solid #2B3744;
                    border-radius: 9px;
                    margin-top: 10px;
                    padding-top: 14px;
                    color: #E8EDF3;
                    font-weight: 600;
                }


                QFrame#StatusRow
                {
                    background-color: #19232D;
                    border: 1px solid #2B3744;
                    border-radius: 7px;
                }

                QLabel#StatusName
                {
                    color: #AAB6C3;
                    font-size: 13px;
                }

                QLabel#StatusBadge
                {
                    font-size: 13px;
                    font-weight: 700;
                }

                QGroupBox#DashboardStatusCard
                {
                    background-color: #151D26;
                    border: 1px solid #2B3744;
                    border-radius: 9px;
                    margin-top: 10px;
                    padding-top: 14px;
                }

                QFrame#MetricRow,
                QFrame#WheelMetricCard
                {
                    background-color: #19232D;
                    border: 1px solid #2B3744;
                    border-radius: 7px;
                }

                QLabel#MetricIcon
                {
                    color: #7EA6C8;
                }

                QLabel#MetricLabel,
                QLabel#WheelMetricLabel
                {
                    color: #AAB6C3;
                    font-size: 13px;
                }

                QLabel#MetricValue,
                QLabel#WheelMetricValue,
                QLabel#StatusValue
                {
                    color: #F0F4F8;
                    font-size: 15px;
                    font-weight: 700;
                }

                QLabel[status="healthy"]
                {
                    color: #42C77A;
                    font-weight: 700;
                }

                QLabel[status="warning"]
                {
                    color: #D88A48;
                    font-weight: 700;
                }

                QLabel[status="neutral"]
                {
                    color: #AAB6C3;
                    font-weight: 700;
                }

                QLabel[status="normal"]
                {
                    color: #F0F4F8;
                    font-weight: 700;
                }

                QTableWidget,
                QTableView,
                QTreeView,
                QListWidget,
                QPlainTextEdit,
                QTextEdit,
                QLineEdit,
                QComboBox
                {
                    background-color: #151D26;
                    color: #E1E7ED;
                    border: 1px solid #2C3845;
                    selection-background-color: #385E82;
                    selection-color: white;
                    gridline-color: #2A3541;
                }

                QHeaderView::section
                {
                    background-color: #202A35;
                    color: #DDE4EA;
                    border: none;
                    border-right: 1px solid #2D3945;
                    border-bottom: 1px solid #2D3945;
                    padding: 7px;
                }
            )"
        );
    }
    else
    {
        setStyleSheet(
            R"(
                QMainWindow,
                QWidget#AppRoot,
                QWidget#ContentArea,
                QWidget#Page,
                QStackedWidget#PageStack
                {
                    background-color: #F4F6F8;
                    color: #17202B;
                }

                QWidget#TopHeader
                {
                    background-color: #FFFFFF;
                    border-bottom: 1px solid #DCE2E8;
                }

                QWidget#Sidebar
                {
                    background-color: #243447;
                    border-right: 1px solid #1F2D3C;
                }

                QWidget#TelemetryContainer,
                QWidget#LowerDashboardContainer
                {
                    background-color: transparent;
                }

                QLabel#ApplicationTitle
                {
                    color: #17202B;
                    font-size: 20px;
                    font-weight: 700;
                }

                QLabel#ApplicationSubtitle,
                QLabel#HeaderCaption,
                QLabel#PageSubtitle
                {
                    color: #657587;
                }

                QLabel#SidebarVersion
                {
                    color: #A9B8C7;
                }

                QLabel#HeaderValue
                {
                    color: #17202B;
                    font-size: 15px;
                    font-weight: 700;
                }

                QLabel#RunningStatus
                {
                    color: #168846;
                    font-size: 14px;
                    font-weight: 700;
                }

                QLabel#PageTitle
                {
                    color: #17202B;
                    font-size: 28px;
                    font-weight: 700;
                }

                QLabel#PageSubtitle
                {
                    font-size: 14px;
                }

                QPushButton
                {
                    min-height: 34px;
                    padding: 6px 14px;
                    border-radius: 7px;
                    border: 1px solid #CCD6E0;
                    background-color: #FFFFFF;
                    color: #1F2935;
                }

                QPushButton:hover
                {
                    background-color: #F0F4F8;
                    border-color: #B6C4D2;
                }

                QWidget#ThemeControl
                {
                    background: transparent;
                }

                QLabel#ThemeModeLabel
                {
                    color: #17202B;
                    font-weight: 600;
                }

                QCheckBox#ThemeSwitch
                {
                    spacing: 0px;
                    min-width: 44px;
                    max-width: 44px;
                    min-height: 24px;
                    max-height: 24px;
                }

                QCheckBox#ThemeSwitch::indicator
                {
                    width: 42px;
                    height: 22px;
                    border-radius: 11px;
                    border: 1px solid #C7D2DC;
                    background-color: #E1E7ED;
                }

                QCheckBox#ThemeSwitch::indicator:checked
                {
                    border: 1px solid #456E97;
                    background-color: #456E97;
                }

                QPushButton#AutoDriveButton:checked,
                QPushButton#ScenarioButton:checked
                {
                    background-color: #456E97;
                    border-color: #456E97;
                    color: #FFFFFF;
                    font-weight: 600;
                }

                QPushButton#ScenarioButton
                {
                    text-align: left;
                    padding: 8px 14px;
                    font-size: 13px;
                    border-radius: 8px;
                }

                QPushButton#ScenarioButton:checked
                {
                    background-color: #E2EBF4;
                    border-color: #7F9DB8;
                    color: #1C3144;
                }

                QWidget#DashboardEventLog QTableWidget
                {
                    border: none;
                    alternate-background-color: #F5F8FA;
                }

                QWidget#DashboardEventLog QTableWidget::item
                {
                    padding-left: 8px;
                    padding-right: 8px;
                }

                QFrame#CanSummaryCard,
                QFrame#CanControlBar,
                QFrame#CanInspectorPanel,
                QFrame#CanInspectorField
                {
                    background-color: #FFFFFF;
                    border: 1px solid #DCE3EA;
                    border-radius: 8px;
                }

                QLabel#CanSummaryLabel,
                QLabel#CanInspectorLabel,
                QLabel#CanInspectorSubtitle,
                QLabel#CanControlLabel,
                QLabel#CanFrameCount
                {
                    color: #708090;
                }

                QLabel#CanSummaryValue,
                QLabel#CanInspectorValue,
                QLabel#CanSectionTitle
                {
                    color: #17202B;
                    font-weight: 600;
                }

                QPushButton#CanActionButton
                {
                    background-color: #FFFFFF;
                    border: 1px solid #CDD8E2;
                    border-radius: 7px;
                    padding: 7px 12px;
                }

                QPushButton#CanActionButton[active="true"]
                {
                    background-color: #FFF1DF;
                    border-color: #D7A05D;
                }

                QPushButton#CanPrimaryButton
                {
                    background-color: #456E97;
                    border: 1px solid #456E97;
                    color: #FFFFFF;
                    border-radius: 7px;
                    padding: 7px 12px;
                    font-weight: 600;
                }

                QTableWidget#CanTraceTable
                {
                    border: 1px solid #DCE3EA;
                    border-radius: 8px;
                    alternate-background-color: #F6F8FA;
                }

                QLabel#StatusIcon
                {
                    background: transparent;
                }

                QPushButton#PrimaryButton
                {
                    background-color: #355D86;
                    border-color: #355D86;
                    color: white;
                    font-weight: 600;
                }

                QPushButton#NavigationButton
                {
                    text-align: left;
                    padding-left: 14px;
                    border: none;
                    background-color: transparent;
                    color: #D6E0E9;
                    font-size: 14px;
                }

                QPushButton#NavigationButton:hover
                {
                    background-color: #2D4054;
                }

                QPushButton#NavigationButton:checked
                {
                    background-color: #3A5878;
                    color: #FFFFFF;
                    font-weight: 600;
                    border-left: 3px solid #A9C3DE;
                }

                QGroupBox
                {
                    background-color: #FFFFFF;
                    border: 1px solid #DCE3EA;
                    border-radius: 9px;
                    margin-top: 10px;
                    padding-top: 14px;
                    color: #1B2733;
                    font-weight: 600;
                }


                QFrame#StatusRow
                {
                    background-color: #F8FAFC;
                    border: 1px solid #E0E6EC;
                    border-radius: 7px;
                }

                QLabel#StatusName
                {
                    color: #637282;
                    font-size: 13px;
                }

                QLabel#StatusBadge
                {
                    font-size: 13px;
                    font-weight: 700;
                }

                QGroupBox#DashboardStatusCard
                {
                    background-color: #FFFFFF;
                    border: 1px solid #DCE3EA;
                    border-radius: 9px;
                    margin-top: 10px;
                    padding-top: 14px;
                }

                QFrame#MetricRow,
                QFrame#WheelMetricCard
                {
                    background-color: #F8FAFC;
                    border: 1px solid #E0E6EC;
                    border-radius: 7px;
                }

                QLabel#MetricIcon
                {
                    color: #4F769A;
                }

                QLabel#MetricLabel,
                QLabel#WheelMetricLabel
                {
                    color: #637282;
                    font-size: 13px;
                }

                QLabel#MetricValue,
                QLabel#WheelMetricValue,
                QLabel#StatusValue
                {
                    color: #17202B;
                    font-size: 15px;
                    font-weight: 700;
                }

                QLabel[status="healthy"]
                {
                    color: #168846;
                    font-weight: 700;
                }

                QLabel[status="warning"]
                {
                    color: #C46A2A;
                    font-weight: 700;
                }

                QLabel[status="neutral"]
                {
                    color: #657587;
                    font-weight: 700;
                }

                QLabel[status="normal"]
                {
                    color: #17202B;
                    font-weight: 700;
                }

                QTableWidget,
                QTableView,
                QTreeView,
                QListWidget,
                QPlainTextEdit,
                QTextEdit,
                QLineEdit,
                QComboBox
                {
                    background-color: #FFFFFF;
                    color: #1E2935;
                    border: 1px solid #D8E0E8;
                    selection-background-color: #DCEAF7;
                    selection-color: #17202B;
                    gridline-color: #E2E7EC;
                }

                QHeaderView::section
                {
                    background-color: #EEF2F6;
                    color: #263340;
                    border: none;
                    border-right: 1px solid #DDE3E9;
                    border-bottom: 1px solid #DDE3E9;
                    padding: 7px;
                }
            )"
        );
    }

    if (statisticsWidget != nullptr)
    {
        statisticsWidget->setDarkMode(
            darkModeEnabled
        );
    }

    if (faultInjectionWidget != nullptr)
    {
        faultInjectionWidget->setDarkMode(
            darkModeEnabled
        );
    }

    if (testRunnerWidget != nullptr)
    {
        testRunnerWidget->setDarkMode(
            darkModeEnabled
        );
    }
}

void MainWindow::updateSimulation()
{
    if (!simulationPaused)
    {
        engine.update(
            16.0
        );
    }
}

void MainWindow::togglePauseResume()
{
    simulationPaused =
        !simulationPaused;

    if (simulationPaused)
    {
        simulationStatusLabel->setText(
            "●  Paused"
        );

        simulationStatusLabel->setStyleSheet(
            "color: #C78A2C;"
        );

        pauseResumeButton->setText(
            "Resume"
        );
    }
    else
    {
        simulationStatusLabel->setText(
            "●  Running"
        );

        simulationStatusLabel->setStyleSheet(
            ""
        );

        pauseResumeButton->setText(
            "Pause"
        );
    }
}

void MainWindow::resetSimulation()
{
    engine.reset();

    eventLogWidget->clear();
    canMonitorWidget->clear();
    faultInjectionWidget->clearRuntimeView();

    simulationPaused =
        false;

    simulationStatusLabel->setText(
        "●  Running"
    );

    simulationStatusLabel->setStyleSheet(
        ""
    );

    pauseResumeButton->setText(
        "Pause"
    );

    engine.startAutomaticDriveCycle();

    refreshUi();
}

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

void MainWindow::runAllTests()
{
    TestRunner runner;

    const std::vector<TestCase> results =
        runner.runAll();

    testRunnerWidget->setResults(
        results
    );
}

void MainWindow::refreshUi()
{
    // ==================================================
    // Header is always visible.
    // ==================================================

    timeValueLabel->setText(
        formatSimulationTime(
            engine.getCurrentTimeMs()
        )
    );

    if (pageStack == nullptr)
    {
        return;
    }

    // ==================================================
    // Refresh only the visible page.
    //
    // CAN trace and statistics can become large during a
    // long run. Avoid rebuilding hidden views.
    // ==================================================

    switch (pageStack->currentIndex())
    {
    case 0:
    {
        vehicleStateWidget->updateState(
            engine.getVehicleState()
        );

        absStateWidget->updateState(
            engine.getAbsState(),
            engine.getVehicleState().brakeApplied,
            engine.getVehicleModel().getThrottlePercent(),
            engine.getVehicleModel().isCoasting(),
            engine.getVehicleState().speedKmh
        );

        eventLogWidget->updateEvents(
            engine.getEvents()
        );

        setStatus(
            powertrainEcuStatusLabel,
            "RUNNING",
            "healthy"
        );

        const bool absHealthy =
            engine.getAbsState().healthStatus ==
            AbsHealthStatus::Healthy;

        setStatus(
            absEcuStatusLabel,
            absHealthy
            ? "HEALTHY"
            : "DEGRADED",
            absHealthy
            ? "healthy"
            : "warning"
        );

        setStatus(
            steeringEcuStatusLabel,
            "RUNNING",
            "healthy"
        );

        setStatus(
            dashboardEcuStatusLabel,
            "RUNNING",
            "healthy"
        );

        setStatus(
            canBusHealthLabel,
            QString::number(
                engine.getCanBitrate() /
                1000
            ) +
            " kbps",
            "healthy"
        );

        const bool ethernetReady =
            engine.getEthernetBus()
            .getRegisteredNodeCount() >
            0;

        setStatus(
            ethernetHealthLabel,
            ethernetReady
            ? QString::number(
                engine.getEthernetBus()
                .getLinkRate() /
                1000000
            ) +
            " Mbps"
            : "OFFLINE",
            ethernetReady
            ? "healthy"
            : "warning"
        );

        const bool udsBusy =
            engine.hasPendingUdsTransaction();

        setStatus(
            diagnosticsHealthLabel,
            udsBusy
            ? "BUSY"
            : "READY",
            udsBusy
            ? "neutral"
            : "healthy"
        );

        setStatus(
            driveModeHealthLabel,
            engine.isAutomaticDriveCycleEnabled()
            ? "AUTO"
            : "MANUAL",
            engine.isAutomaticDriveCycleEnabled()
            ? "healthy"
            : "neutral"
        );

        autoDriveButton->setChecked(
            engine.isAutomaticDriveCycleEnabled()
        );

        autoDriveButton->setText(
            engine.isAutomaticDriveCycleEnabled()
            ? "Auto Active"
            : "Auto Drive"
        );

        break;
    }

    case 1:
        canMonitorWidget->updateTrace(
            engine.getCanTrace(),
            engine.getCanStatistics(),
            engine.getCanBitrate(),
            engine.getCurrentTimeMs()
        );

        break;

    case 2:
    {
        faultInjectionWidget->updateDiagnostics(
            engine.getDtcManager(),
            engine.getUdsServer().getCurrentSession(),
            engine.hasPendingUdsTransaction()
        );

        if (
            engine.hasCompletedUdsResponse()
            )
        {
            faultInjectionWidget
                ->showUdsResponse(
                    engine.getCompletedUdsResponse()
                );

            engine.clearCompletedUdsResponse();

            faultInjectionWidget->updateDiagnostics(
                engine.getDtcManager(),
                engine.getUdsServer().getCurrentSession(),
                false
            );
        }

        break;
    }

    case 3:
        statisticsWidget->updateStatistics(
            engine.getCanStatistics(),
            engine.getCanBitrate(),
            engine.getCurrentTimeMs(),
            engine.getEthernetBus(),
            engine.getEthernetNodeA(),
            engine.getEthernetNodeB(),
            engine.isEthernetNodeACommunicationFaultActive()
        );

        break;

    case 4:
        // Test results are refreshed only when tests run.
        break;

    default:
        break;
    }
}
