#include "TestRunnerWidget.h"

#include <algorithm>
#include <cstddef>
#include <set>

#include <QAbstractItemView>
#include <QComboBox>
#include <QColor>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QVBoxLayout>

namespace
{
    QFrame* createSummaryCard(
        const QString& title,
        QLabel*& valueLabel)
    {
        QFrame* card =
            new QFrame();

        card->setObjectName(
            "VerificationSummaryCard"
        );

        QVBoxLayout* layout =
            new QVBoxLayout(
                card
            );

        layout->setContentsMargins(
            12,
            9,
            12,
            9
        );

        layout->setSpacing(
            2
        );

        QLabel* titleLabel =
            new QLabel(
                title
            );

        titleLabel->setObjectName(
            "VerificationSummaryTitle"
        );

        valueLabel =
            new QLabel(
                "--"
            );

        valueLabel->setObjectName(
            "VerificationSummaryValue"
        );

        layout->addWidget(
            titleLabel
        );

        layout->addWidget(
            valueLabel
        );

        return card;
    }

    QString escaped(
        const std::string& value)
    {
        return
            QString::fromStdString(
                value
            )
            .toHtmlEscaped();
    }
}

// ==================================================
// Constructor
// ==================================================

TestRunnerWidget::TestRunnerWidget(
    QWidget* parent)
    : QGroupBox(
        "Verification Workstation",
        parent
    )
{
    setObjectName(
        "VerificationWorkstation"
    );

    QVBoxLayout* mainLayout =
        new QVBoxLayout(
            this
        );

    mainLayout->setContentsMargins(
        14,
        18,
        14,
        14
    );

    mainLayout->setSpacing(
        12
    );

    // ==================================================
    // Verification summary
    // ==================================================

    QHBoxLayout* summaryCardsLayout =
        new QHBoxLayout();

    summaryCardsLayout->setSpacing(
        10
    );

    summaryCardsLayout->addWidget(
        createSummaryCard(
            "Executed Tests",
            executedValueLabel
        )
    );

    summaryCardsLayout->addWidget(
        createSummaryCard(
            "Passed",
            passedValueLabel
        )
    );

    summaryCardsLayout->addWidget(
        createSummaryCard(
            "Failed",
            failedValueLabel
        )
    );

    summaryCardsLayout->addWidget(
        createSummaryCard(
            "Requirements Exercised",
            requirementsValueLabel
        )
    );

    summaryCardsLayout->addWidget(
        createSummaryCard(
            "Pass Rate",
            passRateValueLabel
        )
    );

    summaryCardsLayout->addWidget(
        createSummaryCard(
            "Baseline",
            baselineValueLabel
        )
    );

    baselineValueLabel->setText(
        "v1.8"
    );

    mainLayout->addLayout(
        summaryCardsLayout
    );

    // ==================================================
    // Suite status
    // ==================================================

    QFrame* statusBanner =
        new QFrame();

    statusBanner->setObjectName(
        "VerificationStatusBanner"
    );

    QHBoxLayout* statusLayout =
        new QHBoxLayout(
            statusBanner
        );

    statusLayout->setContentsMargins(
        12,
        8,
        12,
        8
    );

    QLabel* statusTitle =
        new QLabel(
            "Verification Status"
        );

    statusTitle->setObjectName(
        "VerificationCardTitle"
    );

    suiteStatusLabel =
        new QLabel(
            "● NOT RUN"
        );

    suiteStatusLabel->setObjectName(
        "VerificationSuiteStatus"
    );

    statusLayout->addWidget(
        statusTitle
    );

    statusLayout->addStretch();

    statusLayout->addWidget(
        suiteStatusLabel
    );

    mainLayout->addWidget(
        statusBanner
    );

    // ==================================================
    // Test execution controls
    // ==================================================

    QFrame* controlsFrame =
        new QFrame();

    controlsFrame->setObjectName(
        "VerificationControls"
    );

    QHBoxLayout* selectorLayout =
        new QHBoxLayout(
            controlsFrame
        );

    selectorLayout->setContentsMargins(
        12,
        9,
        12,
        9
    );

    selectorLayout->setSpacing(
        10
    );

    QLabel* selectorLabel =
        new QLabel(
            "Test Case"
        );

    selectorLabel->setObjectName(
        "VerificationMutedText"
    );

    testSelector =
        new QComboBox();

    testSelector->setObjectName(
        "VerificationTestSelector"
    );

    // ==================================================
    // System Tests
    // ==================================================

    testSelector->addItem(
        "TC_SYS_001 - Simulation Time Progression",
        "TC_SYS_001"
    );

    testSelector->addItem(
        "TC_SYS_002 - Simulation Reset",
        "TC_SYS_002"
    );

    testSelector->addItem(
        "TC_SYS_003 - ECU Communication Through Virtual CAN",
        "TC_SYS_003"
    );

    testSelector->addItem(
        "TC_SYS_004 - Automated Validation Isolation",
        "TC_SYS_004"
    );

    // ==================================================
    // Vehicle / Scenario Tests
    // ==================================================

    testSelector->addItem(
        "TC_VEH_001 - Acceleration Scenario",
        "TC_VEH_001"
    );

    testSelector->addItem(
        "TC_VEH_002 - Cruise Scenario",
        "TC_VEH_002"
    );

    testSelector->addItem(
        "TC_VEH_003 - Hard Braking Scenario",
        "TC_VEH_003"
    );

    testSelector->addItem(
        "TC_VEH_004 - Recovery Scenario",
        "TC_VEH_004"
    );

    testSelector->addItem(
        "TC_VEH_005 - Vehicle Speed State",
        "TC_VEH_005"
    );

    testSelector->addItem(
        "TC_VEH_006 - Front-Left Wheel Speed State",
        "TC_VEH_006"
    );

    testSelector->addItem(
        "TC_VEH_007 - Front-Right Wheel Speed State",
        "TC_VEH_007"
    );

    testSelector->addItem(
        "TC_VEH_008 - Brake Input State",
        "TC_VEH_008"
    );

    testSelector->addItem(
        "TC_VEH_009 - Steering Angle State",
        "TC_VEH_009"
    );

    testSelector->addItem(
        "TC_VEH_010 - Vehicle Dynamic State Update",
        "TC_VEH_010"
    );

    testSelector->addItem(
        "TC_VEH_011 - Independent Wheel Speeds",
        "TC_VEH_011"
    );

    // ==================================================
    // ABS Tests
    // ==================================================

    testSelector->addItem(
        "TC_ABS_001 - ABS Emergency Braking",
        "TC_ABS_001"
    );

    testSelector->addItem(
        "TC_ABS_002 - Front Left Wheel Sensor Dropout",
        "TC_ABS_002"
    );

    testSelector->addItem(
        "TC_ABS_003 - Front Right Wheel Sensor Dropout",
        "TC_ABS_003"
    );

    testSelector->addItem(
        "TC_ABS_004 - ABS Healthy State",
        "TC_ABS_004"
    );

    testSelector->addItem(
        "TC_ABS_005 - ABS Intervention State Observability",
        "TC_ABS_005"
    );

    testSelector->addItem(
        "TC_ABS_006 - ABS CAN Identifier",
        "TC_ABS_006"
    );

    testSelector->addItem(
        "TC_ABS_007 - ABS Wheel Slip Processing",
        "TC_ABS_007"
    );

    // ==================================================
// Sensor Tests
// ==================================================

    testSelector->addItem(
        "TC_SNS_001 - Front Left Sensor Dropout",
        "TC_SNS_001"
    );

    testSelector->addItem(
        "TC_SNS_002 - Front Right Sensor Dropout",
        "TC_SNS_002"
    );

    testSelector->addItem(
        "TC_SNS_003 - Sensor Fault Recovery",
        "TC_SNS_003"
    );

    testSelector->addItem(
        "TC_SNS_004 - Front-Left Wheel-Speed Measurement",
        "TC_SNS_004"
    );

    testSelector->addItem(
        "TC_SNS_005 - Front-Right Wheel-Speed Measurement",
        "TC_SNS_005"
    );

    testSelector->addItem(
        "TC_SNS_006 - Front-Left Dropout Injection",
        "TC_SNS_006"
    );


    // ==================================================
    // Powertrain Tests
    // ==================================================

    testSelector->addItem(
        "TC_PT_001 - Powertrain CAN Identifier",
        "TC_PT_001"
    );

    testSelector->addItem(
        "TC_PT_002 - Powertrain Codec Roundtrip",
        "TC_PT_002"
    );

    testSelector->addItem(
        "TC_PT_003 - Powertrain State",
        "TC_PT_003"
    );

    testSelector->addItem(
        "TC_PT_004 - Powertrain Vehicle Speed",
        "TC_PT_004"
    );

    testSelector->addItem(
        "TC_PT_005 - Powertrain Engine Speed",
        "TC_PT_005"
    );

    testSelector->addItem(
        "TC_PT_006 - Powertrain Gear State",
        "TC_PT_006"
    );

    testSelector->addItem(
        "TC_PT_007 - Powertrain Brake State",
        "TC_PT_007"
    );

    // ==================================================
    // Steering Tests
    // ==================================================

    testSelector->addItem(
        "TC_STR_001 - Steering CAN Identifier",
        "TC_STR_001"
    );

    testSelector->addItem(
        "TC_STR_002 - Steering Codec Roundtrip",
        "TC_STR_002"
    );

    testSelector->addItem(
        "TC_STR_003 - Steering State",
        "TC_STR_003"
    );

    testSelector->addItem(
        "TC_STR_004 - Steering Angle",
        "TC_STR_004"
    );

    // ==================================================
    // CAN Tests
    // ==================================================

    testSelector->addItem(
        "TC_DIA_001 - Front-Left Diagnostic Fault Injection",
        "TC_DIA_001"
    );

    testSelector->addItem(
        "TC_DIA_002 - Front-Right Diagnostic Fault Injection",
        "TC_DIA_002"
    );

    testSelector->addItem(
        "TC_DIA_003 - Front-Left Diagnostic Fault Clearing",
        "TC_DIA_003"
    );

    testSelector->addItem(
        "TC_DIA_004 - Front-Right Diagnostic Fault Clearing",
        "TC_DIA_004"
    );

    testSelector->addItem(
        "TC_DIA_005 - ECU Health Observability",
        "TC_DIA_005"
    );

    testSelector->addItem(
        "TC_DIA_006 - Runtime Fault Injection",
        "TC_DIA_006"
    );

    testSelector->addItem(
        "TC_DIA_007 - Runtime Fault Recovery",
        "TC_DIA_007"
    );

    testSelector->addItem(
        "TC_CAN_001 - CAN Arbitration Priority",
        "TC_CAN_001"
    );

    testSelector->addItem(
        "TC_CAN_002 - CAN Bitrate",
        "TC_CAN_002"
    );

    testSelector->addItem(
        "TC_CAN_003 - CAN Trace Recording",
        "TC_CAN_003"
    );

    testSelector->addItem(
        "TC_CAN_004 - CAN Arbitration Statistics",
        "TC_CAN_004"
    );

    testSelector->addItem(
        "TC_CAN_005 - ABS Message Periodicity",
        "TC_CAN_005"
    );

    testSelector->addItem(
        "TC_CAN_006 - Powertrain Message Periodicity",
        "TC_CAN_006"
    );

    testSelector->addItem(
        "TC_CAN_007 - Steering Message Periodicity",
        "TC_CAN_007"
    );

    testSelector->addItem(
        "TC_CAN_008 - CAN Bus Utilization",
        "TC_CAN_008"
    );

    testSelector->addItem(
        "TC_CAN_009 - CAN Trace Timing Evidence",
        "TC_CAN_009"
    );

    testSelector->addItem(
        "TC_CAN_010 - CAN Transmission Duration",
        "TC_CAN_010"
    );

    testSelector->addItem(
        "TC_CAN_011 - CAN Waiting Time",
        "TC_CAN_011"
    );

    testSelector->addItem(
        "TC_CAN_012 - CAN Frame Count",
        "TC_CAN_012"
    );

    testSelector->addItem(
        "TC_CAN_013 - CAN Average Waiting Time",
        "TC_CAN_013"
    );

    testSelector->addItem(
        "TC_CAN_014 - CAN Maximum Waiting Time",
        "TC_CAN_014"
    );

    testSelector->addItem(
        "TC_CAN_015 - CAN Total Transmission Time",
        "TC_CAN_015"
    );

    testSelector->addItem(
        "TC_CAN_016 - CAN Per-Message Statistics",
        "TC_CAN_016"
    );

    testSelector->addItem(
        "TC_CAN_017 - CAN Pending Frame Behavior",
        "TC_CAN_017"
    );

    testSelector->addItem(
        "TC_CAN_018 - CAN Waiting-Time Trace Evidence",
        "TC_CAN_018"
    );

    testSelector->addItem(
        "TC_CAN_019 - CAN Exclusive Bus Access",
        "TC_CAN_019"
    );

    testSelector->addItem(
        "TC_CAN_020 - CAN Trace Identifier Evidence",
        "TC_CAN_020"
    );

    testSelector->addItem(
        "TC_CAN_021 - CAN Trace Payload Evidence",
        "TC_CAN_021"
    );

    testSelector->addItem(
        "TC_CAN_022 - Multiple ECU Transmission Requests",
        "TC_CAN_022"
    );

    // ==================================================
    // Message Tests
    // ==================================================

    testSelector->addItem(
        "TC_MSG_001 - ABS Codec Roundtrip",
        "TC_MSG_001"
    );

    testSelector->addItem(
        "TC_MSG_002 - ABS Message CAN Identifier",
        "TC_MSG_002"
    );

    testSelector->addItem(
        "TC_MSG_003 - ABS Message State Information",
        "TC_MSG_003"
    );

    testSelector->addItem(
        "TC_MSG_004 - Powertrain Message CAN Identifier",
        "TC_MSG_004"
    );

    testSelector->addItem(
        "TC_MSG_005 - Powertrain Vehicle-Speed Information",
        "TC_MSG_005"
    );

    testSelector->addItem(
        "TC_MSG_006 - Powertrain Engine-RPM Information",
        "TC_MSG_006"
    );

    testSelector->addItem(
        "TC_MSG_007 - Powertrain Gear-State Information",
        "TC_MSG_007"
    );

    testSelector->addItem(
        "TC_MSG_008 - Powertrain Brake-State Information",
        "TC_MSG_008"
    );

    testSelector->addItem(
        "TC_MSG_009 - Steering Message CAN Identifier",
        "TC_MSG_009"
    );

    testSelector->addItem(
        "TC_MSG_010 - Steering-Angle Information",
        "TC_MSG_010"
    );

    testSelector->addItem(
        "TC_VAL_001 - Individual Test Execution",
        "TC_VAL_001"
    );

    testSelector->addItem(
        "TC_VAL_002 - Validation Test Isolation",
        "TC_VAL_002"
    );

    testSelector->addItem(
        "TC_VAL_003 - Test Status",
        "TC_VAL_003"
    );

    testSelector->addItem(
        "TC_VAL_004 - Execution-Time Evidence",
        "TC_VAL_004"
    );

    testSelector->addItem(
        "TC_VAL_005 - Response-Time Evidence",
        "TC_VAL_005"
    );

    testSelector->addItem(
        "TC_VAL_006 - Timing Pass Criteria",
        "TC_VAL_006"
    );


    // ==================================================
    // Buttons
    // ==================================================

    runSelectedButton =
        new QPushButton(
            "Run Selected"
        );

    runSelectedButton->setObjectName(
        "VerificationSecondaryButton"
    );

    runAllButton =
        new QPushButton(
            "Run All Tests"
        );

    runAllButton->setObjectName(
        "VerificationPrimaryButton"
    );

    selectorLayout->addWidget(
        selectorLabel
    );

    selectorLayout->addWidget(
        testSelector,
        1
    );

    selectorLayout->addWidget(
        runSelectedButton
    );

    selectorLayout->addWidget(
        runAllButton
    );

    mainLayout->addWidget(
        controlsFrame
    );

    // ==================================================
    // Main verification area
    // ==================================================

    QSplitter* verificationSplitter =
        new QSplitter(
            Qt::Vertical
        );

    verificationSplitter->setObjectName(
        "VerificationSplitter"
    );

    // ==================================================
    // Results table
    // ==================================================

    QFrame* resultsPanel =
        new QFrame();

    resultsPanel->setObjectName(
        "VerificationPanel"
    );

    QVBoxLayout* resultsLayout =
        new QVBoxLayout(
            resultsPanel
        );

    resultsLayout->setContentsMargins(
        10,
        10,
        10,
        10
    );

    resultsLayout->setSpacing(
        7
    );

    QLabel* resultsTitle =
        new QLabel(
            "Test Results"
        );

    resultsTitle->setObjectName(
        "VerificationPanelTitle"
    );

    resultsLayout->addWidget(
        resultsTitle
    );

    resultsTable =
        new QTableWidget(
            0,
            8
        );

    resultsTable->setObjectName(
        "VerificationResultsTable"
    );

    resultsTable->setHorizontalHeaderLabels(
        {
            "Test ID",
            "Test",
            "Requirement",
            "Expected",
            "Actual",
            "Response / Limit",
            "Execution",
            "Result"
        }
    );

    resultsTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    resultsTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    resultsTable->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    resultsTable->setAlternatingRowColors(
        true
    );

    resultsTable->setShowGrid(
        false
    );

    resultsTable
        ->verticalHeader()
        ->setVisible(
            false
        );

    resultsTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            0,
            QHeaderView::ResizeToContents
        );

    resultsTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            1,
            QHeaderView::Stretch
        );

    resultsTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            2,
            QHeaderView::ResizeToContents
        );

    resultsTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            3,
            QHeaderView::Stretch
        );

    resultsTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            4,
            QHeaderView::Stretch
        );

    resultsTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            5,
            QHeaderView::ResizeToContents
        );

    resultsTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            6,
            QHeaderView::ResizeToContents
        );

    resultsTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            7,
            QHeaderView::ResizeToContents
        );

    resultsLayout->addWidget(
        resultsTable,
        1
    );

    verificationSplitter->addWidget(
        resultsPanel
    );

    // ==================================================
    // Verification evidence
    // ==================================================

    QFrame* evidencePanel =
        new QFrame();

    evidencePanel->setObjectName(
        "VerificationPanel"
    );

    QVBoxLayout* evidenceLayout =
        new QVBoxLayout(
            evidencePanel
        );

    evidenceLayout->setContentsMargins(
        10,
        10,
        10,
        10
    );

    evidenceLayout->setSpacing(
        7
    );

    QLabel* evidenceTitle =
        new QLabel(
            "Verification Evidence"
        );

    evidenceTitle->setObjectName(
        "VerificationPanelTitle"
    );

    evidenceLayout->addWidget(
        evidenceTitle
    );

    detailText =
        new QTextEdit();

    detailText->setObjectName(
        "VerificationEvidence"
    );

    detailText->setReadOnly(
        true
    );

    detailText->setPlaceholderText(
        "Select a test result to inspect requirements-based verification evidence."
    );

    evidenceLayout->addWidget(
        detailText,
        1
    );

    verificationSplitter->addWidget(
        evidencePanel
    );

    verificationSplitter->setStretchFactor(
        0,
        3
    );

    verificationSplitter->setStretchFactor(
        1,
        2
    );

    verificationSplitter->setSizes(
        {
            430,
            250
        }
    );

    mainLayout->addWidget(
        verificationSplitter,
        1
    );

    // ==================================================
    // Result selection
    // ==================================================

    connect(
        resultsTable,
        &QTableWidget::cellClicked,
        this,
        [this](
            int row,
            int)
        {
            showTestDetails(
                row
            );
        }
    );

    applyThemeStyle();
}

