#include "StatisticsWidget.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <QAbstractItemView>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace
{
    QString formatEtherType(
        std::uint16_t etherType)
    {
        return
            QString(
                "0x%1"
            )
            .arg(
                etherType,
                4,
                16,
                QLatin1Char('0')
            )
            .toUpper();
    }

    QString formatMac(
        const MacAddress& macAddress)
    {
        return
            QString::fromStdString(
                macAddress.toString()
            );
    }

    QString formatPayload(
        const std::vector<std::uint8_t>& payload)
    {
        if (payload.empty())
        {
            return "--";
        }

        QString text;

        const std::size_t displayCount =
            std::min<std::size_t>(
                payload.size(),
                12
            );

        for (
            std::size_t index = 0;
            index < displayCount;
            ++index
            )
        {
            if (!text.isEmpty())
            {
                text += " ";
            }

            text +=
                QString(
                    "%1"
                )
                .arg(
                    payload[index],
                    2,
                    16,
                    QLatin1Char('0')
                )
                .toUpper();
        }

        if (
            payload.size() >
            displayCount
            )
        {
            text += " …";
        }

        return text;
    }

    QString formatByteCount(
        std::size_t bytes)
    {
        if (
            bytes <
            1024
            )
        {
            return
                QString::number(
                    static_cast<qulonglong>(
                        bytes
                        )
                ) +
                " B";
        }

        return
            QString::number(
                static_cast<double>(
                    bytes
                    ) /
                1024.0,
                'f',
                2
            ) +
            " KB";
    }

    QFrame* createSummaryCard(
        const QString& icon,
        const QString& title,
        QLabel*& valueLabel)
    {
        QFrame* card =
            new QFrame();

        card->setObjectName(
            "NetworkSummaryCard"
        );

        QHBoxLayout* root =
            new QHBoxLayout(
                card
            );

        root->setContentsMargins(
            12,
            9,
            12,
            9
        );

        root->setSpacing(
            10
        );

        QLabel* iconLabel =
            new QLabel(
                icon
            );

        iconLabel->setObjectName(
            "NetworkSummaryIcon"
        );

        iconLabel->setAlignment(
            Qt::AlignCenter
        );

        iconLabel->setFixedWidth(
            28
        );

        QVBoxLayout* textLayout =
            new QVBoxLayout();

        textLayout->setSpacing(
            2
        );

        QLabel* titleLabel =
            new QLabel(
                title
            );

        titleLabel->setObjectName(
            "NetworkSummaryTitle"
        );

        valueLabel =
            new QLabel(
                "--"
            );

        valueLabel->setObjectName(
            "NetworkSummaryValue"
        );

        textLayout->addWidget(
            titleLabel
        );

        textLayout->addWidget(
            valueLabel
        );

        root->addWidget(
            iconLabel
        );

        root->addLayout(
            textLayout,
            1
        );

        return card;
    }

    QFrame* createCompactEcuCard(
        const QString& title,
        const QString& detail,
        QLabel*& statusLabel)
    {
        QFrame* card =
            new QFrame();

        card->setObjectName(
            "NetworkEcuCard"
        );

        QGridLayout* layout =
            new QGridLayout(
                card
            );

        layout->setContentsMargins(
            10,
            7,
            10,
            7
        );

        layout->setHorizontalSpacing(
            8
        );

        layout->setVerticalSpacing(
            1
        );

        QLabel* titleLabel =
            new QLabel(
                title
            );

        titleLabel->setObjectName(
            "NetworkCardTitle"
        );

        QLabel* detailLabel =
            new QLabel(
                detail
            );

        detailLabel->setObjectName(
            "NetworkMutedText"
        );

        statusLabel =
            new QLabel(
                "● ONLINE"
            );

        statusLabel->setObjectName(
            "NetworkStatus"
        );

        statusLabel->setAlignment(
            Qt::AlignRight |
            Qt::AlignVCenter
        );

        layout->addWidget(
            titleLabel,
            0,
            0
        );

        layout->addWidget(
            statusLabel,
            0,
            1
        );

        layout->addWidget(
            detailLabel,
            1,
            0,
            1,
            2
        );

        layout->setColumnStretch(
            0,
            1
        );

        return card;
    }

    QFrame* createEthernetEndpointCard(
        const QString& title,
        QLabel*& macLabel,
        QLabel*& statusLabel)
    {
        QFrame* card =
            new QFrame();

        card->setObjectName(
            "NetworkEndpointCard"
        );

        QVBoxLayout* layout =
            new QVBoxLayout(
                card
            );

        layout->setContentsMargins(
            12,
            10,
            12,
            10
        );

        layout->setSpacing(
            4
        );

        QLabel* titleLabel =
            new QLabel(
                title
            );

        titleLabel->setObjectName(
            "NetworkCardTitle"
        );

        macLabel =
            new QLabel(
                "--"
            );

        macLabel->setObjectName(
            "NetworkMonospace"
        );

        statusLabel =
            new QLabel(
                "● ONLINE"
            );

        statusLabel->setObjectName(
            "NetworkStatus"
        );

        layout->addWidget(
            titleLabel
        );

        layout->addWidget(
            macLabel
        );

        layout->addSpacing(
            2
        );

        layout->addWidget(
            statusLabel
        );

        return card;
    }

    QWidget* createMetricRow(
        const QString& title,
        QLabel*& valueLabel)
    {
        QWidget* row =
            new QWidget();

        QHBoxLayout* layout =
            new QHBoxLayout(
                row
            );

        layout->setContentsMargins(
            0,
            2,
            0,
            2
        );

        QLabel* titleLabel =
            new QLabel(
                title
            );

        titleLabel->setObjectName(
            "NetworkMutedText"
        );

        valueLabel =
            new QLabel(
                "--"
            );

        valueLabel->setObjectName(
            "NetworkMetricValue"
        );

        layout->addWidget(
            titleLabel
        );

        layout->addStretch();

        layout->addWidget(
            valueLabel
        );

        return row;
    }

    QFrame* createHorizontalLine()
    {
        QFrame* line =
            new QFrame();

        line->setObjectName(
            "NetworkTopologyLine"
        );

        line->setFrameShape(
            QFrame::HLine
        );

        line->setFixedHeight(
            2
        );

        return line;
    }
}

