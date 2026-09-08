#include "CanMonitorWidget.h"

#include <algorithm>
#include <fstream>
#include <iomanip>

#include <QAbstractItemView>
#include <QComboBox>
#include <QColor>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QProgressBar>
#include <QPixmap>
#include <QSet>
#include <QSplitter>
#include <QStyle>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include "../../can/CanMessageRegistry.h"

namespace
{
    QFrame* createSummaryCard(
        const QString& title,
        QLabel*& valueLabel)
    {
        QFrame* card =
            new QFrame();

        card->setObjectName(
            "CanSummaryCard"
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
            "CanSummaryLabel"
        );

        valueLabel =
            new QLabel(
                "--"
            );

        valueLabel->setObjectName(
            "CanSummaryValue"
        );

        layout->addWidget(
            titleLabel
        );

        layout->addWidget(
            valueLabel
        );

        return card;
    }

    QWidget* createInspectorField(
        const QString& title,
        QLabel*& valueLabel)
    {
        QFrame* field =
            new QFrame();

        field->setObjectName(
            "CanInspectorField"
        );

        QVBoxLayout* layout =
            new QVBoxLayout(
                field
            );

        layout->setContentsMargins(
            10,
            8,
            10,
            8
        );

        layout->setSpacing(
            3
        );

        QLabel* titleLabel =
            new QLabel(
                title
            );

        titleLabel->setObjectName(
            "CanInspectorLabel"
        );

        valueLabel =
            new QLabel(
                "--"
            );

        valueLabel->setObjectName(
            "CanInspectorValue"
        );

        valueLabel->setTextInteractionFlags(
            Qt::TextSelectableByMouse
        );

        layout->addWidget(
            titleLabel
        );

        layout->addWidget(
            valueLabel
        );

        return field;
    }

    QString formatCanId(
        std::uint32_t id)
    {
        return QString(
            "0x%1"
        )
            .arg(
                id,
                3,
                16,
                QChar('0')
            )
            .toUpper();
    }

    QColor messageAccentColor(
        std::uint32_t arbitrationId)
    {
        switch (arbitrationId)
        {
        case 0x080:
            return QColor(
                "#3D82BE"
            );

        case 0x100:
            return QColor(
                "#5C8D76"
            );

        case 0x120:
            return QColor(
                "#8A6FA8"
            );

        case 0x7E0:
            return QColor(
                "#C58A3A"
            );

        case 0x7E8:
            return QColor(
                "#B66B54"
            );

        default:
            return QColor(
                "#7D8C99"
            );
        }
    }

    QIcon createMessageDotIcon(
        const QColor& color)
    {
        QPixmap pixmap(
            12,
            12
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

        painter.setPen(
            Qt::NoPen
        );

        painter.setBrush(
            color
        );

        painter.drawEllipse(
            QRectF(
                3,
                3,
                6,
                6
            )
        );

        return QIcon(
            pixmap
        );
    }

    QString formatData(
        const CanTraceEntry& entry)
    {
        QString text;

        for (
            std::size_t index = 0;
            index < entry.dlc;
            ++index
            )
        {
            if (!text.isEmpty())
            {
                text += " ";
            }

            text +=
                QString("%1")
                .arg(
                    entry.data[index],
                    2,
                    16,
                    QChar('0')
                )
                .toUpper();
        }

        return text;
    }
}

CanMonitorWidget::CanMonitorWidget(
    QWidget* parent)
    : QGroupBox(
        "CAN Monitor",
        parent
    )
{
    setObjectName(
        "CanMonitorWorkspace"
    );

    QVBoxLayout* mainLayout =
        new QVBoxLayout(
            this
        );

    mainLayout->setContentsMargins(
        14,
        20,
        14,
        14
    );

    mainLayout->setSpacing(
        12
    );

    // ==================================================
    // Bus overview
    // ==================================================

    QHBoxLayout* summaryLayout =
        new QHBoxLayout();

    summaryLayout->setSpacing(
        10
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "Bus Status",
            busStatusValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "Bitrate",
            bitrateValueLabel
        )
    );

