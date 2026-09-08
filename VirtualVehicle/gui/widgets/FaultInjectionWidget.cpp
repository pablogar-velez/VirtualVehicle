#include "FaultInjectionWidget.h"

#include <algorithm>

#include <QAbstractItemView>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QStyle>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTabWidget>
#include <QVBoxLayout>

#include "../../diagnostics/DtcDefinitions.h"
#include "../../diagnostics/UdsService.h"

namespace
{
    QFrame* createSummaryCard(
        const QString& title,
        QLabel*& valueLabel)
    {
        QFrame* frame =
            new QFrame();

        frame->setObjectName(
            "DiagnosticSummaryCard"
        );

        QVBoxLayout* layout =
            new QVBoxLayout(
                frame
            );

        layout->setContentsMargins(
            12,
            9,
            12,
            9
        );

        layout->setSpacing(
            3
        );

        QLabel* titleLabel =
            new QLabel(
                title
            );

        titleLabel->setObjectName(
            "DiagnosticSummaryTitle"
        );

        valueLabel =
            new QLabel();

        valueLabel->setObjectName(
            "DiagnosticSummaryValue"
        );

        layout->addWidget(
            titleLabel
        );

        layout->addWidget(
            valueLabel
        );

        return frame;
    }

    QFrame* createFaultRow(
        const QString& title,
        const QString& description,
        QLabel*& statusLabel,
        QPushButton*& injectButton,
        QPushButton*& clearButton)
    {
        QFrame* frame =
            new QFrame();

        frame->setObjectName(
            "DiagnosticFaultRow"
        );

        QGridLayout* layout =
            new QGridLayout(
                frame
            );

        layout->setContentsMargins(
            12,
            8,
            12,
            8
        );

        layout->setHorizontalSpacing(
            8
        );

        layout->setVerticalSpacing(
            3
        );

        QLabel* titleLabel =
            new QLabel(
                title
            );

        titleLabel->setObjectName(
            "DiagnosticCardTitle"
        );

        QLabel* descriptionLabel =
            new QLabel(
                description
            );

        descriptionLabel->setObjectName(
            "DiagnosticDescription"
        );

        statusLabel =
            new QLabel(
                "● HEALTHY"
            );

        statusLabel->setObjectName(
            "DiagnosticStateBadge"
        );

        statusLabel->setAlignment(
            Qt::AlignRight |
            Qt::AlignVCenter
        );

        injectButton =
            new QPushButton(
                "Inject"
            );

        injectButton->setObjectName(
            "DiagnosticDangerButton"
        );

        clearButton =
            new QPushButton(
                "Clear"
            );

        clearButton->setObjectName(
            "DiagnosticSecondaryButton"
        );

        injectButton->setFixedWidth(
            74
        );

        clearButton->setFixedWidth(
            64
        );

        layout->addWidget(
            titleLabel,
            0,
            0,
            1,
            2
        );

        layout->addWidget(
            statusLabel,
            0,
            2,
            1,
            2
        );

        layout->addWidget(
            descriptionLabel,
            1,
            0,
            1,
            2
        );

        layout->addWidget(
            injectButton,
            1,
            2
        );

        layout->addWidget(
            clearButton,
            1,
            3
        );

        layout->setColumnStretch(
            0,
            1
        );

        layout->setColumnStretch(
            1,
            1
        );

        return frame;
    }
}