// ============================================================
// Constructor
// ============================================================

StatisticsWidget::StatisticsWidget(
    QWidget* parent)
    : QGroupBox(
        "Network Overview",
        parent
    )
{
    setObjectName(
        "NetworkWorkstation"
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

    // ========================================================
    // Summary cards
    // ========================================================

    QHBoxLayout* summaryLayout =
        new QHBoxLayout();

    summaryLayout->setSpacing(
        10
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "⌘",
            "Ethernet Status",
            ethernetStatusValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "⌁",
            "Link Rate",
            ethernetRateValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "▤",
            "Ethernet Frames",
            ethernetFramesValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "◉",
            "Payload Bytes",
            ethernetPayloadBytesValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "⌘",
            "Ethernet Nodes",
            ethernetNodesValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "♡",
            "Network Health",
            networkHealthValueLabel
        )
    );

    mainLayout->addLayout(
        summaryLayout
    );

    // ========================================================
    // Tabs
    // ========================================================

    QTabWidget* tabs =
        new QTabWidget();

    tabs->setObjectName(
        "NetworkWorkspaceTabs"
    );

    tabs->setDocumentMode(
        false
    );

    mainLayout->addWidget(
        tabs,
        1
    );

    // ========================================================
    // Overview / Topology
    // ========================================================

    QWidget* overviewPage =
        new QWidget();

    overviewPage->setObjectName(
        "NetworkTabPage"
    );

    QVBoxLayout* overviewLayout =
        new QVBoxLayout(
            overviewPage
        );

    overviewLayout->setContentsMargins(
        10,
        10,
        10,
        10
    );

    overviewLayout->setSpacing(
        12
    );

    // --------------------------------------------------------
    // Main topology panel
    // --------------------------------------------------------

    QGroupBox* topologyGroup =
        new QGroupBox(
            "Network Topology"
        );

    topologyGroup->setObjectName(
        "NetworkSection"
    );

    QHBoxLayout* topologyRoot =
        new QHBoxLayout(
            topologyGroup
        );

    topologyRoot->setContentsMargins(
        12,
        20,
        12,
        12
    );

    topologyRoot->setSpacing(
        18
    );

    // --------------------------------------------------------
    // CAN side
    // --------------------------------------------------------

    QFrame* canArea =
        new QFrame();

    canArea->setObjectName(
        "NetworkTopologyArea"
    );

    QVBoxLayout* canAreaLayout =
        new QVBoxLayout(
            canArea
        );

    canAreaLayout->setContentsMargins(
        12,
        10,
        12,
        10
    );

    canAreaLayout->setSpacing(
        8
    );

    QHBoxLayout* canTitleRow =
        new QHBoxLayout();

    QLabel* canIcon =
        new QLabel(
            "⌘"
        );

    canIcon->setObjectName(
        "NetworkAreaIcon"
    );

    QLabel* canTitle =
        new QLabel(
            "Vehicle ECUs (CAN)"
        );

    canTitle->setObjectName(
        "NetworkAreaTitle"
    );

    canTitleRow->addWidget(
        canIcon
    );

    canTitleRow->addWidget(
        canTitle
    );

    canTitleRow->addStretch();

    canAreaLayout->addLayout(
        canTitleRow
    );

    QHBoxLayout* canBody =
        new QHBoxLayout();

    canBody->setSpacing(
        12
    );

    QVBoxLayout* ecuColumn =
        new QVBoxLayout();

    ecuColumn->setSpacing(
        7
    );

    ecuColumn->addWidget(
        createCompactEcuCard(
            "Powertrain ECU",
            "0x100  ·  POWERTRAIN_STATE",
            powertrainNodeStatusLabel
        )
    );

    ecuColumn->addWidget(
        createCompactEcuCard(
            "ABS ECU",
            "0x080  ·  ABS_WHEEL_STATE",
            absNodeStatusLabel
        )
    );

    ecuColumn->addWidget(
        createCompactEcuCard(
            "Steering ECU",
            "0x120  ·  STEERING_STATE",
            steeringNodeStatusLabel
        )
    );

    ecuColumn->addWidget(
        createCompactEcuCard(
            "Dashboard ECU",
            "CAN state monitoring",
            dashboardNodeStatusLabel
        )
    );

    QWidget* canBackbone =
        new QWidget();

    QVBoxLayout* canBackboneLayout =
        new QVBoxLayout(
            canBackbone
        );

    canBackboneLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    canBackboneLayout->addStretch();

    QLabel* canBusCard =
        new QLabel(
            "CAN Bus\n500 kbps\n● ONLINE"
        );

    canBusCard->setObjectName(
        "NetworkCenterBus"
    );

    canBusCard->setAlignment(
        Qt::AlignCenter
    );

    canBusCard->setMinimumWidth(
        165
    );

    canBusCard->setMinimumHeight(
        88
    );

    canBackboneLayout->addWidget(
        canBusCard
    );

    QLabel* diagnosticCard =
        new QLabel(
            "Diagnostic Client\n0x7E0 ↔ 0x7E8"
        );

    diagnosticCard->setObjectName(
        "NetworkDiagnosticCard"
    );

    diagnosticCard->setAlignment(
        Qt::AlignCenter
    );

    diagnosticCard->setMinimumHeight(
        60
    );

    canBackboneLayout->addWidget(
        diagnosticCard
    );

    canBackboneLayout->addStretch();

    canBody->addLayout(
        ecuColumn,
        3
    );

    canBody->addWidget(
        canBackbone,
        2
    );

    canAreaLayout->addLayout(
        canBody,
        1
    );

    topologyRoot->addWidget(
        canArea,
        5
    );

    // --------------------------------------------------------
    // Ethernet side
    // --------------------------------------------------------

    QFrame* ethernetArea =
        new QFrame();

    ethernetArea->setObjectName(
        "NetworkTopologyArea"
    );

    QVBoxLayout* ethernetAreaLayout =
        new QVBoxLayout(
            ethernetArea
        );

    ethernetAreaLayout->setContentsMargins(
        12,
        10,
        12,
        10
    );

    ethernetAreaLayout->setSpacing(
        10
    );

    QHBoxLayout* ethernetTitleRow =
        new QHBoxLayout();

    QLabel* ethernetIcon =
        new QLabel(
            "⌘"
        );

    ethernetIcon->setObjectName(
        "NetworkAreaIcon"
    );

    QLabel* ethernetTitle =
        new QLabel(
            "Automotive Ethernet (SIL)"
        );

    ethernetTitle->setObjectName(
        "NetworkAreaTitle"
    );

    ethernetTitleRow->addWidget(
        ethernetIcon
    );

    ethernetTitleRow->addWidget(
        ethernetTitle
    );

    ethernetTitleRow->addStretch();

    ethernetAreaLayout->addLayout(
        ethernetTitleRow
    );

    QHBoxLayout* ethernetPath =
        new QHBoxLayout();

    ethernetPath->setSpacing(
        10
    );

    QFrame* nodeA =
        createEthernetEndpointCard(
            "Ethernet Node A",
            ethernetNodeAMacLabel,
            ethernetNodeAStatusLabel
        );

    QFrame* nodeB =
        createEthernetEndpointCard(
            "Ethernet Node B",
            ethernetNodeBMacLabel,
            ethernetNodeBStatusLabel
        );

    QLabel* ethernetBusCard =
        new QLabel(
            "Virtual Ethernet Bus\n100 Mbps\n● ONLINE"
        );

    ethernetBusCard->setObjectName(
        "NetworkEthernetBus"
    );

    ethernetBusCard->setAlignment(
        Qt::AlignCenter
    );

    ethernetBusCard->setMinimumWidth(
        205
    );

    ethernetBusCard->setMinimumHeight(
        92
    );

    ethernetBusStatusLabel =
        ethernetBusCard;

    ethernetPath->addWidget(
        nodeA,
        2
    );

    ethernetPath->addWidget(
        createHorizontalLine(),
        1
    );

    ethernetPath->addWidget(
        ethernetBusCard,
        3
    );

    ethernetPath->addWidget(
        createHorizontalLine(),
        1
    );

    ethernetPath->addWidget(
        nodeB,
        2
    );

    ethernetAreaLayout->addStretch();

    ethernetAreaLayout->addLayout(
        ethernetPath
    );

    QLabel* deliveryBanner =
        new QLabel(
            "ⓘ  Frames are delivered by destination MAC through the deterministic virtual Ethernet bus."
        );

    deliveryBanner->setObjectName(
        "NetworkInfoBanner"
    );

    deliveryBanner->setWordWrap(
        true
    );

    ethernetAreaLayout->addWidget(
        deliveryBanner
    );

    ethernetAreaLayout->addStretch();

    topologyRoot->addWidget(
        ethernetArea,
        6
    );

    overviewLayout->addWidget(
        topologyGroup,
        1
    );

    // --------------------------------------------------------
    // Bottom information strip
    // --------------------------------------------------------

    QFrame* bottomPanel =
        new QFrame();

    bottomPanel->setObjectName(
        "NetworkBottomPanel"
    );

    QHBoxLayout* bottomLayout =
        new QHBoxLayout(
            bottomPanel
        );

    bottomLayout->setContentsMargins(
        14,
        10,
        14,
        10
    );

    bottomLayout->setSpacing(
        16
    );

    // Network notes
    QWidget* notesArea =
        new QWidget();

    QVBoxLayout* notesLayout =
        new QVBoxLayout(
            notesArea
        );

    notesLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    notesLayout->setSpacing(
        4
    );

    QLabel* notesTitle =
        new QLabel(
            "▣  Network Notes"
        );

    notesTitle->setObjectName(
        "NetworkCardTitle"
    );

    QLabel* notes =
        new QLabel(
            "• Vehicle ECU CAN traffic is analyzed in the dedicated CAN Monitor page.\n"
            "• Ethernet Node A and Node B communicate over the deterministic virtual Ethernet bus.\n"
            "• Use the Ethernet Monitor tab to send test frames and inspect Ethernet traffic."
        );

    notes->setObjectName(
        "NetworkMutedText"
    );

    notes->setWordWrap(
        true
    );

    notesLayout->addWidget(
        notesTitle
    );

    notesLayout->addWidget(
        notes
    );

    bottomLayout->addWidget(
        notesArea,
        5
    );

    QFrame* dividerA =
        new QFrame();

    dividerA->setObjectName(
        "NetworkVerticalDivider"
    );

    dividerA->setFrameShape(
        QFrame::VLine
    );

    bottomLayout->addWidget(
        dividerA
    );

    // Link health
    QWidget* healthArea =
        new QWidget();

    QVBoxLayout* healthLayout =
        new QVBoxLayout(
            healthArea
        );

    healthLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    healthLayout->setSpacing(
        5
    );

    QLabel* healthTitle =
        new QLabel(
            "⌁  Ethernet Link Status"
        );

    healthTitle->setObjectName(
        "NetworkCardTitle"
    );

    ethernetLinkStatusValueLabel =
        new QLabel(
            "● Healthy"
        );

    ethernetLinkStatusValueLabel->setObjectName(
        "NetworkStatusLarge"
    );

    ethernetLinkDescriptionLabel =
        new QLabel(
            "No faults detected. Ethernet communication is operating normally."
        );

    ethernetLinkDescriptionLabel->setObjectName(
        "NetworkMutedText"
    );

    ethernetLinkDescriptionLabel->setWordWrap(
        true
    );

    healthLayout->addWidget(
        healthTitle
    );

    healthLayout->addWidget(
        ethernetLinkStatusValueLabel
    );

    healthLayout->addWidget(
        ethernetLinkDescriptionLabel
    );

    bottomLayout->addWidget(
        healthArea,
        3
    );

    QFrame* dividerB =
        new QFrame();

    dividerB->setObjectName(
        "NetworkVerticalDivider"
    );

    dividerB->setFrameShape(
        QFrame::VLine
    );

    bottomLayout->addWidget(
        dividerB
    );

    QLabel* faultHint =
        new QLabel(
            "🔧  To inject communication faults, go to\nDiagnostics → Fault Injection."
        );

    faultHint->setObjectName(
        "NetworkFaultHint"
    );

    faultHint->setAlignment(
        Qt::AlignCenter
    );

    faultHint->setWordWrap(
        true
    );

    bottomLayout->addWidget(
        faultHint,
        3
    );

    overviewLayout->addWidget(
        bottomPanel
    );

    tabs->addTab(
        overviewPage,
        "Overview / Topology"
    );

    // ========================================================
    // Ethernet Monitor
    // ========================================================

    QWidget* ethernetPage =
        new QWidget();

    ethernetPage->setObjectName(
        "NetworkTabPage"
    );

    QVBoxLayout* ethernetPageLayout =
        new QVBoxLayout(
            ethernetPage
        );

    ethernetPageLayout->setContentsMargins(
        10,
        10,
        10,
        10
    );

    ethernetPageLayout->setSpacing(
        12
    );

    QHBoxLayout* ethernetMetrics =
        new QHBoxLayout();

    ethernetMetrics->setSpacing(
        10
    );

    ethernetMetrics->addWidget(
        createSummaryCard(
            "▣",
            "Pending Frames",
            ethernetPendingFramesValueLabel
        )
    );

    ethernetMetrics->addWidget(
        createSummaryCard(
            "◷",
            "Busy Until",
            ethernetBusyUntilValueLabel
        )
    );

    ethernetPageLayout->addLayout(
        ethernetMetrics
    );

    QHBoxLayout* nodeActivity =
        new QHBoxLayout();

    nodeActivity->setSpacing(
        12
    );

    QGroupBox* nodeAActivityGroup =
        new QGroupBox(
            "Ethernet Node A"
        );

    nodeAActivityGroup->setObjectName(
        "NetworkSection"
    );

    QVBoxLayout* nodeAActivity =
        new QVBoxLayout(
            nodeAActivityGroup
        );

    nodeAActivity->setContentsMargins(
        12,
        18,
        12,
        10
    );

    nodeAActivity->addWidget(
        createMetricRow(
            "Frames TX",
            ethernetNodeATxValueLabel
        )
    );

    nodeAActivity->addWidget(
        createMetricRow(
            "Frames RX",
            ethernetNodeARxValueLabel
        )
    );

    sendAToBButton =
        new QPushButton(
            "Send Test Frame  A → B"
        );

    sendAToBButton->setObjectName(
        "NetworkPrimaryButton"
    );

    nodeAActivity->addWidget(
        sendAToBButton
    );

    QGroupBox* nodeBActivityGroup =
        new QGroupBox(
            "Ethernet Node B"
        );

    nodeBActivityGroup->setObjectName(
        "NetworkSection"
    );

    QVBoxLayout* nodeBActivity =
        new QVBoxLayout(
            nodeBActivityGroup
        );

    nodeBActivity->setContentsMargins(
        12,
        18,
        12,
        10
    );

    nodeBActivity->addWidget(
        createMetricRow(
            "Frames TX",
            ethernetNodeBTxValueLabel
        )
    );

    nodeBActivity->addWidget(
        createMetricRow(
            "Frames RX",
            ethernetNodeBRxValueLabel
        )
    );

    sendBToAButton =
        new QPushButton(
            "Send Test Frame  B → A"
        );

    sendBToAButton->setObjectName(
        "NetworkPrimaryButton"
    );

    nodeBActivity->addWidget(
        sendBToAButton
    );

    nodeActivity->addWidget(
        nodeAActivityGroup,
        1
    );

    nodeActivity->addWidget(
        nodeBActivityGroup,
        1
    );

    ethernetPageLayout->addLayout(
        nodeActivity
    );

    QGroupBox* traceGroup =
        new QGroupBox(
            "Ethernet Frame Trace"
        );

    traceGroup->setObjectName(
        "NetworkSection"
    );

    QVBoxLayout* traceLayout =
        new QVBoxLayout(
            traceGroup
        );

    traceLayout->setContentsMargins(
        12,
        18,
        12,
        10
    );

    ethernetTraceTable =
        new QTableWidget(
            0,
            7
        );

    ethernetTraceTable->setHorizontalHeaderLabels(
        {
            "TX Start",
            "Source MAC",
            "Destination MAC",
            "EtherType",
            "Payload",
            "Wait",
            "TX Time"
        }
    );

    ethernetTraceTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    ethernetTraceTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    ethernetTraceTable->setAlternatingRowColors(
        true
    );

    ethernetTraceTable->setShowGrid(
        false
    );

    ethernetTraceTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            0,
            QHeaderView::ResizeToContents
        );

    ethernetTraceTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            1,
            QHeaderView::ResizeToContents
        );

    ethernetTraceTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            2,
            QHeaderView::ResizeToContents
        );

    ethernetTraceTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            3,
            QHeaderView::ResizeToContents
        );

    ethernetTraceTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            4,
            QHeaderView::Stretch
        );

    ethernetTraceTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            5,
            QHeaderView::ResizeToContents
        );

    ethernetTraceTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            6,
            QHeaderView::ResizeToContents
        );

    ethernetTraceTable
        ->verticalHeader()
        ->setVisible(
            false
        );

    traceLayout->addWidget(
        ethernetTraceTable,
        1
    );

    ethernetPageLayout->addWidget(
        traceGroup,
        1
    );

    tabs->addTab(
        ethernetPage,
        "Ethernet Monitor"
    );

    // ========================================================
    // Styling
    // ========================================================

    applyThemeStyle();
}