    QFrame* utilizationCard =
        createSummaryCard(
            "Utilization",
            utilizationValueLabel
        );

    utilizationBar =
        new QProgressBar(
            utilizationCard
        );

    utilizationBar->setRange(
        0,
        1000
    );

    utilizationBar->setValue(
        0
    );

    utilizationBar->setTextVisible(
        false
    );

    utilizationBar->setFixedHeight(
        5
    );

    utilizationBar->setObjectName(
        "CanUtilizationBar"
    );

    if (
        QVBoxLayout* utilizationLayout =
        qobject_cast<QVBoxLayout*>(
            utilizationCard->layout()
        )
        )
    {
        utilizationLayout->addWidget(
            utilizationBar
        );
    }

    summaryLayout->addWidget(
        utilizationCard
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "Frames",
            totalFramesValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "Arbitrations",
            arbitrationValueLabel
        )
    );

    summaryLayout->addWidget(
        createSummaryCard(
            "Average Wait",
            averageWaitValueLabel
        )
    );

    mainLayout->addLayout(
        summaryLayout
    );

    // ==================================================
    // Capture controls
    // ==================================================

    QFrame* controlFrame =
        new QFrame();

    controlFrame->setObjectName(
        "CanControlBar"
    );

    QHBoxLayout* controlLayout =
        new QHBoxLayout(
            controlFrame
        );

    controlLayout->setContentsMargins(
        10,
        8,
        10,
        8
    );

    controlLayout->setSpacing(
        8
    );

    QLabel* filterLabel =
        new QLabel(
            "CAN ID"
        );

    filterLabel->setObjectName(
        "CanControlLabel"
    );

    idFilterCombo =
        new QComboBox();

    idFilterCombo->setMinimumWidth(
        190
    );

    idFilterCombo->addItem(
        "All IDs",
        -1
    );

    frameCountLabel =
        new QLabel(
            "Total: 0 | Showing: 0 | Mode: LAST 500"
        );

    frameCountLabel->setObjectName(
        "CanFrameCount"
    );

    freezeButton =
        new QPushButton(
            "Freeze Capture"
        );

    clearButton =
        new QPushButton(
            "Clear View"
        );

    showAllButton =
        new QPushButton(
            "Show All"
        );

    exportCsvButton =
        new QPushButton(
            "Export CSV"
        );

    freezeButton->setObjectName(
        "CanActionButton"
    );

    clearButton->setObjectName(
        "CanActionButton"
    );

    showAllButton->setObjectName(
        "CanActionButton"
    );

    exportCsvButton->setObjectName(
        "CanPrimaryButton"
    );

    controlLayout->addWidget(
        filterLabel
    );

    controlLayout->addWidget(
        idFilterCombo
    );

    controlLayout->addSpacing(
        8
    );

    controlLayout->addWidget(
        frameCountLabel
    );

    controlLayout->addStretch();

    controlLayout->addWidget(
        freezeButton
    );

    controlLayout->addWidget(
        clearButton
    );

    controlLayout->addWidget(
        showAllButton
    );

    controlLayout->addWidget(
        exportCsvButton
    );

    mainLayout->addWidget(
        controlFrame
    );

    // ==================================================
    // Analysis workspace
    // ==================================================

    QSplitter* splitter =
        new QSplitter(
            Qt::Horizontal
        );

    splitter->setObjectName(
        "CanWorkspaceSplitter"
    );

    splitter->setChildrenCollapsible(
        false
    );

    // --------------------------------------------------
    // Frame table
    // --------------------------------------------------

    QFrame* tablePanel =
        new QFrame();

    tablePanel->setObjectName(
        "CanTablePanel"
    );

    QVBoxLayout* tableLayout =
        new QVBoxLayout(
            tablePanel
        );

    tableLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    QLabel* tableTitle =
        new QLabel(
            "Live Frame Trace"
        );

    tableTitle->setObjectName(
        "CanSectionTitle"
    );

    tableLayout->addWidget(
        tableTitle
    );

    canTable =
        new QTableWidget(
            0,
            6
        );

    canTable->setObjectName(
        "CanTraceTable"
    );

    canTable->setHorizontalHeaderLabels(
        {
            "TX Start",
            "CAN ID",
            "Message",
            "DLC",
            "Wait",
            "Data"
        }
    );

    canTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            0,
            QHeaderView::ResizeToContents
        );

    canTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            1,
            QHeaderView::ResizeToContents
        );

    canTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            2,
            QHeaderView::ResizeToContents
        );

    canTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            3,
            QHeaderView::ResizeToContents
        );

    canTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            4,
            QHeaderView::ResizeToContents
        );

    canTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            5,
            QHeaderView::Stretch
        );

    canTable->verticalHeader()->setVisible(
        false
    );

    canTable->verticalHeader()->setDefaultSectionSize(
        30
    );

    canTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    canTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    canTable->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    canTable->setAlternatingRowColors(
        true
    );

    canTable->setShowGrid(
        false
    );

    tableLayout->addWidget(
        canTable,
        1
    );

    splitter->addWidget(
        tablePanel
    );

    // --------------------------------------------------
    // Frame inspector
    // --------------------------------------------------

    QFrame* inspectorPanel =
        new QFrame();

    inspectorPanel->setObjectName(
        "CanInspectorPanel"
    );

    inspectorPanel->setMinimumWidth(
        290
    );

    QVBoxLayout* inspectorLayout =
        new QVBoxLayout(
            inspectorPanel
        );

    inspectorLayout->setContentsMargins(
        12,
        12,
        12,
        12
    );

    inspectorLayout->setSpacing(
        8
    );

    QLabel* inspectorTitle =
        new QLabel(
            "Frame Inspector"
        );

    inspectorTitle->setObjectName(
        "CanSectionTitle"
    );

    QLabel* inspectorSubtitle =
        new QLabel(
            "Select a CAN frame to inspect timing and payload."
        );

    inspectorSubtitle->setObjectName(
        "CanInspectorSubtitle"
    );

    inspectorSubtitle->setWordWrap(
        true
    );

    inspectorLayout->addWidget(
        inspectorTitle
    );

    inspectorLayout->addWidget(
        inspectorSubtitle
    );

    inspectorLayout->addWidget(
        createInspectorField(
            "Message",
            inspectorMessageLabel
        )
    );

    QGridLayout* detailGrid =
        new QGridLayout();

    detailGrid->setSpacing(
        8
    );

    detailGrid->addWidget(
        createInspectorField(
            "CAN ID",
            inspectorIdLabel
        ),
        0,
        0
    );

    detailGrid->addWidget(
        createInspectorField(
            "DLC",
            inspectorDlcLabel
        ),
        0,
        1
    );

    detailGrid->addWidget(
        createInspectorField(
            "TX Start",
            inspectorTxStartLabel
        ),
        1,
        0
    );

    detailGrid->addWidget(
        createInspectorField(
            "Wait",
            inspectorWaitLabel
        ),
        1,
        1
    );

    detailGrid->addWidget(
        createInspectorField(
            "TX Duration",
            inspectorTxTimeLabel
        ),
        2,
        0,
        1,
        2
    );

    inspectorLayout->addLayout(
        detailGrid
    );

    inspectorLayout->addWidget(
        createInspectorField(
            "Payload",
            inspectorPayloadLabel
        )
    );

    inspectorPayloadLabel->setWordWrap(
        true
    );

    inspectorLayout->addStretch();

    splitter->addWidget(
        inspectorPanel
    );

    splitter->setStretchFactor(
        0,
        7
    );

    splitter->setStretchFactor(
        1,
        3
    );

    mainLayout->addWidget(
        splitter,
        1
    );

    // ==================================================
    // Connections
    // ==================================================

    connect(
        freezeButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            viewFrozen =
                !viewFrozen;

            freezeButton->setText(
                viewFrozen
                ? "Resume Capture"
                : "Freeze Capture"
            );

            freezeButton->setProperty(
                "active",
                viewFrozen
            );

            freezeButton->style()->unpolish(
                freezeButton
            );

            freezeButton->style()->polish(
                freezeButton
            );

            busStatusValueLabel->setText(
                viewFrozen
                ? "● FROZEN"
                : "● ONLINE"
            );

            busStatusValueLabel->setProperty(
                "state",
                viewFrozen
                ? "frozen"
                : "online"
            );

            busStatusValueLabel->style()->unpolish(
                busStatusValueLabel
            );

            busStatusValueLabel->style()->polish(
                busStatusValueLabel
            );

            if (
                !viewFrozen &&
                currentTrace != nullptr
                )
            {
                rebuildView(
                    *currentTrace
                );
            }
        }
    );

    connect(
        clearButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            clearView();
        }
    );

    connect(
        showAllButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            toggleShowAll();
        }
    );

    connect(
        exportCsvButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            exportCsv();
        }
    );

    connect(
        idFilterCombo,
        &QComboBox::currentIndexChanged,
        this,
        [this](int)
        {
            filterDirty =
                true;
        }
    );

    connect(
        canTable,
        &QTableWidget::currentCellChanged,
        this,
        [this](
            int currentRow,
            int,
            int,
            int)
        {
            updateInspectorFromRow(
                currentRow
            );

            if (
                currentRow >= 0
                )
            {
                autoSelectLatestFrame =
                    false;
            }
        }
    );

    setStyleSheet(
        R"(
            QProgressBar#CanUtilizationBar
            {
                border: none;
                border-radius: 2px;
                background-color: rgba(120, 140, 160, 45);
            }

            QProgressBar#CanUtilizationBar::chunk
            {
                border-radius: 2px;
                background-color: #3E7FB9;
            }

            QLabel#CanSummaryValue[state="online"]
            {
                color: #138A4B;
                font-weight: 700;
            }

            QLabel#CanSummaryValue[state="frozen"]
            {
                color: #B47A2B;
                font-weight: 700;
            }
        )"
    );

    clearInspector();
}