FaultInjectionWidget::FaultInjectionWidget(
    QWidget* parent)
    : QGroupBox(
        "Diagnostic Workstation",
        parent
    )
{
    setObjectName(
        "DiagnosticWorkstation"
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
    // Global Diagnostic Summary
    // ==================================================

    QHBoxLayout* summaryLayout =
        new QHBoxLayout();

    summaryLayout->setSpacing(
        10
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "ECU Status",
            ecuStatusValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "UDS Status",
            udsStatusValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "Session",
            sessionValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "Active DTCs",
            activeDtcCountValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "Transport",
            transportValueLabel
        )
    );

    mainLayout->addLayout(
        summaryLayout
    );

    // ==================================================
    // Diagnostic Workspace Tabs
    // ==================================================

    QTabWidget* workspaceTabs =
        new QTabWidget();

    workspaceTabs->setObjectName(
        "DiagnosticWorkspaceTabs"
    );

    workspaceTabs->setDocumentMode(
        false
    );

    mainLayout->addWidget(
        workspaceTabs,
        1
    );

    // ==================================================
    // TAB 1 — OVERVIEW
    // ==================================================

    QWidget* overviewPage =
        new QWidget();

    overviewPage->setObjectName(
        "DiagnosticTabPage"
    );

    QHBoxLayout* overviewRootLayout =
        new QHBoxLayout(
            overviewPage
        );

    overviewRootLayout->setContentsMargins(
        8,
        10,
        8,
        8
    );

    overviewRootLayout->setSpacing(
        12
    );

    // --------------------------------------------------
    // Left: System / ECU Health
    // --------------------------------------------------

    QGroupBox* monitorGroup =
        new QGroupBox(
            "Live System / ECU Health"
        );

    monitorGroup->setObjectName(
        "DiagnosticSection"
    );

    QVBoxLayout* monitorRootLayout =
        new QVBoxLayout(
            monitorGroup
        );

    monitorRootLayout->setContentsMargins(
        12,
        18,
        12,
        12
    );

    monitorRootLayout->setSpacing(
        10
    );

    monitorOverallStateLabel =
        new QLabel(
            "● SYSTEM HEALTHY"
        );

    monitorOverallStateLabel->setObjectName(
        "DiagnosticMonitorOverall"
    );

    monitorRootLayout->addWidget(
        monitorOverallStateLabel
    );

    auto createMonitorLine =
        [](const QString& title, QLabel*& value)
        {
            QWidget* row =
                new QWidget();

            QHBoxLayout* rowLayout =
                new QHBoxLayout(
                    row
                );

            rowLayout->setContentsMargins(
                0,
                2,
                0,
                2
            );

            rowLayout->setSpacing(
                8
            );

            QLabel* titleLabel =
                new QLabel(
                    title
                );

            titleLabel->setObjectName(
                "DiagnosticDescription"
            );

            value =
                new QLabel(
                    "● OK"
                );

            value->setObjectName(
                "DiagnosticMonitorValue"
            );

            value->setAlignment(
                Qt::AlignRight |
                Qt::AlignVCenter
            );

            rowLayout->addWidget(
                titleLabel
            );

            rowLayout->addStretch();

            rowLayout->addWidget(
                value
            );

            return row;
        };

    QHBoxLayout* healthColumns =
        new QHBoxLayout();

    healthColumns->setSpacing(
        12
    );

    QFrame* ecuHealthCard =
        new QFrame();

    ecuHealthCard->setObjectName(
        "DiagnosticHealthCard"
    );

    QVBoxLayout* ecuHealthLayout =
        new QVBoxLayout(
            ecuHealthCard
        );

    ecuHealthLayout->setContentsMargins(
        12,
        10,
        12,
        10
    );

    ecuHealthLayout->setSpacing(
        8
    );

    QLabel* ecuHealthTitle =
        new QLabel(
            "ECU Health"
        );

    ecuHealthTitle->setObjectName(
        "DiagnosticCardTitle"
    );

    ecuHealthLayout->addWidget(
        ecuHealthTitle
    );

    ecuHealthLayout->addWidget(
        createMonitorLine(
            "Powertrain ECU",
            monitorPowertrainEcuLabel
        )
    );

    ecuHealthLayout->addWidget(
        createMonitorLine(
            "ABS ECU",
            monitorAbsEcuLabel
        )
    );

    ecuHealthLayout->addWidget(
        createMonitorLine(
            "Steering ECU",
            monitorSteeringEcuLabel
        )
    );

    ecuHealthLayout->addWidget(
        createMonitorLine(
            "Dashboard ECU",
            monitorDashboardEcuLabel
        )
    );

    ecuHealthLayout->addStretch();

    QFrame* communicationHealthCard =
        new QFrame();

    communicationHealthCard->setObjectName(
        "DiagnosticHealthCard"
    );

    QVBoxLayout* communicationHealthLayout =
        new QVBoxLayout(
            communicationHealthCard
        );

    communicationHealthLayout->setContentsMargins(
        12,
        10,
        12,
        10
    );

    communicationHealthLayout->setSpacing(
        8
    );

    QLabel* communicationHealthTitle =
        new QLabel(
            "Signals / Communication"
        );

    communicationHealthTitle->setObjectName(
        "DiagnosticCardTitle"
    );

    communicationHealthLayout->addWidget(
        communicationHealthTitle
    );

    communicationHealthLayout->addWidget(
        createMonitorLine(
            "Front Left Sensor",
            monitorFrontLeftLabel
        )
    );

    communicationHealthLayout->addWidget(
        createMonitorLine(
            "Front Right Sensor",
            monitorFrontRightLabel
        )
    );

    communicationHealthLayout->addWidget(
        createMonitorLine(
            "ABS CAN 0x080",
            monitorAbsCanLabel
        )
    );

    communicationHealthLayout->addWidget(
        createMonitorLine(
            "Powertrain CAN 0x100",
            monitorPowertrainCanLabel
        )
    );

    communicationHealthLayout->addWidget(
        createMonitorLine(
            "Steering CAN 0x120",
            monitorSteeringCanLabel
        )
    );

    communicationHealthLayout->addWidget(
        createMonitorLine(
            "Ethernet Node A",
            monitorEthernetLabel
        )
    );

    communicationHealthLayout->addStretch();

    healthColumns->addWidget(
        ecuHealthCard,
        1
    );

    healthColumns->addWidget(
        communicationHealthCard,
        1
    );

    monitorRootLayout->addLayout(
        healthColumns,
        1
    );

    overviewRootLayout->addWidget(
        monitorGroup,
        5
    );

    // --------------------------------------------------
    // Right: DTCs + Guidance
    // --------------------------------------------------

    QWidget* overviewRight =
        new QWidget();

    QVBoxLayout* overviewRightLayout =
        new QVBoxLayout(
            overviewRight
        );

    overviewRightLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    overviewRightLayout->setSpacing(
        12
    );

    QGroupBox* dtcGroup =
        new QGroupBox(
            "Diagnostic Trouble Codes"
        );

    dtcGroup->setObjectName(
        "DiagnosticSection"
    );

    QVBoxLayout* dtcLayout =
        new QVBoxLayout(
            dtcGroup
        );

    dtcLayout->setContentsMargins(
        12,
        18,
        12,
        10
    );

    dtcLayout->setSpacing(
        8
    );

    dtcEmptyStateLabel =
        new QLabel(
            "● No active diagnostic trouble codes"
        );

    dtcEmptyStateLabel->setObjectName(
        "DtcEmptyState"
    );

    dtcTable =
        new QTableWidget(
            0,
            4
        );

    dtcTable->setHorizontalHeaderLabels(
        {
            "DTC",
            "Status",
            "Description",
            "Occurrences"
        }
    );

    dtcTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    dtcTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    dtcTable->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    dtcTable->setAlternatingRowColors(
        true
    );

    dtcTable->setShowGrid(
        false
    );

    dtcTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            0,
            QHeaderView::ResizeToContents
        );

    dtcTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            1,
            QHeaderView::ResizeToContents
        );

    dtcTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            2,
            QHeaderView::Stretch
        );

    dtcTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            3,
            QHeaderView::ResizeToContents
        );

    dtcTable
        ->verticalHeader()
        ->setVisible(
            false
        );

    dtcLayout->addWidget(
        dtcEmptyStateLabel
    );

    dtcLayout->addWidget(
        dtcTable,
        1
    );

    QGroupBox* actionGroup =
        new QGroupBox(
            "Diagnostic Guidance"
        );

    actionGroup->setObjectName(
        "DiagnosticSection"
    );

    QVBoxLayout* actionLayout =
        new QVBoxLayout(
            actionGroup
        );

    actionLayout->setContentsMargins(
        12,
        18,
        12,
        10
    );

    actionLayout->setSpacing(
        7
    );

    selectedFaultTitleLabel =
        new QLabel(
            "No active fault"
        );

    selectedFaultTitleLabel->setObjectName(
        "DiagnosticGuidanceTitle"
    );

    selectedFaultImpactLabel =
        new QLabel(
            "System operation is nominal."
        );

    selectedFaultImpactLabel->setObjectName(
        "DiagnosticDescription"
    );

    selectedFaultImpactLabel->setWordWrap(
        true
    );

    selectedFaultActionLabel =
        new QLabel(
            "No corrective action is required."
        );

    selectedFaultActionLabel->setObjectName(
        "DiagnosticGuidanceAction"
    );

    selectedFaultActionLabel->setWordWrap(
        true
    );

    QLabel* workflowHintLabel =
        new QLabel(
            "Workflow: Inject → Observe → Confirm evidence → Clear → Verify recovery"
        );

    workflowHintLabel->setObjectName(
        "DiagnosticWorkflowHint"
    );

    workflowHintLabel->setWordWrap(
        true
    );

    actionLayout->addWidget(
        selectedFaultTitleLabel
    );

    actionLayout->addWidget(
        selectedFaultImpactLabel
    );

    actionLayout->addWidget(
        selectedFaultActionLabel
    );

    actionLayout->addStretch();

    actionLayout->addWidget(
        workflowHintLabel
    );

    overviewRightLayout->addWidget(
        dtcGroup,
        5
    );

    overviewRightLayout->addWidget(
        actionGroup,
        3
    );

    overviewRootLayout->addWidget(
        overviewRight,
        7
    );

    workspaceTabs->addTab(
        overviewPage,
        "Overview / Health"
    );

    // ==================================================
    // TAB 2 — FAULT INJECTION
    // ==================================================

    QWidget* faultsPage =
        new QWidget();

    faultsPage->setObjectName(
        "DiagnosticTabPage"
    );

    QVBoxLayout* faultsPageLayout =
        new QVBoxLayout(
            faultsPage
        );

    faultsPageLayout->setContentsMargins(
        8,
        10,
        8,
        8
    );

    faultsPageLayout->setSpacing(
        10
    );

    QFrame* faultInstructionCard =
        new QFrame();

    faultInstructionCard->setObjectName(
        "DiagnosticInstructionCard"
    );

    QHBoxLayout* faultInstructionLayout =
        new QHBoxLayout(
            faultInstructionCard
        );

    faultInstructionLayout->setContentsMargins(
        14,
        10,
        14,
        10
    );

    faultInstructionLayout->setSpacing(
        12
    );

    QVBoxLayout* faultInstructionTextLayout =
        new QVBoxLayout();

    faultInstructionTextLayout->setSpacing(
        2
    );

    QLabel* faultHelpTitle =
        new QLabel(
            "Recommended Diagnostic Workflow"
        );

    faultHelpTitle->setObjectName(
        "DiagnosticCardTitle"
    );

    QLabel* faultHelpLabel =
        new QLabel(
            "1  Inject fault   →   2  Observe in Overview / Health   →   3  Clear fault   →   4  Verify recovery"
        );

    faultHelpLabel->setObjectName(
        "DiagnosticDescription"
    );

    faultHelpLabel->setWordWrap(
        true
    );

    faultInstructionTextLayout->addWidget(
        faultHelpTitle
    );

    faultInstructionTextLayout->addWidget(
        faultHelpLabel
    );

    QLabel* faultWorkflowHint =
        new QLabel(
            "Use Overview / Health to confirm the DTC, affected ECU or communication path, and recovery evidence."
        );

    faultWorkflowHint->setObjectName(
        "DiagnosticWorkflowHint"
    );

    faultWorkflowHint->setWordWrap(
        true
    );

    faultInstructionTextLayout->addWidget(
        faultWorkflowHint
    );

    clearAllFaultsButton =
        new QPushButton(
            "Clear All"
        );

    clearAllFaultsButton->setObjectName(
        "DiagnosticClearAllButton"
    );

    clearAllFaultsButton->setMinimumWidth(
        130
    );

    clearAllFaultsButton->setMinimumHeight(
        38
    );

    faultInstructionLayout->addLayout(
        faultInstructionTextLayout,
        1
    );

    faultInstructionLayout->addWidget(
        clearAllFaultsButton
    );

    faultsPageLayout->addWidget(
        faultInstructionCard
    );

    QScrollArea* faultScrollArea =
        new QScrollArea();

    faultScrollArea->setObjectName(
        "DiagnosticScrollArea"
    );

    faultScrollArea->setWidgetResizable(
        true
    );

    faultScrollArea->setFrameShape(
        QFrame::NoFrame
    );

    faultScrollArea->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff
    );

    QWidget* faultScrollContent =
        new QWidget();

    faultScrollContent->setObjectName(
        "DiagnosticScrollContent"
    );

    QGridLayout* faultGrid =
        new QGridLayout(
            faultScrollContent
        );

    faultGrid->setContentsMargins(
        0,
        0,
        6,
        0
    );

    faultGrid->setHorizontalSpacing(
        10
    );

    faultGrid->setVerticalSpacing(
        10
    );

    faultGrid->addWidget(
        createFaultRow(
            "Front Left Wheel Sensor",
            "Wheel-speed signal becomes invalid",
            frontLeftStatusLabel,
            frontLeftDropoutButton,
            clearFrontLeftFaultButton
        ),
        0,
        0
    );

    faultGrid->addWidget(
        createFaultRow(
            "Front Right Wheel Sensor",
            "Wheel-speed signal becomes invalid",
            frontRightStatusLabel,
            frontRightDropoutButton,
            clearFrontRightFaultButton
        ),
        0,
        1
    );

    faultGrid->addWidget(
        createFaultRow(
            "ABS CAN Communication",
            "Suppress CAN 0x080 ABS_WHEEL_STATE",
            absCanStatusLabel,
            absCanDropoutButton,
            clearAbsCanFaultButton
        ),
        1,
        0
    );

    faultGrid->addWidget(
        createFaultRow(
            "Powertrain CAN Communication",
            "Suppress CAN 0x100 POWERTRAIN_STATE",
            powertrainCanStatusLabel,
            powertrainCanDropoutButton,
            clearPowertrainCanFaultButton
        ),
        1,
        1
    );

    faultGrid->addWidget(
        createFaultRow(
            "Steering CAN Communication",
            "Suppress CAN 0x120 STEERING_STATE",
            steeringCanStatusLabel,
            steeringCanDropoutButton,
            clearSteeringCanFaultButton
        ),
        2,
        0
    );

    faultGrid->addWidget(
        createFaultRow(
            "Ethernet Node A",
            "Block Node A Ethernet TX and RX",
            ethernetNodeAStatusLabel,
            ethernetNodeADropoutButton,
            clearEthernetNodeAFaultButton
        ),
        2,
        1
    );

    faultGrid->setColumnStretch(
        0,
        1
    );

    faultGrid->setColumnStretch(
        1,
        1
    );

    faultGrid->setRowStretch(
        3,
        1
    );

    faultScrollArea->setWidget(
        faultScrollContent
    );

    faultsPageLayout->addWidget(
        faultScrollArea,
        1
    );

    workspaceTabs->addTab(
        faultsPage,
        "Fault Injection"
    );

    // ==================================================
    // TAB 3 — UDS TESTER
    // ==================================================

    QWidget* udsPage =
        new QWidget();

    udsPage->setObjectName(
        "DiagnosticTabPage"
    );

    QVBoxLayout* udsPageLayout =
        new QVBoxLayout(
            udsPage
        );

    udsPageLayout->setContentsMargins(
        8,
        10,
        8,
        8
    );

    udsPageLayout->setSpacing(
        12
    );

    QGroupBox* udsGroup =
        new QGroupBox(
            "UDS Diagnostic Tester"
        );

    udsGroup->setObjectName(
        "DiagnosticSection"
    );

    QVBoxLayout* udsLayout =
        new QVBoxLayout(
            udsGroup
        );

    udsLayout->setContentsMargins(
        12,
        18,
        12,
        12
    );

    udsLayout->setSpacing(
        10
    );

    QHBoxLayout* udsControlLayout =
        new QHBoxLayout();

    QLabel* serviceLabel =
        new QLabel(
            "Service"
        );

    serviceLabel->setObjectName(
        "DiagnosticDescription"
    );

    udsServiceCombo =
        new QComboBox();

    udsServiceCombo->setObjectName(
        "UdsServiceCombo"
    );

    udsServiceCombo->addItem(
        "Read Active DTCs  —  0x19 / 0x02",
        0
    );

    udsServiceCombo->addItem(
        "Read Vehicle Speed  —  DID 0xF100",
        1
    );

    udsServiceCombo->addItem(
        "Read Engine RPM  —  DID 0xF101",
        2
    );

    udsServiceCombo->addItem(
        "Read Steering Angle  —  DID 0xF102",
        3
    );

    udsServiceCombo->addItem(
        "Extended Diagnostic Session  —  0x10 / 0x03",
        4
    );

    udsServiceCombo->addItem(
        "Default Diagnostic Session  —  0x10 / 0x01",
        5
    );

    udsServiceCombo->addItem(
        "Clear All DTCs  —  0x14 / FF FF FF",
        6
    );

    udsServiceCombo->setMaxVisibleItems(
        10
    );

    udsServiceCombo->setMinimumHeight(
        38
    );

    udsServiceCombo->setMinimumContentsLength(
        36
    );

    sendUdsRequestButton =
        new QPushButton(
            "Send Request"
        );

    sendUdsRequestButton->setObjectName(
        "DiagnosticPrimaryButton"
    );

    udsControlLayout->addWidget(
        serviceLabel
    );

    udsControlLayout->addWidget(
        udsServiceCombo,
        1
    );

    udsControlLayout->addWidget(
        sendUdsRequestButton
    );

    udsLayout->addLayout(
        udsControlLayout
    );

    udsServiceDescriptionLabel =
        new QLabel(
            "Reports all currently active diagnostic trouble codes."
        );

    udsServiceDescriptionLabel->setObjectName(
        "UdsServiceDescription"
    );

    udsServiceDescriptionLabel->setWordWrap(
        true
    );

    udsLayout->addWidget(
        udsServiceDescriptionLabel
    );

    QHBoxLayout* transactionLayout =
        new QHBoxLayout();

    transactionLayout->setSpacing(
        12
    );

    QFrame* requestCard =
        new QFrame();

    requestCard->setObjectName(
        "DiagnosticTransactionCard"
    );

    QVBoxLayout* requestLayout =
        new QVBoxLayout(
            requestCard
        );

    QLabel* requestTitle =
        new QLabel(
            "REQUEST"
        );

    requestTitle->setObjectName(
        "DiagnosticTransactionTitle"
    );

    requestCanIdLabel =
        new QLabel(
            "CAN ID  0x7E0"
        );

    requestCanIdLabel->setObjectName(
        "DiagnosticDescription"
    );

    requestBytesLabel =
        new QLabel(
            "--"
        );

    requestBytesLabel->setObjectName(
        "DiagnosticByteString"
    );

    requestLayout->addWidget(
        requestTitle
    );

    requestLayout->addWidget(
        requestCanIdLabel
    );

    requestLayout->addWidget(
        requestBytesLabel
    );

    requestLayout->addStretch();

    QFrame* responseCard =
        new QFrame();

    responseCard->setObjectName(
        "DiagnosticTransactionCard"
    );

    QVBoxLayout* responseLayout =
        new QVBoxLayout(
            responseCard
        );

    QHBoxLayout* responseHeader =
        new QHBoxLayout();

    QLabel* responseTitle =
        new QLabel(
            "RESPONSE"
        );

    responseTitle->setObjectName(
        "DiagnosticTransactionTitle"
    );

    responseStatusLabel =
        new QLabel(
            "WAITING"
        );

    responseStatusLabel->setObjectName(
        "UdsResponseStatus"
    );

    responseHeader->addWidget(
        responseTitle
    );

    responseHeader->addStretch();

    responseHeader->addWidget(
        responseStatusLabel
    );

    responseCanIdLabel =
        new QLabel(
            "CAN ID  0x7E8"
        );

    responseCanIdLabel->setObjectName(
        "DiagnosticDescription"
    );

    responseBytesLabel =
        new QLabel(
            "--"
        );

    responseBytesLabel->setObjectName(
        "DiagnosticByteString"
    );

    responseLayout->addLayout(
        responseHeader
    );

    responseLayout->addWidget(
        responseCanIdLabel
    );

    responseLayout->addWidget(
        responseBytesLabel
    );

    responseLayout->addStretch();

    transactionLayout->addWidget(
        requestCard,
        1
    );

    transactionLayout->addWidget(
        responseCard,
        1
    );

    udsLayout->addLayout(
        transactionLayout,
        1
    );

    udsPageLayout->addWidget(
        udsGroup,
        1
    );

    workspaceTabs->addTab(
        udsPage,
        "UDS Services"
    );

    // ==================================================
    // Interactions
    // ==================================================

    connect(
        dtcTable,
        &QTableWidget::currentCellChanged,
        this,
        [this](
            int currentRow,
            int,
            int,
            int)
        {
            if (
                currentRow < 0 ||
                currentRow >=
                dtcTable->rowCount()
                )
            {
                return;
            }

            QTableWidgetItem* item =
                dtcTable->item(
                    currentRow,
                    0
                );

            if (item == nullptr)
            {
                return;
            }

            bool ok =
                false;

            const std::uint32_t code =
                item->text()
                .mid(2)
                .toUInt(
                    &ok,
                    16
                );

            if (ok)
            {
                updateRecommendedAction(
                    code
                );
            }
        }
    );

    connect(
        udsServiceCombo,
        &QComboBox::currentIndexChanged,
        this,
        [this](int index)
        {
            switch (index)
            {
            case 0:
                udsServiceDescriptionLabel->setText(
                    "Read active DTCs using UDS service 0x19 with subfunction 0x02."
                );
                break;

            case 1:
                udsServiceDescriptionLabel->setText(
                    "Read vehicle speed using ReadDataByIdentifier with DID 0xF100."
                );
                break;

            case 2:
                udsServiceDescriptionLabel->setText(
                    "Read engine RPM using ReadDataByIdentifier with DID 0xF101."
                );
                break;

            case 3:
                udsServiceDescriptionLabel->setText(
                    "Read steering angle using ReadDataByIdentifier with DID 0xF102."
                );
                break;

            case 4:
                udsServiceDescriptionLabel->setText(
                    "Switch the diagnostic server to Extended Session (0x03)."
                );
                break;

            case 5:
                udsServiceDescriptionLabel->setText(
                    "Return the diagnostic server to Default Session (0x01)."
                );
                break;

            case 6:
                udsServiceDescriptionLabel->setText(
                    "Clear all registered DTCs using the simplified FF FF FF group request."
                );
                break;

            default:
                udsServiceDescriptionLabel->clear();
                break;
            }
        }
    );

    // ==================================================
    // Initial values
    // ==================================================

    ecuStatusValueLabel->setText(
        "● ONLINE"
    );

    udsStatusValueLabel->setText(
        "● READY"
    );

    sessionValueLabel->setText(
        "DEFAULT  (0x01)"
    );

    activeDtcCountValueLabel->setText(
        "0"
    );

    transportValueLabel->setText(
        "0x7E0 → 0x7E8"
    );

    refreshDynamicStyle(
        ecuStatusValueLabel,
        "healthy"
    );

    refreshDynamicStyle(
        udsStatusValueLabel,
        "healthy"
    );

    refreshDynamicStyle(
        frontLeftStatusLabel,
        "healthy"
    );

    refreshDynamicStyle(
        frontRightStatusLabel,
        "healthy"
    );

    refreshDynamicStyle(
        absCanStatusLabel,
        "healthy"
    );

    refreshDynamicStyle(
        powertrainCanStatusLabel,
        "healthy"
    );

    refreshDynamicStyle(
        steeringCanStatusLabel,
        "healthy"
    );

    refreshDynamicStyle(
        ethernetNodeAStatusLabel,
        "healthy"
    );

    refreshDynamicStyle(
        responseStatusLabel,
        "neutral"
    );

    // ==================================================
    // Workstation-specific styling
    // ==================================================

    setStyleSheet(
        R"(
            QGroupBox#DiagnosticWorkstation
            {
                background: transparent;
            }

            QTabWidget#DiagnosticWorkspaceTabs::pane
            {
                border: 1px solid #DCE3EA;
                border-radius: 9px;
                background-color: transparent;
                top: -1px;
            }

            QTabWidget#DiagnosticWorkspaceTabs QTabBar
            {
                background: transparent;
            }

            QTabWidget#DiagnosticWorkspaceTabs QTabBar::tab
            {
                min-width: 135px;
                min-height: 36px;
                padding: 5px 16px;
                margin-right: 6px;
                border: 1px solid #DCE3EA;
                border-radius: 7px;
                background-color: #EEF2F5;
                color: #6C7F92;
            }

            QTabWidget#DiagnosticWorkspaceTabs QTabBar::tab:hover
            {
                background-color: #E4EBF2;
                color: #2D4053;
            }

            QTabWidget#DiagnosticWorkspaceTabs QTabBar::tab:selected
            {
                background-color: #456E97;
                color: #FFFFFF;
                border-color: #456E97;
                font-weight: 700;
            }

            QWidget#DiagnosticTabPage,
            QWidget#DiagnosticScrollContent
            {
                background: transparent;
            }

            QGroupBox#DiagnosticSection
            {
                border: 1px solid #DCE3EA;
                border-radius: 9px;
                margin-top: 10px;
                padding-top: 12px;
                font-weight: 700;
            }

            QGroupBox#DiagnosticSection::title
            {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px;
                color: #17212B;
            }

            QFrame#DiagnosticSummaryCard,
            QFrame#DiagnosticFaultRow,
            QFrame#DiagnosticHealthCard,
            QFrame#DiagnosticTransactionCard,
            QFrame#DiagnosticInstructionCard
            {
                border: 1px solid #DCE3EA;
                border-radius: 8px;
                background-color: #FFFFFF;
            }

            QFrame#DiagnosticFaultRow
            {
                min-height: 68px;
            }

            QLabel#DiagnosticSummaryTitle,
            QLabel#DiagnosticDescription,
            QLabel#DiagnosticTransactionTitle,
            QLabel#UdsServiceDescription
            {
                color: #74879A;
            }

            QLabel#DiagnosticCardTitle,
            QLabel#DiagnosticGuidanceTitle
            {
                color: #17212B;
                font-size: 13px;
                font-weight: 700;
            }

            QLabel#DiagnosticSummaryValue
            {
                color: #17212B;
                font-weight: 700;
            }

            QLabel#DiagnosticStateBadge,
            QLabel#UdsResponseStatus,
            QLabel#DiagnosticMonitorOverall,
            QLabel#DiagnosticMonitorValue
            {
                font-weight: 700;
            }

            QLabel#DtcEmptyState
            {
                color: #138A4B;
                font-weight: 700;
            }

            QLabel[state="healthy"],
            QLabel[state="positive"]
            {
                color: #138A4B;
            }

            QLabel[state="warning"],
            QLabel[state="negative"]
            {
                color: #C2633A;
            }

            QLabel[state="busy"]
            {
                color: #B47A2B;
            }

            QLabel[state="neutral"]
            {
                color: #6F7E8D;
            }

            QLabel#DiagnosticGuidanceAction
            {
                color: #425364;
                font-weight: 600;
            }

            QLabel#DiagnosticWorkflowHint
            {
                color: #6F7E8D;
                font-size: 11px;
                font-weight: 600;
            }

            QLabel#DiagnosticByteString
            {
                color: #17212B;
                font-family: Consolas, "Courier New", monospace;
                font-size: 14px;
                font-weight: 700;
            }

            QScrollArea#DiagnosticScrollArea
            {
                background: transparent;
                border: none;
            }

            QScrollBar:vertical
            {
                background: transparent;
                width: 9px;
                margin: 2px 0 2px 0;
            }

            QScrollBar::handle:vertical
            {
                background: #C6D0D9;
                min-height: 28px;
                border-radius: 4px;
            }

            QScrollBar::handle:vertical:hover
            {
                background: #AEBBC7;
            }

            QScrollBar::add-line:vertical,
            QScrollBar::sub-line:vertical
            {
                height: 0px;
            }

            QScrollBar::add-page:vertical,
            QScrollBar::sub-page:vertical
            {
                background: transparent;
            }

            QPushButton#DiagnosticPrimaryButton
            {
                min-width: 120px;
                min-height: 38px;
            }

            QLabel#DiagnosticWorkflowHint
            {
                color: #6F8298;
                font-size: 11px;
            }

            QFrame#DiagnosticInstructionCard
            {
                background: #F8FAFC;
                border: 1px solid #D8E2EC;
                border-radius: 8px;
            }

            QPushButton#DiagnosticClearAllButton
            {
                min-width: 130px;
                min-height: 38px;
                font-weight: 700;
            }

            QPushButton#DiagnosticDangerButton,
            QPushButton#DiagnosticSecondaryButton
            {
                min-height: 32px;
            }

            QTableWidget
            {
                color: #17212B;
                background-color: #FFFFFF;
                alternate-background-color: #F6F8FA;
                border: 1px solid #E0E6EC;
                border-radius: 6px;
            }

            QHeaderView::section
            {
                color: #425364;
                background-color: #EEF2F5;
                border: none;
                border-bottom: 1px solid #DCE3EA;
                padding: 7px;
            }

            QComboBox#UdsServiceCombo
            {
                color: #17212B;
                background-color: #FFFFFF;
                border: 1px solid #CFD9E2;
                border-radius: 6px;
                padding: 7px 10px;
            }

            QComboBox#UdsServiceCombo QAbstractItemView
            {
                color: #17212B;
                background-color: #FFFFFF;
                selection-color: #FFFFFF;
                selection-background-color: #456E97;
                border: 1px solid #CFD9E2;
                outline: none;
            }
        )"
    );
}

