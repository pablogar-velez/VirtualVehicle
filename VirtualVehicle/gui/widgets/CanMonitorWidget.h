#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <QGroupBox>

#include "../../can/CanTraceEntry.h"

class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;

class CanMonitorWidget : public QGroupBox
{
public:
    explicit CanMonitorWidget(
        QWidget* parent = nullptr
    );

    void updateTrace(
        const std::vector<CanTraceEntry>& trace
    );

    void clear();

private:
    // ==================================================
    // Configuration
    // ==================================================

    static constexpr int maxVisibleRows =
        500;

    // ==================================================
    // Controls
    // ==================================================

    QComboBox* idFilterCombo{};

    QPushButton* freezeButton{};
    QPushButton* clearButton{};
    QPushButton* showAllButton{};
    QPushButton* exportCsvButton{};

    QLabel* frameCountLabel{};

    // ==================================================
    // CAN table
    // ==================================================

    QTableWidget* canTable{};

    // ==================================================
    // State
    // ==================================================

    std::size_t displayedTraceCount{ 0 };
    std::size_t latestTraceSize{ 0 };

    bool viewFrozen{ false };
    bool filterDirty{ false };
    bool showAllRows{ false };

    // ==================================================
    // Current trace
    // ==================================================

    const std::vector<CanTraceEntry>*
        currentTrace{ nullptr };

    // ==================================================
    // Internal methods
    // ==================================================

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
};