void CanMonitorWidget::updateTrace(
    const std::vector<CanTraceEntry>& trace,
    const CanStatistics& statistics,
    std::uint32_t bitrate,
    double simulationTimeMs)
{
    currentTrace =
        &trace;

    latestTraceSize =
        trace.size();

    updateBusSummary(
        statistics,
        bitrate,
        simulationTimeMs
    );

    updateAvailableIds(
        trace
    );

    if (viewFrozen)
    {
        updateFrameCount();
        return;
    }

    if (filterDirty)
    {
        rebuildView(
            trace
        );

        filterDirty =
            false;

        return;
    }

    while (
        displayedTraceCount <
        trace.size()
        )
    {
        const CanTraceEntry& entry =
            trace[
                displayedTraceCount
            ];

        if (
            matchesCurrentFilter(
                entry
            )
            )
        {
            appendTraceEntry(
                entry
            );
        }

        ++displayedTraceCount;
    }

    if (!showAllRows)
    {
        trimOldRows();
    }

    if (
        canTable->rowCount() >
        0
        )
    {
        canTable->scrollToBottom();
    }

    selectLatestFrameIfNeeded();

    updateFrameCount();
}

void CanMonitorWidget::appendTraceEntry(
    const CanTraceEntry& entry)
{
    const int row =
        canTable->rowCount();

    canTable->insertRow(
        row
    );

    QTableWidgetItem* txStartItem =
        new QTableWidgetItem(
            QString::number(
                entry.txStartTimeMs,
                'f',
                3
            ) +
            " ms"
        );

    QTableWidgetItem* canIdItem =
        new QTableWidgetItem(
            formatCanId(
                entry.arbitrationId
            )
        );

    const std::string messageName =
        CanMessageRegistry::getMessageName(
            entry.arbitrationId
        );

    QTableWidgetItem* messageItem =
        new QTableWidgetItem(
            QString::fromStdString(
                messageName
            )
        );

    QTableWidgetItem* dlcItem =
        new QTableWidgetItem(
            QString::number(
                static_cast<int>(
                    entry.dlc
                    )
            )
        );

    QTableWidgetItem* waitItem =
        new QTableWidgetItem(
            QString::number(
                entry.waitingTimeMs,
                'f',
                3
            ) +
            " ms"
        );

    const QString dataText =
        formatData(
            entry
        );

    QTableWidgetItem* dataItem =
        new QTableWidgetItem(
            dataText
        );

    txStartItem->setData(
        Qt::UserRole,
        entry.txStartTimeMs
    );

    txStartItem->setData(
        Qt::UserRole + 1,
        entry.waitingTimeMs
    );

    txStartItem->setData(
        Qt::UserRole + 2,
        entry.transmissionTimeMs
    );

    txStartItem->setData(
        Qt::UserRole + 3,
        static_cast<qulonglong>(
            entry.arbitrationId
            )
    );

    txStartItem->setData(
        Qt::UserRole + 4,
        static_cast<int>(
            entry.dlc
            )
    );

    txStartItem->setData(
        Qt::UserRole + 5,
        dataText
    );

    txStartItem->setData(
        Qt::UserRole + 6,
        QString::fromStdString(
            messageName
        )
    );

    applyMessageVisuals(
        canIdItem,
        messageItem,
        entry.arbitrationId
    );

    applyWaitVisuals(
        waitItem,
        entry.waitingTimeMs
    );

    canTable->setItem(
        row,
        0,
        txStartItem
    );

    canTable->setItem(
        row,
        1,
        canIdItem
    );

    canTable->setItem(
        row,
        2,
        messageItem
    );

    canTable->setItem(
        row,
        3,
        dlcItem
    );

    canTable->setItem(
        row,
        4,
        waitItem
    );

    canTable->setItem(
        row,
        5,
        dataItem
    );
}