QPushButton*
FaultInjectionWidget::getFrontLeftDropoutButton() const
{
    return frontLeftDropoutButton;
}

QPushButton*
FaultInjectionWidget::getFrontRightDropoutButton() const
{
    return frontRightDropoutButton;
}

QPushButton*
FaultInjectionWidget::getClearFrontLeftFaultButton() const
{
    return clearFrontLeftFaultButton;
}

QPushButton*
FaultInjectionWidget::getClearFrontRightFaultButton() const
{
    return clearFrontRightFaultButton;
}

QPushButton*
FaultInjectionWidget::getAbsCanDropoutButton() const
{
    return absCanDropoutButton;
}

QPushButton*
FaultInjectionWidget::getClearAbsCanFaultButton() const
{
    return clearAbsCanFaultButton;
}

QPushButton*
FaultInjectionWidget::getPowertrainCanDropoutButton() const
{
    return powertrainCanDropoutButton;
}

QPushButton*
FaultInjectionWidget::getClearPowertrainCanFaultButton() const
{
    return clearPowertrainCanFaultButton;
}

QPushButton*
FaultInjectionWidget::getSteeringCanDropoutButton() const
{
    return steeringCanDropoutButton;
}

QPushButton*
FaultInjectionWidget::getClearSteeringCanFaultButton() const
{
    return clearSteeringCanFaultButton;
}

