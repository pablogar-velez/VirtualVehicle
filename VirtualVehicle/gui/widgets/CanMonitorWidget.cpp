#include "CanMonitorWidget.h"

#include <fstream>
#include <iomanip>

#include <QAbstractItemView>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSet>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include "../../can/CanMessageRegistry.h"

// ==================================================
// Constructor
// ==================================================

CanMonitorWidget::CanMonitorWidget(
    QWidget* parent)
    : QGroupBox(
        "CAN Monitor",
        parent
    )
{
    QVBoxLayout* mainLayout =
        new QVBoxLayout(this);

    // ==================================================
    // Controls
    // ==================================================

    QHBoxLayout* controlLayout =
        new QHBoxLayout();

    QLabel* filterLabel =
        new QLabel(
            "CAN ID:"
        );

    idFilterCombo =
        new QComboBox();

    idFilterCombo->addItem(
        "All IDs",
        -1
    );

    frameCountLabel =
        new QLabel(
            "Total: 0 | Showing: 0 | Mode: LAST 500"
        );

    freezeButton =
        new QPushButton(
            "Freeze View"
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

    controlLayout->addWidget(
        filterLabel
    );

    controlLayout->addWidget(
        idFilterCombo
    );

    controlLayout->addSpacing(
        20
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

    mainLayout->addLayout(
        controlLayout
    );

    // ==================================================
    // CAN table
    // ==================================================

    canTable =
        new QTableWidget(
            0,
            6
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

    // ==================================================
    // Column sizing
    // ==================================================

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

    // ==================================================
    // Table behavior
    // ==================================================

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

    mainLayout->addWidget(
        canTable,
        1
    );

    // ==================================================
    // Freeze / Resume View
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
                ? "Resume View"
                : "Freeze View"
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

    // ==================================================
    // Clear View
    // ==================================================

    connect(
        clearButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            clearView();
        }
    );

    // ==================================================
    // Show All / Last 500
    // ==================================================

    connect(
        showAllButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            toggleShowAll();
        }
    );

    // ==================================================
    // CSV Export
    // ==================================================

    connect(
        exportCsvButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            exportCsv();
        }
    );

    // ==================================================
    // Filter
    // ==================================================

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
}

// ==================================================
// Update CAN trace
// ==================================================

void CanMonitorWidget::updateTrace(
    const std::vector<CanTraceEntry>& trace)
{
    currentTrace =
        &trace;

    latestTraceSize =
        trace.size();

    // ==================================================
    // Discover available CAN IDs
    // ==================================================

    updateAvailableIds(
        trace
    );

    // ==================================================
    // Frozen visualization
    // ==================================================

    if (viewFrozen)
    {
        updateFrameCount();

        return;
    }

    // ==================================================
    // Filter changed
    // ==================================================

    if (filterDirty)
    {
        rebuildView(
            trace
        );

        filterDirty =
            false;

        return;
    }

    // ==================================================
    // Process new trace entries
    // ==================================================

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

    // ==================================================
    // Limit normal mode to newest 500 rows
    // ==================================================

    if (!showAllRows)
    {
        trimOldRows();
    }

    // ==================================================
    // Auto-scroll
    // ==================================================

    if (
        canTable->rowCount() >
        0
        )
    {
        canTable->scrollToBottom();
    }

    updateFrameCount();
}

// ==================================================
// Append one CAN frame
// ==================================================