bool CanMonitorWidget::matchesCurrentFilter(
    const CanTraceEntry& entry) const
{
    const int selectedId =
        idFilterCombo
        ->currentData()
        .toInt();

    if (selectedId < 0)
    {
        return true;
    }

    return
        entry.arbitrationId ==
        static_cast<std::uint32_t>(
            selectedId
            );
}

void CanMonitorWidget::updateAvailableIds(
    const std::vector<CanTraceEntry>& trace)
{
    QSet<std::uint32_t> existingIds;

    for (
        int index = 1;
        index < idFilterCombo->count();
        ++index
        )
    {
        existingIds.insert(
            static_cast<std::uint32_t>(
                idFilterCombo
                ->itemData(index)
                .toUInt()
                )
        );
    }

    for (
        const CanTraceEntry& entry :
        trace
        )
    {
        if (
            existingIds.contains(
                entry.arbitrationId
            )
            )
        {
            continue;
        }

        const std::string messageName =
            CanMessageRegistry::getMessageName(
                entry.arbitrationId
            );

        const QString text =
            formatCanId(
                entry.arbitrationId
            ) +
            " - " +
            QString::fromStdString(
                messageName
            );

        idFilterCombo->addItem(
            text,
            static_cast<unsigned int>(
                entry.arbitrationId
                )
        );

        existingIds.insert(
            entry.arbitrationId
        );
    }
}