// ============================================================
// Theme
// ============================================================

void StatisticsWidget::setDarkMode(
    bool enabled)
{
    darkModeEnabled =
        enabled;

    applyThemeStyle();
}

void StatisticsWidget::applyThemeStyle()
{
    if (darkModeEnabled)
    {
        setStyleSheet(
            R"(
            QGroupBox#NetworkWorkstation
            {
                background: transparent;
            }

            QTabWidget#NetworkWorkspaceTabs::pane
            {
                border: 1px solid #2D3C4A;
                border-radius: 9px;
                background-color: transparent;
                top: -1px;
            }

            QTabWidget#NetworkWorkspaceTabs QTabBar::tab
            {
                min-width: 160px;
                min-height: 36px;
                padding: 5px 16px;
                margin-right: 6px;
                border: 1px solid #314252;
                border-radius: 7px;
                background-color: #172431;
                color: #91A4B7;
            }

            QTabWidget#NetworkWorkspaceTabs QTabBar::tab:hover
            {
                background-color: #1E3040;
                color: #D6E2ED;
            }

            QTabWidget#NetworkWorkspaceTabs QTabBar::tab:selected
            {
                background-color: #456E97;
                color: #FFFFFF;
                border-color: #5B86AE;
                font-weight: 700;
            }

            QWidget#NetworkTabPage
            {
                background: transparent;
            }

            QGroupBox#NetworkSection
            {
                border: 1px solid #304050;
                border-radius: 9px;
                margin-top: 10px;
                padding-top: 12px;
                font-weight: 700;
                color: #E6EDF4;
            }

            QGroupBox#NetworkSection::title
            {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px;
                color: #E6EDF4;
            }

            QFrame#NetworkSummaryCard,
            QFrame#NetworkEcuCard,
            QFrame#NetworkEndpointCard,
            QFrame#NetworkTopologyArea,
            QFrame#NetworkBottomPanel
            {
                border: 1px solid #304050;
                border-radius: 8px;
                background-color: #16222E;
            }

            QFrame#NetworkTopologyArea
            {
                border-style: dashed;
                background-color: #111B24;
            }

            QFrame#NetworkBottomPanel
            {
                background-color: #16222E;
            }

            QLabel#NetworkSummaryIcon,
            QLabel#NetworkAreaIcon
            {
                color: #69A8E4;
                font-size: 20px;
                font-weight: 700;
            }

            QLabel#NetworkSummaryTitle,
            QLabel#NetworkMutedText
            {
                color: #8EA2B5;
            }

            QLabel#NetworkSummaryValue,
            QLabel#NetworkMetricValue,
            QLabel#NetworkCardTitle,
            QLabel#NetworkAreaTitle
            {
                color: #EAF1F7;
                font-weight: 700;
            }

            QLabel#NetworkAreaTitle
            {
                font-size: 13px;
            }

            QLabel#NetworkStatus,
            QLabel#NetworkStatusLarge
            {
                font-weight: 700;
            }

            QLabel#NetworkStatusLarge
            {
                font-size: 14px;
            }

            QLabel#NetworkCenterBus,
            QLabel#NetworkEthernetBus
            {
                color: #DCE8F2;
                background-color: #1D2B38;
                border: 1px solid #3A5267;
                border-radius: 7px;
                padding: 10px;
                font-weight: 700;
            }

            QLabel#NetworkEthernetBus
            {
                color: #8BC4F4;
                border-color: #426B8E;
                background-color: #163047;
            }

            QLabel#NetworkDiagnosticCard
            {
                color: #C8D5E0;
                background-color: #1A2632;
                border: 1px solid #334454;
                border-radius: 7px;
                padding: 8px;
                font-weight: 600;
            }

            QLabel#NetworkInfoBanner
            {
                color: #A9BED0;
                background-color: #172A3B;
                border: 1px solid #2D526E;
                border-radius: 7px;
                padding: 9px;
            }

            QLabel#NetworkFaultHint
            {
                color: #8EC1EC;
                background-color: #172A3B;
                border: 1px solid #2D526E;
                border-radius: 7px;
                padding: 10px;
                font-weight: 600;
            }

            QLabel#NetworkMonospace
            {
                color: #A8BDD0;
                font-family: Consolas, "Courier New", monospace;
            }

            QLabel[state="healthy"]
            {
                color: #43D17D;
            }

            QLabel[state="warning"]
            {
                color: #F08A53;
            }

            QFrame#NetworkTopologyLine
            {
                color: #42A4E2;
                background-color: #42A4E2;
                border: none;
                min-width: 24px;
                max-height: 2px;
            }

            QFrame#NetworkVerticalDivider
            {
                color: #304050;
                background-color: #304050;
                max-width: 1px;
            }

            QPushButton#NetworkPrimaryButton
            {
                min-height: 36px;
                font-weight: 600;
                color: #E7EEF5;
                background-color: #1A2733;
                border: 1px solid #34495A;
                border-radius: 6px;
            }

            QPushButton#NetworkPrimaryButton:hover
            {
                background-color: #233544;
                border-color: #4B667B;
            }

            QTableWidget
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

            )"
        );

        return;
    }

    setStyleSheet(
        R"(
            QGroupBox#NetworkWorkstation
            {
                background: transparent;
            }

            QTabWidget#NetworkWorkspaceTabs::pane
            {
                border: 1px solid #DCE3EA;
                border-radius: 9px;
                background-color: transparent;
                top: -1px;
            }

            QTabWidget#NetworkWorkspaceTabs QTabBar::tab
            {
                min-width: 160px;
                min-height: 36px;
                padding: 5px 16px;
                margin-right: 6px;
                border: 1px solid #DCE3EA;
                border-radius: 7px;
                background-color: #EEF2F5;
                color: #6C7F92;
            }

            QTabWidget#NetworkWorkspaceTabs QTabBar::tab:hover
            {
                background-color: #E4EBF2;
                color: #2D4053;
            }

            QTabWidget#NetworkWorkspaceTabs QTabBar::tab:selected
            {
                background-color: #456E97;
                color: #FFFFFF;
                border-color: #456E97;
                font-weight: 700;
            }

            QWidget#NetworkTabPage
            {
                background: transparent;
            }

            QGroupBox#NetworkSection
            {
                border: 1px solid #DCE3EA;
                border-radius: 9px;
                margin-top: 10px;
                padding-top: 12px;
                font-weight: 700;
            }

            QGroupBox#NetworkSection::title
            {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px;
                color: #17212B;
            }

            QFrame#NetworkSummaryCard,
            QFrame#NetworkEcuCard,
            QFrame#NetworkEndpointCard,
            QFrame#NetworkTopologyArea,
            QFrame#NetworkBottomPanel
            {
                border: 1px solid #DCE3EA;
                border-radius: 8px;
                background-color: #FFFFFF;
            }

            QFrame#NetworkTopologyArea
            {
                border-style: dashed;
                background-color: #FCFDFE;
            }

            QFrame#NetworkBottomPanel
            {
                background-color: #FFFFFF;
            }

            QLabel#NetworkSummaryIcon,
            QLabel#NetworkAreaIcon
            {
                color: #2D78D4;
                font-size: 20px;
                font-weight: 700;
            }

            QLabel#NetworkSummaryTitle,
            QLabel#NetworkMutedText
            {
                color: #74879A;
            }

            QLabel#NetworkSummaryValue,
            QLabel#NetworkMetricValue,
            QLabel#NetworkCardTitle,
            QLabel#NetworkAreaTitle
            {
                color: #17212B;
                font-weight: 700;
            }

            QLabel#NetworkAreaTitle
            {
                font-size: 13px;
            }

            QLabel#NetworkStatus,
            QLabel#NetworkStatusLarge
            {
                font-weight: 700;
            }

            QLabel#NetworkStatusLarge
            {
                font-size: 14px;
            }

            QLabel#NetworkCenterBus,
            QLabel#NetworkEthernetBus
            {
                color: #2D4053;
                background-color: #EEF4F8;
                border: 1px solid #CAD8E4;
                border-radius: 7px;
                padding: 10px;
                font-weight: 700;
            }

            QLabel#NetworkEthernetBus
            {
                color: #456E97;
                border-color: #9FC5E8;
                background-color: #EFF7FD;
            }

            QLabel#NetworkDiagnosticCard
            {
                color: #425364;
                background-color: #F5F8FA;
                border: 1px solid #DCE3EA;
                border-radius: 7px;
                padding: 8px;
                font-weight: 600;
            }

            QLabel#NetworkInfoBanner
            {
                color: #5E7388;
                background-color: #EFF6FC;
                border: 1px solid #D7E7F4;
                border-radius: 7px;
                padding: 9px;
            }

            QLabel#NetworkFaultHint
            {
                color: #456E97;
                background-color: #EFF6FC;
                border: 1px solid #D7E7F4;
                border-radius: 7px;
                padding: 10px;
                font-weight: 600;
            }

            QLabel#NetworkMonospace
            {
                color: #53697E;
                font-family: Consolas, "Courier New", monospace;
            }

            QLabel[state="healthy"]
            {
                color: #138A4B;
            }

            QLabel[state="warning"]
            {
                color: #C2633A;
            }

            QFrame#NetworkTopologyLine
            {
                color: #2D9CDB;
                background-color: #2D9CDB;
                border: none;
                min-width: 24px;
                max-height: 2px;
            }

            QFrame#NetworkVerticalDivider
            {
                color: #DCE3EA;
                background-color: #DCE3EA;
                max-width: 1px;
            }

            QPushButton#NetworkPrimaryButton
            {
                min-height: 36px;
                font-weight: 600;
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
                font-weight: 700;
            }
        
        )"
    );
}

