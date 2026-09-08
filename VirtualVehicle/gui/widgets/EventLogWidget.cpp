#include "EventLogWidget.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace
{
    QString formatSimulationTimestamp(
        double timeMs)
    {
        if (timeMs < 0.0)
        {
            timeMs = 0.0;
        }

        const qint64 totalMilliseconds =
            static_cast<qint64>(
                timeMs +
                0.5
                );

        const qint64 hours =
            totalMilliseconds /
            3600000LL;

        const qint64 minutes =
            (
                totalMilliseconds /
                60000LL
                ) %
            60LL;

        const qint64 seconds =
            (
                totalMilliseconds /
                1000LL
                ) %
            60LL;

        const qint64 milliseconds =
            totalMilliseconds %
            1000LL;

        return
            QString(
                "%1:%2:%3.%4"
            )
            .arg(
                hours,
                2,
                10,
                QLatin1Char('0')
            )
            .arg(
                minutes,
                2,
                10,
                QLatin1Char('0')
            )
            .arg(
                seconds,
                2,
                10,
                QLatin1Char('0')
            )
            .arg(
                milliseconds,
                3,
                10,
                QLatin1Char('0')
            );
    }
}

EventLogWidget::EventLogWidget(
    QWidget* parent)
    : QGroupBox(
        "Event Log",
        parent
    )
{
    QVBoxLayout* layout =
        new QVBoxLayout(this);

    eventTable =
        new QTableWidget(
            0,
            3
        );

    eventTable->setHorizontalHeaderLabels(
        {
            "Time",
            "Category",
            "Message"
        }
    );

    eventTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            0,
            QHeaderView::ResizeToContents
        );

    eventTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            1,
            QHeaderView::ResizeToContents
        );

    eventTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            2,
            QHeaderView::Stretch
        );

    eventTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    eventTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    eventTable->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    eventTable->setAlternatingRowColors(
        true
    );

    eventTable->setMinimumHeight(
        220
    );

    layout->addWidget(
        eventTable
    );
}

void EventLogWidget::updateEvents(
    const std::vector<VehicleEvent>& events)
{
    while (
        displayedEventCount <
        events.size()
        )
    {
        const VehicleEvent& event =
            events[displayedEventCount];

        const int row =
            eventTable->rowCount();

        eventTable->insertRow(
            row
        );

        QTableWidgetItem* timeItem =
            new QTableWidgetItem(
                formatSimulationTimestamp(
                    event.timeMs
                )
            );

        QTableWidgetItem* categoryItem =
            new QTableWidgetItem(
                QString::fromStdString(
                    event.category
                )
            );

        QTableWidgetItem* messageItem =
            new QTableWidgetItem(
                QString::fromStdString(
                    event.message
                )
            );

        eventTable->setItem(
            row,
            0,
            timeItem
        );

        eventTable->setItem(
            row,
            1,
            categoryItem
        );

        eventTable->setItem(
            row,
            2,
            messageItem
        );

        ++displayedEventCount;
    }

    if (
        eventTable->rowCount() >
        0
        )
    {
        eventTable->scrollToBottom();
    }
}

void EventLogWidget::clear()
{
    eventTable->setRowCount(
        0
    );

    displayedEventCount =
        0;
}