void CanMonitorWidget::rebuildView(
    const std::vector<CanTraceEntry>& trace)
{
    canTable->setRowCount(
        0
    );

    clearInspector();

    std::vector<const CanTraceEntry*>
        matchingEntries;

    matchingEntries.reserve(
        trace.size()
    );

    for (
        const CanTraceEntry& entry :
        trace
        )
    {
        if (
            matchesCurrentFilter(
                entry
            )
            )
        {
            matchingEntries.push_back(
                &entry
            );
        }
    }

    std::size_t startIndex =
        0;

    if (
        !showAllRows &&
        matchingEntries.size() >
        static_cast<std::size_t>(
            maxVisibleRows
            )
        )
    {
        startIndex =
            matchingEntries.size() -
            maxVisibleRows;
    }

    for (
        std::size_t index = startIndex;
        index < matchingEntries.size();
        ++index
        )
    {
        appendTraceEntry(
            *matchingEntries[index]
        );
    }

    displayedTraceCount =
        trace.size();

    if (
        canTable->rowCount() >
        0
        )
    {
        canTable->scrollToBottom();
    }

    autoSelectLatestFrame =
        true;

    selectLatestFrameIfNeeded();

    updateFrameCount();
}

void CanMonitorWidget::trimOldRows()
{
    while (
        canTable->rowCount() >
        maxVisibleRows
        )
    {
        canTable->removeRow(
            0
        );
    }
}