// ============================================================
// Refresh
// ============================================================

void StatisticsWidget::updateStatistics(
    const CanStatistics&,
    std::uint32_t,
    double,
    const VirtualEthernetBus& ethernetBus,
    const EthernetNode& ethernetNodeA,
    const EthernetNode& ethernetNodeB,
    bool ethernetNodeAFaultActive)
{
    const EthernetStatistics& statistics =
        ethernetBus.getStatistics();

    ethernetStatusValueLabel->setText(
        ethernetNodeAFaultActive
        ? "● DEGRADED"
        : "● ONLINE"
    );

    setStatusStyle(
        ethernetStatusValueLabel,
        ethernetNodeAFaultActive
        ? "warning"
        : "healthy"
    );

    ethernetRateValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                ethernetBus.getLinkRate() /
                1000000ULL
                )
        ) +
        " Mbps"
    );

    ethernetFramesValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                statistics.getFrameCount()
                )
        )
    );

    ethernetPayloadBytesValueLabel->setText(
        formatByteCount(
            statistics.getPayloadByteCount()
        )
    );

    ethernetNodesValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                ethernetBus.getRegisteredNodeCount()
                )
        )
    );

    networkHealthValueLabel->setText(
        ethernetNodeAFaultActive
        ? "● DEGRADED"
        : "● HEALTHY"
    );

    setStatusStyle(
        networkHealthValueLabel,
        ethernetNodeAFaultActive
        ? "warning"
        : "healthy"
    );

    powertrainNodeStatusLabel->setText(
        "● ONLINE"
    );

    absNodeStatusLabel->setText(
        "● ONLINE"
    );

    steeringNodeStatusLabel->setText(
        "● ONLINE"
    );

    dashboardNodeStatusLabel->setText(
        "● ONLINE"
    );

    setStatusStyle(
        powertrainNodeStatusLabel,
        "healthy"
    );

    setStatusStyle(
        absNodeStatusLabel,
        "healthy"
    );

    setStatusStyle(
        steeringNodeStatusLabel,
        "healthy"
    );

    setStatusStyle(
        dashboardNodeStatusLabel,
        "healthy"
    );

    ethernetLinkStatusValueLabel->setText(
        ethernetNodeAFaultActive
        ? "● Degraded"
        : "● Healthy"
    );

    setStatusStyle(
        ethernetLinkStatusValueLabel,
        ethernetNodeAFaultActive
        ? "warning"
        : "healthy"
    );

    ethernetLinkDescriptionLabel->setText(
        ethernetNodeAFaultActive
        ? "Ethernet Node A communication fault is active. Node A traffic is blocked."
        : "No faults detected. Ethernet communication is operating normally."
    );

    ethernetBusStatusLabel->setText(
        ethernetNodeAFaultActive
        ? "Virtual Ethernet Bus\n100 Mbps\n● DEGRADED"
        : "Virtual Ethernet Bus\n100 Mbps\n● ONLINE"
    );

    updateEthernetStatistics(
        ethernetBus,
        ethernetNodeA,
        ethernetNodeB,
        ethernetNodeAFaultActive
    );

    updateEthernetTrace(
        ethernetBus
    );
}

