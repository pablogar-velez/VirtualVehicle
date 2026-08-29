#pragma once

#include <string>
#include <vector>

#include <QGroupBox>

#include "../../test/TestCase.h"

class QLabel;
class QPushButton;
class QTableWidget;
class QTextEdit;
class QComboBox;

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

private:
    // ==================================================
    // Test selection
    // ==================================================

    QComboBox* testSelector{};

    QPushButton* runSelectedButton{};
    QPushButton* runAllButton{};

    QLabel* summaryLabel{};

    // ==================================================
    // Results
    // ==================================================

    QTableWidget* resultsTable{};

    QTextEdit* detailText{};

    std::vector<TestCase> currentResults;

    // ==================================================
    // Helpers
    // ==================================================

    void updateSummary(
        const std::vector<TestCase>& results
    );

    void showTestDetails(
        int row
    );

    QString statusToString(
        TestStatus status
    ) const;
};