void CanMonitorWidget::clearView()
{
    canTable->setRowCount(
        0
    );

    displayedTraceCount =
        latestTraceSize;

    clearInspector();

    autoSelectLatestFrame =
        true;

    updateFrameCount();
}

void CanMonitorWidget::toggleShowAll()
{
    showAllRows =
        !showAllRows;

    showAllButton->setText(
        showAllRows
        ? "Last 500"
        : "Show All"
    );

    if (
        currentTrace != nullptr
        )
    {
        rebuildView(
            *currentTrace
        );
    }
}

void CanMonitorWidget::updateFrameCount()
{
    const std::size_t totalFrames =
        latestTraceSize;

    const int visibleRows =
        canTable != nullptr
        ? canTable->rowCount()
        : 0;

    const QString modeText =
        showAllRows
        ? "ALL"
        : "LAST 500";

    if (visibleRows <= 0)
    {
        frameCountLabel->setText(
            QString(
                "Total: %1  •  Showing: 0  •  %2"
            )
            .arg(
                static_cast<qulonglong>(
                    totalFrames
                    )
            )
            .arg(
                modeText
            )
        );

        return;
    }

    const int selectedId =
        idFilterCombo
        ->currentData()
        .toInt();

    if (selectedId < 0)
    {
        std::size_t firstVisible =
            1;

        std::size_t lastVisible =
            totalFrames;

        if (
            !showAllRows &&
            totalFrames >
            static_cast<std::size_t>(
                visibleRows
                )
            )
        {
            firstVisible =
                totalFrames -
                static_cast<std::size_t>(
                    visibleRows
                    ) +
                1;
        }

        frameCountLabel->setText(
            QString(
                "Total: %1  •  Showing: %2-%3  •  %4"
            )
            .arg(
                static_cast<qulonglong>(
                    totalFrames
                    )
            )
            .arg(
                static_cast<qulonglong>(
                    firstVisible
                    )
            )
            .arg(
                static_cast<qulonglong>(
                    lastVisible
                    )
            )
            .arg(
                modeText
            )
        );

        return;
    }

    frameCountLabel->setText(
        QString(
            "Bus Frames: %1  •  Filtered: %2  •  %3"
        )
        .arg(
            static_cast<qulonglong>(
                totalFrames
                )
        )
        .arg(
            visibleRows
        )
        .arg(
            modeText
        )
    );
}