void CanMonitorWidget::appendTraceEntry(
    const CanTraceEntry& entry)
{
    const int row =
        canTable->rowCount();

    canTable->insertRow(
        row
    );

    // ==================================================
    // TX Start
    // ==================================================

    QTableWidgetItem* txStartItem =
        new QTableWidgetItem(
            QString::number(
                entry.txStartTimeMs,
                'f',
                3
            ) +
            " ms"
        );

    // ==================================================
    // CAN ID
    // ==================================================

    const QString canIdText =
        QString(
            "0x%1"
        )
        .arg(
            entry.arbitrationId,
            0,
            16
        )
        .toUpper();

    QTableWidgetItem* canIdItem =
        new QTableWidgetItem(
            canIdText
        );

    // ==================================================
    // Message
    // ==================================================

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

    // ==================================================
    // DLC
    // ==================================================

    QTableWidgetItem* dlcItem =
        new QTableWidgetItem(
            QString::number(
                static_cast<int>(
                    entry.dlc
                    )
            )
        );

    // ==================================================
    // Wait
    // ==================================================

    QTableWidgetItem* waitItem =
        new QTableWidgetItem(
            QString::number(
                entry.waitingTimeMs,
                'f',
                3
            ) +
            " ms"
        );

    // ==================================================
    // Data
    // ==================================================

    QString dataText;

    for (
        std::size_t i = 0;
        i < entry.dlc;
        ++i
        )
    {
        if (!dataText.isEmpty())
        {
            dataText += " ";
        }

        dataText +=
            QString("%1")
            .arg(
                entry.data[i],
                2,
                16,
                QChar('0')
            )
            .toUpper();
    }

    QTableWidgetItem* dataItem =
        new QTableWidgetItem(
            dataText
        );

    // ==================================================
    // Insert cells
    // ==================================================

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

// ==================================================
// Current filter matching
// ==================================================

bool CanMonitorWidget::matchesCurrentFilter(
    const CanTraceEntry& entry) const
{
    const int selectedId =
        idFilterCombo
        ->currentData()
        .toInt();

    // -1 = All IDs
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

// ==================================================
// Discover CAN IDs
// ==================================================

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

        const QString idText =
            QString(
                "0x%1"
            )
            .arg(
                entry.arbitrationId,
                0,
                16
            )
            .toUpper();

        const QString text =
            idText +
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

// ==================================================
// Rebuild current view
// ==================================================

void CanMonitorWidget::rebuildView(
    const std::vector<CanTraceEntry>& trace)
{
    canTable->setRowCount(
        0
    );

    std::vector<const CanTraceEntry*>
        matchingEntries;

    matchingEntries.reserve(
        trace.size()
    );

    // ==================================================
    // Find matching entries
    // ==================================================

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

    // ==================================================
    // Select starting position
    // ==================================================

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

    // ==================================================
    // Rebuild table
    // ==================================================

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

    updateFrameCount();
}

// ==================================================
// Trim oldest rows
// ==================================================

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

// ==================================================
// Clear visual table
// ==================================================

void CanMonitorWidget::clearView()
{
    canTable->setRowCount(
        0
    );

    // Do not replay previous history automatically.
    displayedTraceCount =
        latestTraceSize;

    updateFrameCount();
}

// ==================================================
// Show All / Last 500
// ==================================================

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

// ==================================================
// Frame information
// ==================================================

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

    // ==================================================
    // Empty table
    // ==================================================

    if (visibleRows <= 0)
    {
        frameCountLabel->setText(
            QString(
                "Total: %1 | Showing: 0 | Mode: %2"
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

    // ==================================================
    // All IDs selected
    // ==================================================

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
                "Total: %1 | Showing: %2 - %3 | Mode: %4"
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

    // ==================================================
    // Filtered mode
    // ==================================================
    //
    // A filtered table no longer maps directly to the
    // global frame indexes, so show visible count instead.
    // ==================================================

    frameCountLabel->setText(
        QString(
            "Total Bus Frames: %1 | Filtered Visible: %2 | Mode: %3"
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

// ==================================================
// Export full CAN trace to CSV
// ==================================================

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

    // ==================================================
    // CSV header
    // ==================================================

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

    // ==================================================
    // Export complete backend trace
    // ==================================================

    for (
        const CanTraceEntry& entry :
        *currentTrace
        )
    {
        const std::string messageName =
            CanMessageRegistry::getMessageName(
                entry.arbitrationId
            );

        // --------------------------------------------------
        // TX Start
        // --------------------------------------------------

        file
            << entry.txStartTimeMs
            << ",";

        // --------------------------------------------------
        // CAN ID
        // --------------------------------------------------

        file
            << "0x"
            << std::hex
            << std::uppercase
            << entry.arbitrationId
            << std::dec
            << ",";

        // --------------------------------------------------
        // Message
        // --------------------------------------------------

        file
            << messageName
            << ",";

        // --------------------------------------------------
        // DLC
        // --------------------------------------------------

        file
            << static_cast<int>(
                entry.dlc
                )
            << ",";

        // --------------------------------------------------
        // Wait
        // --------------------------------------------------

        file
            << entry.waitingTimeMs
            << ",";

        // --------------------------------------------------
        // Data
        // --------------------------------------------------

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

    file.close();
}

// ==================================================
// Full GUI reset
// ==================================================

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

    freezeButton->setText(
        "Freeze View"
    );

    showAllButton->setText(
        "Show All"
    );

    idFilterCombo->setCurrentIndex(
        0
    );

    updateFrameCount();
}