QPushButton*
FaultInjectionWidget::getEthernetNodeADropoutButton() const
{
    return ethernetNodeADropoutButton;
}

QPushButton*
FaultInjectionWidget::getClearEthernetNodeAFaultButton() const
{
    return clearEthernetNodeAFaultButton;
}

QPushButton*
FaultInjectionWidget::getClearAllFaultsButton() const
{
    return clearAllFaultsButton;
}

QPushButton*
FaultInjectionWidget::getSendUdsRequestButton() const
{
    return sendUdsRequestButton;
}

UdsRequest
FaultInjectionWidget::buildSelectedUdsRequest() const
{
    UdsRequest request;

    switch (
        udsServiceCombo->currentData().toInt()
        )
    {
    case 0:
        request.serviceId =
            UdsService::ReadDtcInformation;

        request.payload =
        {
            UdsService::ReportActiveDtcs
        };

        break;

    case 1:
        request.serviceId =
            UdsService::ReadDataByIdentifier;

        request.payload =
        {
            0xF1,
            0x00
        };

        break;

    case 2:
        request.serviceId =
            UdsService::ReadDataByIdentifier;

        request.payload =
        {
            0xF1,
            0x01
        };

        break;

    case 3:
        request.serviceId =
            UdsService::ReadDataByIdentifier;

        request.payload =
        {
            0xF1,
            0x02
        };

        break;

    case 4:
        request.serviceId =
            UdsService::DiagnosticSessionControl;

        request.payload =
        {
            UdsService::ExtendedSession
        };

        break;

    case 5:
        request.serviceId =
            UdsService::DiagnosticSessionControl;

        request.payload =
        {
            UdsService::DefaultSession
        };

        break;

    case 6:
        request.serviceId =
            UdsService::ClearDiagnosticInformation;

        request.payload =
        {
            0xFF,
            0xFF,
            0xFF
        };

        break;

    default:
        request.serviceId =
            UdsService::ReadDtcInformation;

        request.payload =
        {
            UdsService::ReportActiveDtcs
        };

        break;
    }

    return request;
}