// ==================================================
// Buttons
// ==================================================

QPushButton*
TestRunnerWidget::getRunAllButton() const
{
    return runAllButton;
}

QPushButton*
TestRunnerWidget::getRunSelectedButton() const
{
    return runSelectedButton;
}

// ==================================================
// Selected test
// ==================================================

std::string
TestRunnerWidget::getSelectedTestId() const
{
    return testSelector
        ->currentData()
        .toString()
        .toStdString();
}

// ==================================================
// Status
// ==================================================

QString TestRunnerWidget::statusToString(
    TestStatus status) const
{
    switch (status)
    {
    case TestStatus::NotRun:
        return "NOT RUN";

    case TestStatus::Running:
        return "RUNNING";

    case TestStatus::Passed:
        return "PASS";

    case TestStatus::Failed:
        return "FAIL";
    }

    return "UNKNOWN";
}

// ==================================================
// Single result
// ==================================================

void TestRunnerWidget::setSingleResult(
    const TestCase& result)
{
    std::vector<TestCase> results;

    results.push_back(
        result
    );

    setResults(
        results
    );
}

// ==================================================
// Set results
// ==================================================

void TestRunnerWidget::setResults(
    const std::vector<TestCase>& results)
{
    currentResults =
        results;

    synchronizeSelector(
        results
    );

    resultsTable->setRowCount(
        static_cast<int>(
            results.size()
            )
    );

    int row =
        0;

    for (
        const TestCase& test :
        results
        )
    {
        QTableWidgetItem* idItem =
            new QTableWidgetItem(
                QString::fromStdString(
                    test.id
                )
            );

        QTableWidgetItem* nameItem =
            new QTableWidgetItem(
                QString::fromStdString(
                    test.name
                )
            );

        QTableWidgetItem* requirementItem =
            new QTableWidgetItem(
                QString::fromStdString(
                    test.requirementId
                )
            );

        QTableWidgetItem* expectedItem =
            new QTableWidgetItem(
                QString::fromStdString(
                    test.result.expected
                )
            );

        QTableWidgetItem* actualItem =
            new QTableWidgetItem(
                QString::fromStdString(
                    test.result.actual
                )
            );

        QString timingText;

        if (
            test.result.hasTimingRequirement()
            )
        {
            timingText =
                QString::number(
                    test.result.responseTimeMs,
                    'f',
                    3
                ) +
                " / " +
                QString::number(
                    test.result.maximumAllowedResponseTimeMs,
                    'f',
                    3
                ) +
                " ms";
        }
        else
        {
            timingText =
                QString::number(
                    test.result.responseTimeMs,
                    'f',
                    3
                ) +
                " ms";
        }

        QTableWidgetItem* timingItem =
            new QTableWidgetItem(
                timingText
            );

        timingItem->setTextAlignment(
            Qt::AlignCenter
        );

        QTableWidgetItem* executionItem =
            new QTableWidgetItem(
                QString::number(
                    test.result.executionTimeMs,
                    'f',
                    3
                ) +
                " ms"
            );

        executionItem->setTextAlignment(
            Qt::AlignCenter
        );

        QTableWidgetItem* statusItem =
            new QTableWidgetItem(
                statusToString(
                    test.result.status
                )
            );

        statusItem->setTextAlignment(
            Qt::AlignCenter
        );

        QFont statusFont =
            statusItem->font();

        statusFont.setBold(
            true
        );

        statusItem->setFont(
            statusFont
        );

        if (
            test.result.status ==
            TestStatus::Passed
            )
        {
            statusItem->setForeground(
                QColor(
                    "#138A4B"
                )
            );
        }
        else if (
            test.result.status ==
            TestStatus::Failed
            )
        {
            statusItem->setForeground(
                QColor(
                    "#C94B4B"
                )
            );
        }

        resultsTable->setItem(
            row,
            0,
            idItem
        );

        resultsTable->setItem(
            row,
            1,
            nameItem
        );

        resultsTable->setItem(
            row,
            2,
            requirementItem
        );

        resultsTable->setItem(
            row,
            3,
            expectedItem
        );

        resultsTable->setItem(
            row,
            4,
            actualItem
        );

        resultsTable->setItem(
            row,
            5,
            timingItem
        );

        resultsTable->setItem(
            row,
            6,
            executionItem
        );

        resultsTable->setItem(
            row,
            7,
            statusItem
        );

        ++row;
    }

    updateSummary(
        results
    );

    if (!results.empty())
    {
        resultsTable->selectRow(
            0
        );

        showTestDetails(
            0
        );
    }
    else
    {
        detailText->clear();
    }
}

