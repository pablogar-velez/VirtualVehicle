#include "StatisticsWidget.h"

#include <algorithm>
#include <vector>

#include <QAbstractItemView>
#include <QFont>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include "../../can/CanMessageRegistry.h"

// ==================================================
// Constructor
// ==================================================

StatisticsWidget::StatisticsWidget(
    QWidget* parent)
    : QGroupBox(
        "CAN Bus Statistics",
        parent
    )
{
    QVBoxLayout* mainLayout =
        new QVBoxLayout(this);

    // ==================================================
    // Global statistics
    // ==================================================

    QGridLayout* statsLayout =
        new QGridLayout();

    QLabel* bitrateLabel =
        new QLabel(
            "Bitrate"
        );

    QLabel* simulationTimeLabel =
        new QLabel(
            "Simulation Time"
        );

    QLabel* framesLabel =
        new QLabel(
            "Frames Transmitted"
        );

    QLabel* arbitrationsLabel =
        new QLabel(
            "Arbitrations"
        );

    QLabel* utilizationLabel =
        new QLabel(
            "Bus Utilization"
        );

    QLabel* averageWaitLabel =
        new QLabel(
            "Average Wait"
        );

    QLabel* maximumWaitLabel =
        new QLabel(
            "Maximum Wait"
        );

    bitrateValueLabel =
        new QLabel(
            "0 kbps"
        );

    simulationTimeValueLabel =
        new QLabel(
            "0.000 s"
        );

    framesValueLabel =
        new QLabel(
            "0"
        );

    arbitrationsValueLabel =
        new QLabel(
            "0"
        );

    utilizationValueLabel =
        new QLabel(
            "0.000 %"
        );

    averageWaitValueLabel =
        new QLabel(
            "0.000 ms"
        );

    maximumWaitValueLabel =
        new QLabel(
            "0.000 ms"
        );

    QFont valueFont;

    valueFont.setPointSize(
        13
    );

    valueFont.setBold(
        true
    );

    bitrateValueLabel->setFont(
        valueFont
    );

    simulationTimeValueLabel->setFont(
        valueFont
    );

    framesValueLabel->setFont(
        valueFont
    );

    arbitrationsValueLabel->setFont(
        valueFont
    );

    utilizationValueLabel->setFont(
        valueFont
    );

    averageWaitValueLabel->setFont(
        valueFont
    );

    maximumWaitValueLabel->setFont(
        valueFont
    );

    // ==================================================
    // Global layout
    // ==================================================

    statsLayout->addWidget(
        bitrateLabel,
        0,
        0
    );

    statsLayout->addWidget(
        bitrateValueLabel,
        0,
        1
    );

    statsLayout->addWidget(
        simulationTimeLabel,
        0,
        2
    );

    statsLayout->addWidget(
        simulationTimeValueLabel,
        0,
        3
    );

    statsLayout->addWidget(
        framesLabel,
        1,
        0
    );

    statsLayout->addWidget(
        framesValueLabel,
        1,
        1
    );

    statsLayout->addWidget(
        arbitrationsLabel,
        1,
        2
    );

    statsLayout->addWidget(
        arbitrationsValueLabel,
        1,
        3
    );

    statsLayout->addWidget(
        utilizationLabel,
        2,
        0
    );

    statsLayout->addWidget(
        utilizationValueLabel,
        2,
        1
    );

    statsLayout->addWidget(
        averageWaitLabel,
        2,
        2
    );

    statsLayout->addWidget(
        averageWaitValueLabel,
        2,
        3
    );

    statsLayout->addWidget(
        maximumWaitLabel,
        3,
        0
    );

    statsLayout->addWidget(
        maximumWaitValueLabel,
        3,
        1
    );

    mainLayout->addLayout(
        statsLayout
    );

    // ==================================================
    // Message statistics table
    // ==================================================

    messageTable =
        new QTableWidget(
            0,
            5
        );

    messageTable->setHorizontalHeaderLabels(
        {
            "CAN ID",
            "Message",
            "Frames",
            "Avg Wait",
            "Max Wait"
        }
    );

    messageTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            0,
            QHeaderView::ResizeToContents
        );

    messageTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            1,
            QHeaderView::Stretch
        );

    messageTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            2,
            QHeaderView::ResizeToContents
        );

    messageTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            3,
            QHeaderView::ResizeToContents
        );

    messageTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            4,
            QHeaderView::ResizeToContents
        );

    messageTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    messageTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    messageTable->setAlternatingRowColors(
        true
    );

    mainLayout->addWidget(
        messageTable,
        1
    );
}