// ============================================================
// Buttons
// ============================================================

QPushButton*
StatisticsWidget::getSendAToBButton() const
{
    return sendAToBButton;
}

QPushButton*
StatisticsWidget::getSendBToAButton() const
{
    return sendBToAButton;
}

// ============================================================
// Ethernet statistics
// ============================================================

void StatisticsWidget::updateEthernetStatistics(
    const VirtualEthernetBus& ethernetBus,
    const EthernetNode& ethernetNodeA,
    const EthernetNode& ethernetNodeB,
    bool ethernetNodeAFaultActive)
{
    const EthernetStatistics& statistics =
        ethernetBus.getStatistics();

    const MacAddress& nodeAMac =
        ethernetNodeA.getMacAddress();

    const MacAddress& nodeBMac =
        ethernetNodeB.getMacAddress();

    ethernetNodeAMacLabel->setText(
        formatMac(
            nodeAMac
        )
    );

    ethernetNodeBMacLabel->setText(
        formatMac(
            nodeBMac
        )
    );

    ethernetNodeAStatusLabel->setText(
        ethernetNodeAFaultActive
        ? "● BLOCKED"
        : "● ONLINE"
    );

    setStatusStyle(
        ethernetNodeAStatusLabel,
        ethernetNodeAFaultActive
        ? "warning"
        : "healthy"
    );

    ethernetNodeBStatusLabel->setText(
        "● ONLINE"
    );

    setStatusStyle(
        ethernetNodeBStatusLabel,
        "healthy"
    );

    ethernetPendingFramesValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                ethernetBus.getTotalPendingFrameCount()
                )
        )
    );

    ethernetBusyUntilValueLabel->setText(
        QString::number(
            ethernetBus.getBusyUntilMs(),
            'f',
            3
        ) +
        " ms"
    );

    const EthernetNodeStatistics&
        nodeAStatistics =
        statistics.getNodeStatistics(
            nodeAMac
        );

    const EthernetNodeStatistics&
        nodeBStatistics =
        statistics.getNodeStatistics(
            nodeBMac
        );

    ethernetNodeATxValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                nodeAStatistics.framesTransmitted
                )
        )
    );

    ethernetNodeBTxValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                nodeBStatistics.framesTransmitted
                )
        )
    );

    ethernetNodeARxValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                ethernetNodeA
                .getReceivedFrames()
                .size()
                )
        )
    );

    ethernetNodeBRxValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                ethernetNodeB
                .getReceivedFrames()
                .size()
                )
        )
    );
}