void FaultInjectionWidget::showSubmittedRequest(
    const UdsRequest& request)
{
    requestBytesLabel->setText(
        formatBytes(
            request
        )
    );

    responseBytesLabel->setText(
        "--"
    );

    responseStatusLabel->setText(
        "PENDING"
    );

    refreshDynamicStyle(
        responseStatusLabel,
        "busy"
    );
}

void FaultInjectionWidget::showUdsResponse(
    const UdsResponse& response)
{
    responseBytesLabel->setText(
        formatBytes(
            response
        )
    );

    responseStatusLabel->setText(
        response.isPositive()
        ? "● POSITIVE RESPONSE"
        : "● NEGATIVE RESPONSE"
    );

    refreshDynamicStyle(
        responseStatusLabel,
        response.isPositive()
        ? "positive"
        : "negative"
    );
}

void FaultInjectionWidget::updateDiagnostics(
    const DtcManager& dtcManager,
    std::uint8_t udsSession,
    bool udsTransactionPending)
{
    std::size_t activeCount =
        0;

    for (
        const Dtc& dtc :
        dtcManager.getDtcs()
        )
    {
        if (
            dtc.isActive()
            )
        {
            ++activeCount;
        }
    }

    activeDtcCountValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                activeCount
                )
        )
    );

    sessionValueLabel->setText(
        sessionText(
            udsSession
        )
    );

    udsStatusValueLabel->setText(
        udsTransactionPending
        ? "● BUSY"
        : "● READY"
    );

    refreshDynamicStyle(
        udsStatusValueLabel,
        udsTransactionPending
        ? "busy"
        : "healthy"
    );

    updateFaultStatusCards(
        dtcManager
    );

    updateLiveMonitor(
        dtcManager
    );

    updateDtcTable(
        dtcManager
    );
}