// ==================================================
// Selector synchronization
// ==================================================

void TestRunnerWidget::synchronizeSelector(
    const std::vector<TestCase>& results)
{
    for (
        const TestCase& test :
        results
        )
    {
        bool alreadyPresent =
            false;

        for (
            int index = 0;
            index < testSelector->count();
            ++index
            )
        {
            if (
                testSelector
                ->itemData(
                    index
                )
                .toString() ==
                QString::fromStdString(
                    test.id
                )
                )
            {
                alreadyPresent =
                    true;

                break;
            }
        }

        if (!alreadyPresent)
        {
            testSelector->addItem(
                QString::fromStdString(
                    test.id +
                    " - " +
                    test.name
                ),
                QString::fromStdString(
                    test.id
                )
            );
        }
    }
}

// ==================================================
// Summary
// ==================================================

void TestRunnerWidget::updateSummary(
    const std::vector<TestCase>& results)
{
    std::size_t passed =
        0;

    std::size_t failed =
        0;

    std::set<std::string>
        exercisedRequirements;

    for (
        const TestCase& test :
        results
        )
    {
        if (
            !test.requirementId.empty()
            )
        {
            exercisedRequirements.insert(
                test.requirementId
            );
        }

        if (
            test.result.status ==
            TestStatus::Passed
            )
        {
            ++passed;
        }
        else if (
            test.result.status ==
            TestStatus::Failed
            )
        {
            ++failed;
        }
    }

    const std::size_t executed =
        passed +
        failed;

    const double passRate =
        executed >
        0
        ? (
            static_cast<double>(
                passed
                ) /
            static_cast<double>(
                executed
                )
            ) *
        100.0
        : 0.0;

    executedValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                executed
                )
        )
    );

    passedValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                passed
                )
        )
    );

    failedValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                failed
                )
        )
    );

    requirementsValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                exercisedRequirements.size()
                )
        )
    );

    passRateValueLabel->setText(
        QString::number(
            passRate,
            'f',
            1
        ) +
        " %"
    );

    if (
        results.empty()
        )
    {
        suiteStatusLabel->setText(
            "● NOT RUN"
        );

        refreshDynamicStyle(
            suiteStatusLabel,
            "neutral"
        );

        return;
    }

    if (
        failed >
        0
        )
    {
        suiteStatusLabel->setText(
            QString(
                "● %1 FAILED"
            )
            .arg(
                static_cast<qulonglong>(
                    failed
                    )
            )
        );

        refreshDynamicStyle(
            suiteStatusLabel,
            "failed"
        );

        return;
    }

    if (
        executed ==
        results.size()
        )
    {
        suiteStatusLabel->setText(
            QString(
                "● ALL %1 TESTS PASS"
            )
            .arg(
                static_cast<qulonglong>(
                    passed
                    )
            )
        );

        refreshDynamicStyle(
            suiteStatusLabel,
            "passed"
        );

        return;
    }

    suiteStatusLabel->setText(
        "● PARTIAL EXECUTION"
    );

    refreshDynamicStyle(
        suiteStatusLabel,
        "warning"
    );
}