// ==================================================
// Statistics refresh
// ==================================================

void StatisticsWidget::updateStatistics(
    const CanStatistics& statistics,
    std::uint32_t bitrate,
    double simulationTimeMs)
{
    // ==================================================
    // Bitrate
    // ==================================================

    bitrateValueLabel->setText(
        QString::number(
            bitrate / 1000
        ) +
        " kbps"
    );

    // ==================================================
    // Simulation time
    // ==================================================

    simulationTimeValueLabel->setText(
        QString::number(
            simulationTimeMs /
            1000.0,
            'f',
            3
        ) +
        " s"
    );

    // ==================================================
    // Frames
    // ==================================================

    framesValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                statistics
                .getFramesTransmitted()
                )
        )
    );

    // ==================================================
    // Arbitrations
    // ==================================================

    arbitrationsValueLabel->setText(
        QString::number(
            static_cast<qulonglong>(
                statistics
                .getArbitrationCount()
                )
        )
    );

    // ==================================================
    // Utilization
    // ==================================================

    utilizationValueLabel->setText(
        QString::number(
            statistics.getBusUtilization(
                simulationTimeMs
            ),
            'f',
            3
        ) +
        " %"
    );

    // ==================================================
    // Average wait
    // ==================================================

    averageWaitValueLabel->setText(
        QString::number(
            statistics
            .getAverageWaitingTimeMs(),
            'f',
            3
        ) +
        " ms"
    );

    // ==================================================
    // Maximum wait
    // ==================================================

    maximumWaitValueLabel->setText(
        QString::number(
            statistics
            .getMaximumWaitingTimeMs(),
            'f',
            3
        ) +
        " ms"
    );

    // ==================================================
    // Message table
    // ==================================================

    updateMessageStatistics(
        statistics
    );
}

// ==================================================
// Per-message statistics
// ==================================================

void StatisticsWidget::updateMessageStatistics(
    const CanStatistics& statistics)
{
    const auto& messageStats =
        statistics.getMessageStatistics();

    std::vector<std::uint32_t> ids;

    ids.reserve(
        messageStats.size()
    );

    for (
        const auto& entry :
        messageStats
        )
    {
        ids.push_back(
            entry.first
        );
    }

    std::sort(
        ids.begin(),
        ids.end()
    );

    messageTable->setRowCount(
        static_cast<int>(
            ids.size()
            )
    );

    int row =
        0;

    for (
        std::uint32_t id :
    ids
        )
    {
        const CanMessageStatistics& stats =
            messageStats.at(
                id
            );

        const QString idText =
            QString(
                "0x%1"
            )
            .arg(
                id,
                0,
                16
            )
            .toUpper();

        const std::string messageName =
            CanMessageRegistry::getMessageName(
                id
            );

        messageTable->setItem(
            row,
            0,
            new QTableWidgetItem(
                idText
            )
        );

        messageTable->setItem(
            row,
            1,
            new QTableWidgetItem(
                QString::fromStdString(
                    messageName
                )
            )
        );

        messageTable->setItem(
            row,
            2,
            new QTableWidgetItem(
                QString::number(
                    static_cast<qulonglong>(
                        stats.frameCount
                        )
                )
            )
        );

        messageTable->setItem(
            row,
            3,
            new QTableWidgetItem(
                QString::number(
                    stats
                    .getAverageWaitingTimeMs(),
                    'f',
                    3
                ) +
                " ms"
            )
        );

        messageTable->setItem(
            row,
            4,
            new QTableWidgetItem(
                QString::number(
                    stats.maximumWaitingTimeMs,
                    'f',
                    3
                ) +
                " ms"
            )
        );

        ++row;
    }
}