void FaultInjectionWidget::clearRuntimeView()
{
    requestBytesLabel->setText(
        "--"
    );

    responseBytesLabel->setText(
        "--"
    );

    responseStatusLabel->setText(
        "WAITING"
    );

    refreshDynamicStyle(
        responseStatusLabel,
        "neutral"
    );
}

void FaultInjectionWidget::updateDtcTable(
    const DtcManager& dtcManager)
{
    const int previousRow =
        dtcTable->currentRow();

    dtcTable->setRowCount(
        0
    );

    std::size_t activeCount =
        0;

    std::uint32_t firstActiveCode =
        0;

    for (
        const Dtc& dtc :
        dtcManager.getDtcs()
        )
    {
        if (
            !dtc.isActive()
            )
        {
            continue;
        }

        if (activeCount == 0)
        {
            firstActiveCode =
                dtc.code;
        }

        ++activeCount;

        const int row =
            dtcTable->rowCount();

        dtcTable->insertRow(
            row
        );

        QTableWidgetItem* codeItem =
            new QTableWidgetItem(
                formatDtcCode(
                    dtc.code
                )
            );

        QTableWidgetItem* statusItem =
            new QTableWidgetItem(
                dtcStatusText(
                    dtc.status
                )
            );

        QTableWidgetItem* descriptionItem =
            new QTableWidgetItem(
                QString::fromStdString(
                    dtc.description
                )
            );

        QTableWidgetItem* occurrenceItem =
            new QTableWidgetItem(
                QString::number(
                    dtc.occurrenceCount
                )
            );

        statusItem->setForeground(
            QColor(
                "#C2633A"
            )
        );

        QFont statusFont =
            statusItem->font();

        statusFont.setBold(
            true
        );

        statusItem->setFont(
            statusFont
        );

        dtcTable->setItem(
            row,
            0,
            codeItem
        );

        dtcTable->setItem(
            row,
            1,
            statusItem
        );

        dtcTable->setItem(
            row,
            2,
            descriptionItem
        );

        dtcTable->setItem(
            row,
            3,
            occurrenceItem
        );
    }

    dtcEmptyStateLabel->setVisible(
        activeCount ==
        0
    );

    dtcTable->setVisible(
        activeCount >
        0
    );

    if (
        activeCount ==
        0
        )
    {
        selectedFaultTitleLabel->setText(
            "No active fault"
        );

        selectedFaultImpactLabel->setText(
            "System operation is nominal."
        );

        selectedFaultActionLabel->setText(
            "No corrective action is required."
        );

        return;
    }

    const int rowToSelect =
        std::clamp(
            previousRow,
            0,
            dtcTable->rowCount() -
            1
        );

    dtcTable->setCurrentCell(
        rowToSelect,
        0
    );

    QTableWidgetItem* selectedCode =
        dtcTable->item(
            rowToSelect,
            0
        );

    if (selectedCode != nullptr)
    {
        bool ok =
            false;

        const std::uint32_t code =
            selectedCode->text()
            .mid(2)
            .toUInt(
                &ok,
                16
            );

        if (ok)
        {
            updateRecommendedAction(
                code
            );
        }
        else
        {
            updateRecommendedAction(
                firstActiveCode
            );
        }
    }
}