// ==================================================
// Details
// ==================================================

void TestRunnerWidget::showTestDetails(
    int row)
{
    if (
        row < 0 ||
        row >=
        static_cast<int>(
            currentResults.size()
            )
        )
    {
        return;
    }

    const TestCase& test =
        currentResults[
            static_cast<std::size_t>(
                row
                )
        ];

    QString statusColor =
        "#6C7F92";

    if (
        test.result.status ==
        TestStatus::Passed
        )
    {
        statusColor =
            "#138A4B";
    }
    else if (
        test.result.status ==
        TestStatus::Failed
        )
    {
        statusColor =
            "#C94B4B";
    }

    QString timingEvidence =
        QString::number(
            test.result.responseTimeMs,
            'f',
            3
        ) +
        " ms";

    if (
        test.result.hasTimingRequirement()
        )
    {
        timingEvidence +=
            " / limit " +
            QString::number(
                test.result.maximumAllowedResponseTimeMs,
                'f',
                3
            ) +
            " ms";
    }

    const QString html =
        QString(
            R"(
            <div style="font-family:'Segoe UI';">
                <table width="100%" cellspacing="0" cellpadding="4">
                    <tr>
                        <td width="18%"><b>Test ID</b></td>
                        <td width="32%">%1</td>
                        <td width="18%"><b>Requirement</b></td>
                        <td width="32%">%2</td>
                    </tr>
                    <tr>
                        <td><b>Test</b></td>
                        <td colspan="3">%3</td>
                    </tr>
                    <tr>
                        <td><b>Status</b></td>
                        <td colspan="3">
                            <span style="color:%4; font-weight:700;">%5</span>
                        </td>
                    </tr>
                </table>

                <hr>

                <p><b>Verification Objective</b><br>%6</p>
                <p><b>Precondition</b><br>%7</p>
                <p><b>Stimulus</b><br>%8</p>

                <table width="100%" cellspacing="0" cellpadding="5">
                    <tr>
                        <td width="18%"><b>Expected Result</b></td>
                        <td>%9</td>
                    </tr>
                    <tr>
                        <td><b>Expected Evidence</b></td>
                        <td>%10</td>
                    </tr>
                    <tr>
                        <td><b>Actual Evidence</b></td>
                        <td>%11</td>
                    </tr>
                    <tr>
                        <td><b>Response / Limit</b></td>
                        <td>%12</td>
                    </tr>
                    <tr>
                        <td><b>Execution Time</b></td>
                        <td>%13 ms</td>
                    </tr>
                </table>

                <p><b>Result Message</b><br>%14</p>
            </div>
            )"
        )
        .arg(
            escaped(
                test.id
            )
        )
        .arg(
            escaped(
                test.requirementId
            )
        )
        .arg(
            escaped(
                test.name
            )
        )
        .arg(
            statusColor
        )
        .arg(
            statusToString(
                test.result.status
            )
        )
        .arg(
            escaped(
                test.description
            )
        )
        .arg(
            escaped(
                test.precondition
            )
        )
        .arg(
            escaped(
                test.stimulus
            )
        )
        .arg(
            escaped(
                test.expectedResult
            )
        )
        .arg(
            escaped(
                test.result.expected
            )
        )
        .arg(
            escaped(
                test.result.actual
            )
        )
        .arg(
            timingEvidence.toHtmlEscaped()
        )
        .arg(
            QString::number(
                test.result.executionTimeMs,
                'f',
                3
            )
        )
        .arg(
            escaped(
                test.result.message
            )
        );

    detailText->setHtml(
        html
    );
}

