#pragma once

#include <string>
#include <vector>

#include <QGroupBox>

#include "../../test/TestCase.h"

class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QTextEdit;

class TestRunnerWidget : public QGroupBox
{
public:
    explicit TestRunnerWidget(
        QWidget* parent = nullptr
    );

    // ==================================================
    // Controls
    // ==================================================

    QPushButton*
        getRunAllButton() const;

    QPushButton*
        getRunSelectedButton() const;

    std::string
        getSelectedTestId() const;

    // ==================================================
    // Results
    // ==================================================

    void setResults(
        const std::vector<TestCase>& results
    );

    void setSingleResult(
        const TestCase& result
    );

    void clear();

    // ==================================================
    // Theme
    // ==================================================

    void setDarkMode(
        bool enabled
    );

private:
    // ==================================================
    // Test selection / execution
    // ==================================================

    QComboBox* testSelector{};

    QPushButton* runSelectedButton{};
    QPushButton* runAllButton{};

    QLabel* suiteStatusLabel{};

    // ==================================================
    // Verification summary
    // ==================================================

    QLabel* executedValueLabel{};
    QLabel* passedValueLabel{};
    QLabel* failedValueLabel{};
    QLabel* requirementsValueLabel{};
    QLabel* passRateValueLabel{};
    QLabel* baselineValueLabel{};

    // ==================================================
    // Results
    // ==================================================

    QTableWidget* resultsTable{};
    QTextEdit* detailText{};

    std::vector<TestCase> currentResults;

    bool darkModeEnabled{ false };

    // ==================================================
    // Helpers
    // ==================================================

    void updateSummary(
        const std::vector<TestCase>& results
    );

    void showTestDetails(
        int row
    );

    void synchronizeSelector(
        const std::vector<TestCase>& results
    );

    QString statusToString(
        TestStatus status
    ) const;

    void applyThemeStyle();

    void refreshDynamicStyle(
        QLabel* label,
        const char* state
    );
};