void CanMonitorWidget::updateBusSummary(
    const CanStatistics& statistics,
    std::uint32_t bitrate,
    double simulationTimeMs)
{
    busStatusValueLabel->setText(
        viewFrozen
        ? "● FROZEN"
        : "● ONLINE"
    );

    busStatusValueLabel->setProperty(
        "state",
        viewFrozen
        ? "frozen"
        : "online"
    );

    busStatusValueLabel->style()->unpolish(
        busStatusValueLabel
    );

    busStatusValueLabel->style()->polish(
        busStatusValueLabel
    );

    bitrateValueLabel->setText(
        QString::number(
            bitrate /
            1000
        ) +
        " kbps"
    );

    const double utilization =
        statistics.getBusUtilization(
            simulationTimeMs
        );

    utilizationValueLabel->setText(
        QString::number(
            utilization,
            'f',
            2
        ) +
        " %"
    );

    utilizationBar->setValue(
        static_cast<int>(
            std::clamp(
                utilization,
                0.0,
                100.0
            ) *
            10.0
            )
    );

    totalFramesValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                statistics.getFramesTransmitted()
                )
        )
    );

    arbitrationValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                statistics.getArbitrationCount()
                )
        )
    );

    averageWaitValueLabel->setText(
        QString::number(
            statistics.getAverageWaitingTimeMs(),
            'f',
            3
        ) +
        " ms"
    );
}

void CanMonitorWidget::updateInspectorFromRow(
    int row)
{
    if (
        row < 0 ||
        row >= canTable->rowCount()
        )
    {
        clearInspector();
        return;
    }

    QTableWidgetItem* sourceItem =
        canTable->item(
            row,
            0
        );

    if (sourceItem == nullptr)
    {
        clearInspector();
        return;
    }

    const double txStart =
        sourceItem
        ->data(
            Qt::UserRole
        )
        .toDouble();

    const double wait =
        sourceItem
        ->data(
            Qt::UserRole + 1
        )
        .toDouble();

    const double txDuration =
        sourceItem
        ->data(
            Qt::UserRole + 2
        )
        .toDouble();

    const std::uint32_t id =
        static_cast<std::uint32_t>(
            sourceItem
            ->data(
                Qt::UserRole + 3
            )
            .toULongLong()
            );

    const int dlc =
        sourceItem
        ->data(
            Qt::UserRole + 4
        )
        .toInt();

    const QString payload =
        sourceItem
        ->data(
            Qt::UserRole + 5
        )
        .toString();

    const QString message =
        sourceItem
        ->data(
            Qt::UserRole + 6
        )
        .toString();

    inspectorMessageLabel->setText(
        message
    );

    inspectorIdLabel->setText(
        formatCanId(
            id
        )
    );

    inspectorDlcLabel->setText(
        QString::number(
            dlc
        )
    );

    inspectorTxStartLabel->setText(
        QString::number(
            txStart,
            'f',
            3
        ) +
        " ms"
    );

    inspectorWaitLabel->setText(
        QString::number(
            wait,
            'f',
            3
        ) +
        " ms"
    );

    inspectorTxTimeLabel->setText(
        QString::number(
            txDuration,
            'f',
            3
        ) +
        " ms"
    );

    inspectorPayloadLabel->setText(
        payload.isEmpty()
        ? "<empty>"
        : payload
    );
}

void CanMonitorWidget::clearInspector()
{
    inspectorMessageLabel->setText(
        "No frame selected"
    );

    inspectorIdLabel->setText(
        "--"
    );

    inspectorDlcLabel->setText(
        "--"
    );

    inspectorTxStartLabel->setText(
        "--"
    );

    inspectorWaitLabel->setText(
        "--"
    );

    inspectorTxTimeLabel->setText(
        "--"
    );

    inspectorPayloadLabel->setText(
        "--"
    );
}

void CanMonitorWidget::applyMessageVisuals(
    QTableWidgetItem* canIdItem,
    QTableWidgetItem* messageItem,
    std::uint32_t arbitrationId) const
{
    const QColor accent =
        messageAccentColor(
            arbitrationId
        );

    messageItem->setIcon(
        createMessageDotIcon(
            accent
        )
    );

    canIdItem->setForeground(
        accent
    );
}