// ============================================================
// Ethernet trace
// ============================================================

void StatisticsWidget::updateEthernetTrace(
    const VirtualEthernetBus& ethernetBus)
{
    const std::vector<EthernetTraceEntry>& trace =
        ethernetBus.getTrace();

    constexpr std::size_t maxVisibleRows =
        200;

    const std::size_t startIndex =
        trace.size() >
        maxVisibleRows
        ? trace.size() -
        maxVisibleRows
        : 0;

    const std::size_t visibleRows =
        trace.size() -
        startIndex;

    ethernetTraceTable->setRowCount(
        static_cast<int>(
            visibleRows
            )
    );

    int row =
        0;

    for (
        std::size_t index = startIndex;
        index < trace.size();
        ++index
        )
    {
        const EthernetTraceEntry& entry =
            trace[index];

        ethernetTraceTable->setItem(
            row,
            0,
            new QTableWidgetItem(
                QString::number(
                    entry.startTimeMs,
                    'f',
                    3
                ) +
                " ms"
            )
        );

        ethernetTraceTable->setItem(
            row,
            1,
            new QTableWidgetItem(
                formatMac(
                    entry.frame.sourceMac
                )
            )
        );

        ethernetTraceTable->setItem(
            row,
            2,
            new QTableWidgetItem(
                formatMac(
                    entry.frame.destinationMac
                )
            )
        );

        ethernetTraceTable->setItem(
            row,
            3,
            new QTableWidgetItem(
                formatEtherType(
                    entry.frame.etherType
                )
            )
        );

        ethernetTraceTable->setItem(
            row,
            4,
            new QTableWidgetItem(
                formatPayload(
                    entry.frame.payload
                )
            )
        );

        ethernetTraceTable->setItem(
            row,
            5,
            new QTableWidgetItem(
                QString::number(
                    entry.waitingTimeMs,
                    'f',
                    6
                ) +
                " ms"
            )
        );

        ethernetTraceTable->setItem(
            row,
            6,
            new QTableWidgetItem(
                QString::number(
                    entry.transmissionTimeMs,
                    'f',
                    6
                ) +
                " ms"
            )
        );

        ++row;
    }

    if (
        ethernetTraceTable->rowCount() >
        0
        )
    {
        ethernetTraceTable->scrollToBottom();
    }
}

// ============================================================
// Dynamic status
// ============================================================

void StatisticsWidget::setStatusStyle(
    QLabel* label,
    const QString& state)
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
}