// ==================================================
// Clear
// ==================================================

void TestRunnerWidget::clear()
{
    currentResults.clear();

    resultsTable->setRowCount(
        0
    );

    detailText->clear();

    executedValueLabel->setText(
        "0"
    );

    passedValueLabel->setText(
        "0"
    );

    failedValueLabel->setText(
        "0"
    );

    requirementsValueLabel->setText(
        "0"
    );

    passRateValueLabel->setText(
        "0.0 %"
    );

    suiteStatusLabel->setText(
        "● NOT RUN"
    );

    refreshDynamicStyle(
        suiteStatusLabel,
        "neutral"
    );
}

// ==================================================
// Theme
// ==================================================

void TestRunnerWidget::setDarkMode(
    bool enabled)
{
    darkModeEnabled =
        enabled;

    applyThemeStyle();
}

void TestRunnerWidget::refreshDynamicStyle(
    QLabel* label,
    const char* state)
{
    if (
        label ==
        nullptr
        )
    {
        return;
    }

    label->setProperty(
        "state",
        state
    );

    label->style()->unpolish(
        label
    );

    label->style()->polish(
        label
    );
}

void TestRunnerWidget::applyThemeStyle()
{
    if (darkModeEnabled)
    {
        setStyleSheet(
            R"(
                QGroupBox#VerificationWorkstation
                {
                    background: transparent;
                    color: #E8EDF3;
                }

                QFrame#VerificationSummaryCard,
                QFrame#VerificationStatusBanner,
                QFrame#VerificationControls,
                QFrame#VerificationPanel
                {
                    background-color: #16222E;
                    border: 1px solid #304050;
                    border-radius: 8px;
                }

                QLabel#VerificationSummaryTitle,
                QLabel#VerificationMutedText
                {
                    color: #8EA2B5;
                }

                QLabel#VerificationSummaryValue,
                QLabel#VerificationCardTitle,
                QLabel#VerificationPanelTitle
                {
                    color: #EAF1F7;
                    font-weight: 700;
                }

                QLabel#VerificationSummaryValue
                {
                    font-size: 15px;
                }

                QLabel#VerificationSuiteStatus
                {
                    font-weight: 700;
                }

                QLabel[state="passed"]
                {
                    color: #43D17D;
                }

                QLabel[state="failed"]
                {
                    color: #F07171;
                }

                QLabel[state="warning"]
                {
                    color: #E2A34B;
                }

                QLabel[state="neutral"]
                {
                    color: #9AAABB;
                }

                QComboBox#VerificationTestSelector
                {
                    min-height: 34px;
                    color: #E7EEF5;
                    background-color: #17232E;
                    border: 1px solid #34495A;
                    border-radius: 6px;
                    padding: 3px 8px;
                }

                QComboBox#VerificationTestSelector QAbstractItemView
                {
                    color: #E7EEF5;
                    background-color: #17232E;
                    selection-background-color: #315A7D;
                }

                QPushButton#VerificationPrimaryButton,
                QPushButton#VerificationSecondaryButton
                {
                    min-height: 36px;
                    min-width: 115px;
                    border-radius: 6px;
                    font-weight: 600;
                }

                QPushButton#VerificationPrimaryButton
                {
                    color: #FFFFFF;
                    background-color: #456E97;
                    border: 1px solid #5B86AE;
                }

                QPushButton#VerificationPrimaryButton:hover
                {
                    background-color: #527DA7;
                }

                QPushButton#VerificationSecondaryButton
                {
                    color: #E7EEF5;
                    background-color: #1A2733;
                    border: 1px solid #34495A;
                }

                QPushButton#VerificationSecondaryButton:hover
                {
                    background-color: #233544;
                    border-color: #4B667B;
                }

                QTableWidget#VerificationResultsTable
                {
                    color: #D9E3EC;
                    background-color: #121C25;
                    alternate-background-color: #17232E;
                    border: 1px solid #2F4050;
                    border-radius: 6px;
                    selection-background-color: #315A7D;
                    selection-color: #FFFFFF;
                }

                QHeaderView::section
                {
                    color: #C7D4DF;
                    background-color: #1C2935;
                    border: none;
                    border-bottom: 1px solid #314252;
                    padding: 7px;
                    font-weight: 700;
                }

                QTextEdit#VerificationEvidence
                {
                    color: #D9E3EC;
                    background-color: #121C25;
                    border: 1px solid #2F4050;
                    border-radius: 6px;
                    padding: 6px;
                }

                QSplitter::handle
                {
                    background-color: #263746;
                    height: 4px;
                }
            )"
        );

        return;
    }

    setStyleSheet(
        R"(
            QGroupBox#VerificationWorkstation
            {
                background: transparent;
                color: #17212B;
            }

            QFrame#VerificationSummaryCard,
            QFrame#VerificationStatusBanner,
            QFrame#VerificationControls,
            QFrame#VerificationPanel
            {
                background-color: #FFFFFF;
                border: 1px solid #DCE3EA;
                border-radius: 8px;
            }

            QLabel#VerificationSummaryTitle,
            QLabel#VerificationMutedText
            {
                color: #74879A;
            }

            QLabel#VerificationSummaryValue,
            QLabel#VerificationCardTitle,
            QLabel#VerificationPanelTitle
            {
                color: #17212B;
                font-weight: 700;
            }

            QLabel#VerificationSummaryValue
            {
                font-size: 15px;
            }

            QLabel#VerificationSuiteStatus
            {
                font-weight: 700;
            }

            QLabel[state="passed"]
            {
                color: #138A4B;
            }

            QLabel[state="failed"]
            {
                color: #C94B4B;
            }

            QLabel[state="warning"]
            {
                color: #C58A20;
            }

            QLabel[state="neutral"]
            {
                color: #6C7F92;
            }

            QComboBox#VerificationTestSelector
            {
                min-height: 34px;
                color: #17212B;
                background-color: #FFFFFF;
                border: 1px solid #CBD6E0;
                border-radius: 6px;
                padding: 3px 8px;
            }

            QPushButton#VerificationPrimaryButton,
            QPushButton#VerificationSecondaryButton
            {
                min-height: 36px;
                min-width: 115px;
                border-radius: 6px;
                font-weight: 600;
            }

            QPushButton#VerificationPrimaryButton
            {
                color: #FFFFFF;
                background-color: #456E97;
                border: 1px solid #456E97;
            }

            QPushButton#VerificationPrimaryButton:hover
            {
                background-color: #527DA7;
            }

            QPushButton#VerificationSecondaryButton
            {
                color: #263746;
                background-color: #FFFFFF;
                border: 1px solid #CBD6E0;
            }

            QPushButton#VerificationSecondaryButton:hover
            {
                background-color: #F0F4F8;
            }

            QTableWidget#VerificationResultsTable
            {
                color: #17212B;
                background-color: #FFFFFF;
                alternate-background-color: #F6F8FA;
                border: 1px solid #E0E6EC;
                border-radius: 6px;
                selection-background-color: #D9EAF8;
                selection-color: #17212B;
            }

            QHeaderView::section
            {
                color: #425364;
                background-color: #EEF2F5;
                border: none;
                border-bottom: 1px solid #DCE3EA;
                padding: 7px;
                font-weight: 700;
            }

            QTextEdit#VerificationEvidence
            {
                color: #17212B;
                background-color: #FFFFFF;
                border: 1px solid #E0E6EC;
                border-radius: 6px;
                padding: 6px;
            }

            QSplitter::handle
            {
                background-color: #DDE5EC;
                height: 4px;
            }
        )"
    );
}