void FaultInjectionWidget::updateFaultStatusCards(
    const DtcManager& dtcManager)
{
    struct FaultStatusBinding
    {
        std::uint32_t code;
        QLabel* label;
    };

    const FaultStatusBinding bindings[] =
    {
        {
            DtcDefinitions::FrontLeftWheelSpeedSensor,
            frontLeftStatusLabel
        },
        {
            DtcDefinitions::FrontRightWheelSpeedSensor,
            frontRightStatusLabel
        },
        {
            DtcDefinitions::AbsCanCommunication,
            absCanStatusLabel
        },
        {
            DtcDefinitions::PowertrainCanCommunication,
            powertrainCanStatusLabel
        },
        {
            DtcDefinitions::SteeringCanCommunication,
            steeringCanStatusLabel
        },
        {
            DtcDefinitions::EthernetNodeACommunication,
            ethernetNodeAStatusLabel
        }
    };

    for (
        const FaultStatusBinding& binding :
        bindings
        )
    {
        const bool active =
            dtcManager.isDtcActive(
                binding.code
            );

        binding.label->setText(
            active
            ? "● FAULT ACTIVE"
            : "● HEALTHY"
        );

        refreshDynamicStyle(
            binding.label,
            active
            ? "warning"
            : "healthy"
        );
    }
}

void FaultInjectionWidget::updateLiveMonitor(
    const DtcManager& dtcManager)
{
    const bool frontLeftFault =
        dtcManager.isDtcActive(
            DtcDefinitions::FrontLeftWheelSpeedSensor
        );

    const bool frontRightFault =
        dtcManager.isDtcActive(
            DtcDefinitions::FrontRightWheelSpeedSensor
        );

    const bool absCanFault =
        dtcManager.isDtcActive(
            DtcDefinitions::AbsCanCommunication
        );

    const bool powertrainCanFault =
        dtcManager.isDtcActive(
            DtcDefinitions::PowertrainCanCommunication
        );

    const bool steeringCanFault =
        dtcManager.isDtcActive(
            DtcDefinitions::SteeringCanCommunication
        );

    const bool ethernetFault =
        dtcManager.isDtcActive(
            DtcDefinitions::EthernetNodeACommunication
        );

    struct MonitorValue
    {
        QLabel* label;
        bool fault;
        const char* healthyText;
        const char* faultText;
    };

    const MonitorValue values[] =
    {
        {
            monitorFrontLeftLabel,
            frontLeftFault,
            "VALID",
            "INVALID"
        },
        {
            monitorFrontRightLabel,
            frontRightFault,
            "VALID",
            "INVALID"
        },
        {
            monitorAbsCanLabel,
            absCanFault,
            "ONLINE",
            "DROPOUT"
        },
        {
            monitorPowertrainCanLabel,
            powertrainCanFault,
            "ONLINE",
            "DROPOUT"
        },
        {
            monitorSteeringCanLabel,
            steeringCanFault,
            "ONLINE",
            "DROPOUT"
        },
        {
            monitorEthernetLabel,
            ethernetFault,
            "ONLINE",
            "BLOCKED"
        }
    };

    std::size_t activeCount =
        0;

    for (
        const MonitorValue& value :
        values
        )
    {
        if (value.fault)
        {
            ++activeCount;
        }

        value.label->setText(
            value.fault
            ? QString("● %1").arg(value.faultText)
            : QString("● %1").arg(value.healthyText)
        );

        refreshDynamicStyle(
            value.label,
            value.fault
            ? "warning"
            : "healthy"
        );
    }

    // ECU health is derived from the faults currently modeled by v1.8.
    const bool absEcuDegraded =
        frontLeftFault ||
        frontRightFault ||
        absCanFault;

    monitorPowertrainEcuLabel->setText(
        powertrainCanFault
        ? "● DEGRADED"
        : "● RUNNING"
    );

    refreshDynamicStyle(
        monitorPowertrainEcuLabel,
        powertrainCanFault
        ? "warning"
        : "healthy"
    );

    monitorAbsEcuLabel->setText(
        absEcuDegraded
        ? "● DEGRADED"
        : "● HEALTHY"
    );

    refreshDynamicStyle(
        monitorAbsEcuLabel,
        absEcuDegraded
        ? "warning"
        : "healthy"
    );

    monitorSteeringEcuLabel->setText(
        steeringCanFault
        ? "● DEGRADED"
        : "● RUNNING"
    );

    refreshDynamicStyle(
        monitorSteeringEcuLabel,
        steeringCanFault
        ? "warning"
        : "healthy"
    );

    monitorDashboardEcuLabel->setText(
        "● RUNNING"
    );

    refreshDynamicStyle(
        monitorDashboardEcuLabel,
        "healthy"
    );

    monitorOverallStateLabel->setText(
        activeCount == 0
        ? "● SYSTEM HEALTHY"
        : QString(
            "● %1 ACTIVE FAULT%2"
        )
        .arg(
            activeCount
        )
        .arg(
            activeCount == 1
            ? ""
            : "S"
        )
    );

    refreshDynamicStyle(
        monitorOverallStateLabel,
        activeCount == 0
        ? "healthy"
        : "warning"
    );
}

