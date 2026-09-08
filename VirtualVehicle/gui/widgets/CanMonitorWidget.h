#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <QGroupBox>

#include "../../can/CanTraceEntry.h"
#include "../../can/CanStatistics.h"

class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QTableWidgetItem;
class QFrame;
class QProgressBar;

class CanMonitorWidget : public QGroupBox
{
public:
    explicit CanMonitorWidget(
        QWidget* parent = nullptr
    );

    void updateTrace(
        const std::vector<CanTraceEntry>& trace,
        const CanStatistics& statistics,
        std::uint32_t bitrate,
        double simulationTimeMs
    );

    void clear();

private:
    static constexpr int maxVisibleRows = 500;

    QComboBox* idFilterCombo{};

    QPushButton* freezeButton{};
    QPushButton* clearButton{};
    QPushButton* showAllButton{};
    QPushButton* exportCsvButton{};

    QLabel* frameCountLabel{};

    QLabel* busStatusValueLabel{};
    QLabel* bitrateValueLabel{};
    QLabel* utilizationValueLabel{};
    QProgressBar* utilizationBar{};

    QLabel* totalFramesValueLabel{};
    QLabel* arbitrationValueLabel{};
    QLabel* averageWaitValueLabel{};

    QTableWidget* canTable{};

    QLabel* inspectorMessageLabel{};
    QLabel* inspectorIdLabel{};
    QLabel* inspectorDlcLabel{};
    QLabel* inspectorTxStartLabel{};
    QLabel* inspectorWaitLabel{};
    QLabel* inspectorTxTimeLabel{};
    QLabel* inspectorPayloadLabel{};

    std::size_t displayedTraceCount{ 0 };
    std::size_t latestTraceSize{ 0 };

    bool viewFrozen{ false };
    bool filterDirty{ false };
    bool showAllRows{ false };
    bool autoSelectLatestFrame{ true };

    const std::vector<CanTraceEntry>* currentTrace{ nullptr };

    void appendTraceEntry(
        const CanTraceEntry& entry
    );

    void rebuildView(
        const std::vector<CanTraceEntry>& trace
    );

    bool matchesCurrentFilter(
        const CanTraceEntry& entry
    ) const;

    void updateAvailableIds(
        const std::vector<CanTraceEntry>& trace
    );

    void trimOldRows();
    void clearView();
    void toggleShowAll();
    void exportCsv();
    void updateFrameCount();

    void updateBusSummary(
        const CanStatistics& statistics,
        std::uint32_t bitrate,
        double simulationTimeMs
    );

    void updateInspectorFromRow(
        int row
    );

    void clearInspector();

    void applyMessageVisuals(
        QTableWidgetItem* canIdItem,
        QTableWidgetItem* messageItem,
        std::uint32_t arbitrationId
    ) const;

    void applyWaitVisuals(
        QTableWidgetItem* waitItem,
        double waitingTimeMs
    ) const;

    void selectLatestFrameIfNeeded();
};
