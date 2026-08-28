#include "EventLogWidget.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

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
                QString::number(
                    event.timeMs,
                    'f',
                    3
                ) +
                " ms"
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