void FaultInjectionWidget::updateRecommendedAction(
    std::uint32_t dtcCode)
{
    QString title;
    QString impact;
    QString action;

    switch (dtcCode)
    {
    case DtcDefinitions::FrontLeftWheelSpeedSensor:
        title =
            "Front Left Wheel Speed Sensor";

        impact =
            "The ABS ECU receives an invalid front-left wheel-speed signal. ABS health becomes degraded and ABS intervention is inhibited.";

        action =
            "Recommended action: verify the front-left wheel-speed signal, restore the sensor input, then use Clear Fault. Confirm the wheel state returns to VALID and ABS health returns to HEALTHY.";
        break;

    case DtcDefinitions::FrontRightWheelSpeedSensor:
        title =
            "Front Right Wheel Speed Sensor";

        impact =
            "The ABS ECU receives an invalid front-right wheel-speed signal. ABS health becomes degraded and ABS intervention is inhibited.";

        action =
            "Recommended action: verify the front-right wheel-speed signal, restore the sensor input, then use Clear Fault. Confirm the wheel state returns to VALID and ABS health returns to HEALTHY.";
        break;

    case DtcDefinitions::AbsCanCommunication:
        title =
            "ABS CAN Communication";

        impact =
            "Periodic CAN frame 0x080 (ABS_WHEEL_STATE) is suppressed. Other ECU traffic remains operational.";

        action =
            "Recommended action: open CAN Monitor and confirm 0x080 is missing while 0x100 and 0x120 continue. Clear the fault, then verify 0x080 transmission resumes.";
        break;

    case DtcDefinitions::PowertrainCanCommunication:
        title =
            "Powertrain CAN Communication";

        impact =
            "Periodic CAN frame 0x100 (POWERTRAIN_STATE) is suppressed. Dashboard data sourced from this message may become stale.";

        action =
            "Recommended action: open CAN Monitor and confirm 0x100 is missing while 0x080 and 0x120 continue. Clear the fault, then verify 0x100 transmission resumes.";
        break;

    case DtcDefinitions::SteeringCanCommunication:
        title =
            "Steering CAN Communication";

        impact =
            "Periodic CAN frame 0x120 (STEERING_STATE) is suppressed while unrelated CAN traffic remains operational.";

        action =
            "Recommended action: open CAN Monitor and confirm 0x120 is missing while 0x080 and 0x100 continue. Clear the fault, then verify 0x120 transmission resumes.";
        break;

    case DtcDefinitions::EthernetNodeACommunication:
        title =
            "Ethernet Node A Communication";

        impact =
            "Ethernet frames using Node A as source or destination are blocked by the runtime submission path.";

        action =
            "Recommended action: use the Network page to attempt Node A communication and confirm it is blocked. Clear the fault and verify Node A communication is accepted again.";
        break;

    default:
        title =
            "Diagnostic Trouble Code";

        impact =
            "An active diagnostic condition is present.";

        action =
            "Recommended action: review the DTC description, identify the affected subsystem, restore the injected condition, and verify recovery after clearing the fault.";
        break;
    }

    selectedFaultTitleLabel->setText(
        title
    );

    selectedFaultImpactLabel->setText(
        impact
    );

    selectedFaultActionLabel->setText(
        action
    );
}

void FaultInjectionWidget::refreshDynamicStyle(
    QLabel* label,
    const char* state)
{
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

    label->update();
}

QString FaultInjectionWidget::formatDtcCode(
    std::uint32_t code)
{
    return QString(
        "0x%1"
    )
        .arg(
            code,
            5,
            16,
            QChar('0')
        )
        .toUpper();
}

QString FaultInjectionWidget::formatBytes(
    const UdsRequest& request)
{
    QString text =
        QString("%1")
        .arg(
            request.serviceId,
            2,
            16,
            QChar('0')
        )
        .toUpper();

    for (
        const std::uint8_t byte :
    request.payload
        )
    {
        text +=
            " " +
            QString("%1")
            .arg(
                byte,
                2,
                16,
                QChar('0')
            )
            .toUpper();
    }

    return text;
}

QString FaultInjectionWidget::formatBytes(
    const UdsResponse& response)
{
    QString text =
        QString("%1")
        .arg(
            response.serviceId,
            2,
            16,
            QChar('0')
        )
        .toUpper();

    for (
        const std::uint8_t byte :
    response.payload
        )
    {
        text +=
            " " +
            QString("%1")
            .arg(
                byte,
                2,
                16,
                QChar('0')
            )
            .toUpper();
    }

    return text;
}

QString FaultInjectionWidget::dtcStatusText(
    DtcStatus status)
{
    switch (status)
    {
    case DtcStatus::Inactive:
        return "INACTIVE";

    case DtcStatus::Pending:
        return "PENDING";

    case DtcStatus::Active:
        return "ACTIVE";

    case DtcStatus::Cleared:
        return "CLEARED";
    }

    return "UNKNOWN";
}

QString FaultInjectionWidget::sessionText(
    std::uint8_t session)
{
    if (
        session ==
        UdsService::ExtendedSession
        )
    {
        return "EXTENDED  (0x03)";
    }

    return "DEFAULT  (0x01)";
}
