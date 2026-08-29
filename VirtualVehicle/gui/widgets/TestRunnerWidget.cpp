#include "TestRunnerWidget.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QFont>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QVBoxLayout>

// ==================================================
// Constructor
// ==================================================

TestRunnerWidget::TestRunnerWidget(
    QWidget* parent)
    : QGroupBox(
        "Automated Validation",
        parent
    )
{
    QVBoxLayout* mainLayout =
        new QVBoxLayout(this);

    // ==================================================
    // Test selection
    // ==================================================

    QHBoxLayout* selectorLayout =
        new QHBoxLayout();

    QLabel* selectorLabel =
        new QLabel(
            "Test:"
        );

    testSelector =
        new QComboBox();

    // ==================================================
    // System Tests
    // ==================================================

    testSelector->addItem(
        "TC_SYS_001 - Simulation Time Progression",
        "TC_SYS_001"
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

    // ==================================================
    // CAN Tests
    // ==================================================

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

    // ==================================================
    // Message Tests
    // ==================================================

    testSelector->addItem(
        "TC_MSG_001 - ABS Codec Roundtrip",
        "TC_MSG_001"
    );

    // ==================================================
    // Buttons
    // ==================================================

    runSelectedButton =
        new QPushButton(
            "Run Selected Test"
        );

    runAllButton =
        new QPushButton(
            "Run All Tests"
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

    mainLayout->addLayout(
        selectorLayout
    );

    // ==================================================
    // Summary
    // ==================================================

    summaryLabel =
        new QLabel(
            "Tests: 0 | Passed: 0 | Failed: 0"
        );

    QFont summaryFont =
        summaryLabel->font();

    summaryFont.setBold(
        true
    );

    summaryLabel->setFont(
        summaryFont
    );

    mainLayout->addWidget(
        summaryLabel
    );

    // ==================================================
    // Results table
    // ==================================================

    resultsTable =
        new QTableWidget(
            0,
            8
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

    // ==================================================
    // Column sizing
    // ==================================================

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

    mainLayout->addWidget(
        resultsTable,
        2
    );

    // ==================================================
    // Detail panel
    // ==================================================

    detailText =
        new QTextEdit();

    detailText->setReadOnly(
        true
    );

    detailText->setPlaceholderText(
        "Select a test result to inspect validation evidence."
    );

    mainLayout->addWidget(
        detailText,
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
        // ==================================================
        // Test ID
        // ==================================================

        resultsTable->setItem(
            row,
            0,
            new QTableWidgetItem(
                QString::fromStdString(
                    test.id
                )
            )
        );

        // ==================================================
        // Name
        // ==================================================

        resultsTable->setItem(
            row,
            1,
            new QTableWidgetItem(
                QString::fromStdString(
                    test.name
                )
            )
        );

        // ==================================================
        // Requirement
        // ==================================================

        resultsTable->setItem(
            row,
            2,
            new QTableWidgetItem(
                QString::fromStdString(
                    test.requirementId
                )
            )
        );

        // ==================================================
        // Expected
        // ==================================================

        resultsTable->setItem(
            row,
            3,
            new QTableWidgetItem(
                QString::fromStdString(
                    test.result.expected
                )
            )
        );

        // ==================================================
        // Actual
        // ==================================================

        resultsTable->setItem(
            row,
            4,
            new QTableWidgetItem(
                QString::fromStdString(
                    test.result.actual
                )
            )
        );

        // ==================================================
        // Response / Limit
        // ==================================================

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

        resultsTable->setItem(
            row,
            5,
            timingItem
        );

        // ==================================================
        // Execution time
        // ==================================================

        resultsTable->setItem(
            row,
            6,
            new QTableWidgetItem(
                QString::number(
                    test.result.executionTimeMs,
                    'f',
                    3
                ) +
                " ms"
            )
        );

        // ==================================================
        // Result
        // ==================================================

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
// Summary
// ==================================================

void TestRunnerWidget::updateSummary(
    const std::vector<TestCase>& results)
{
    std::size_t passed =
        0;

    std::size_t failed =
        0;

    for (
        const TestCase& test :
        results
        )
    {
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

    summaryLabel->setText(
        QString(
            "Tests: %1 | Passed: %2 | Failed: %3"
        )
        .arg(
            static_cast<qulonglong>(
                results.size()
                )
        )
        .arg(
            static_cast<qulonglong>(
                passed
                )
        )
        .arg(
            static_cast<qulonglong>(
                failed
                )
        )
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

    QString detail;

    detail +=
        "TEST CASE\n";

    detail +=
        "=========\n\n";

    detail +=
        "ID: " +
        QString::fromStdString(
            test.id
        ) +
        "\n";

    detail +=
        "Name: " +
        QString::fromStdString(
            test.name
        ) +
        "\n";

    detail +=
        "Requirement: " +
        QString::fromStdString(
            test.requirementId
        ) +
        "\n\n";

    detail +=
        "Description:\n" +
        QString::fromStdString(
            test.description
        ) +
        "\n\n";

    detail +=
        "Precondition:\n" +
        QString::fromStdString(
            test.precondition
        ) +
        "\n\n";

    detail +=
        "Stimulus:\n" +
        QString::fromStdString(
            test.stimulus
        ) +
        "\n\n";

    detail +=
        "Expected Result:\n" +
        QString::fromStdString(
            test.expectedResult
        ) +
        "\n\n";

    detail +=
        "Expected:\n" +
        QString::fromStdString(
            test.result.expected
        ) +
        "\n\n";

    detail +=
        "Actual:\n" +
        QString::fromStdString(
            test.result.actual
        ) +
        "\n\n";

    // ==================================================
    // Timing evidence
    // ==================================================

    detail +=
        "Response Time:\n" +
        QString::number(
            test.result.responseTimeMs,
            'f',
            3
        ) +
        " ms\n";

    if (
        test.result.hasTimingRequirement()
        )
    {
        detail +=
            "Maximum Allowed:\n" +
            QString::number(
                test.result.maximumAllowedResponseTimeMs,
                'f',
                3
            ) +
            " ms\n";
    }

    detail +=
        "\nExecution Time:\n" +
        QString::number(
            test.result.executionTimeMs,
            'f',
            3
        ) +
        " ms\n\n";

    detail +=
        "Message:\n" +
        QString::fromStdString(
            test.result.message
        ) +
        "\n\n";

    detail +=
        "RESULT:\n" +
        statusToString(
            test.result.status
        );

    detailText->setPlainText(
        detail
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

    summaryLabel->setText(
        "Tests: 0 | Passed: 0 | Failed: 0"
    );
}