void CanMonitorWidget::applyWaitVisuals(
    QTableWidgetItem* waitItem,
    double waitingTimeMs) const
{
    if (
        waitingTimeMs <=
        0.0005
        )
    {
        return;
    }

    const QColor waitColor =
        waitingTimeMs >=
        0.30
        ? QColor("#B86D3D")
        : QColor("#8A7443");

    waitItem->setForeground(
        waitColor
    );

    QFont font =
        waitItem->font();

    font.setBold(
        true
    );

    waitItem->setFont(
        font
    );

    waitItem->setToolTip(
        "Frame waited for CAN arbitration before transmission."
    );
}

void CanMonitorWidget::selectLatestFrameIfNeeded()
{
    if (
        !autoSelectLatestFrame ||
        canTable == nullptr ||
        canTable->rowCount() <= 0
        )
    {
        return;
    }

    const int latestRow =
        canTable->rowCount() -
        1;

    canTable->setCurrentCell(
        latestRow,
        0
    );

    updateInspectorFromRow(
        latestRow
    );

    // After the initial automatic selection, keep the
    // inspector stable until the user selects another row.
    autoSelectLatestFrame =
        false;
}

void CanMonitorWidget::exportCsv()
{
    if (
        currentTrace == nullptr ||
        currentTrace->empty()
        )
    {
        return;
    }

    const QString filePath =
        QFileDialog::getSaveFileName(
            this,
            "Export CAN Trace",
            "can_trace.csv",
            "CSV Files (*.csv)"
        );

    if (filePath.isEmpty())
    {
        return;
    }

    std::ofstream file(
        filePath.toStdString()
    );

    if (!file.is_open())
    {
        return;
    }

    file
        << "TX Start (ms),"
        << "CAN ID,"
        << "Message,"
        << "DLC,"
        << "Wait (ms),"
        << "Data\n";

    file
        << std::fixed
        << std::setprecision(3);

    for (
        const CanTraceEntry& entry :
        *currentTrace
        )
    {
        const std::string messageName =
            CanMessageRegistry::getMessageName(
                entry.arbitrationId
            );

        file
            << entry.txStartTimeMs
            << ",";

        file
            << "0x"
            << std::hex
            << std::uppercase
            << entry.arbitrationId
            << std::dec
            << ",";

        file
            << messageName
            << ",";

        file
            << static_cast<int>(
                entry.dlc
                )
            << ",";

        file
            << entry.waitingTimeMs
            << ",";

        for (
            std::size_t index = 0;
            index < entry.dlc;
            ++index
            )
        {
            if (index > 0)
            {
                file << " ";
            }

            file
                << std::hex
                << std::uppercase
                << std::setw(2)
                << std::setfill('0')
                << static_cast<int>(
                    entry.data[index]
                    )
                << std::dec;
        }

        file
            << std::setfill(' ')
            << "\n";
    }
}

void CanMonitorWidget::clear()
{
    canTable->setRowCount(
        0
    );

    displayedTraceCount =
        0;

    latestTraceSize =
        0;

    currentTrace =
        nullptr;

    viewFrozen =
        false;

    filterDirty =
        false;

    showAllRows =
        false;

    autoSelectLatestFrame =
        true;

    freezeButton->setText(
        "Freeze Capture"
    );

    freezeButton->setProperty(
        "active",
        false
    );

    showAllButton->setText(
        "Show All"
    );

    idFilterCombo->setCurrentIndex(
        0
    );

    busStatusValueLabel->setText(
        "● ONLINE"
    );

    bitrateValueLabel->setText(
        "--"
    );

    utilizationValueLabel->setText(
        "--"
    );

    totalFramesValueLabel->setText(
        "0"
    );

    arbitrationValueLabel->setText(
        "0"
    );

    averageWaitValueLabel->setText(
        "0.000 ms"
    );

    clearInspector();
    updateFrameCount();
}
