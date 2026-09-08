#pragma once

#include <cstdint>

#include <QGroupBox>

#include "../../diagnostics/DtcManager.h"
#include "../../diagnostics/UdsRequest.h"
#include "../../diagnostics/UdsResponse.h"

class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QListWidget;

class FaultInjectionWidget : public QGroupBox
{
public:
    explicit FaultInjectionWidget(
        QWidget* parent = nullptr
    );

    // ==================================================
    // Existing fault-injection controls
    // ==================================================

    QPushButton* getFrontLeftDropoutButton() const;
    QPushButton* getFrontRightDropoutButton() const;

    QPushButton* getClearFrontLeftFaultButton() const;
    QPushButton* getClearFrontRightFaultButton() const;

    QPushButton* getAbsCanDropoutButton() const;
    QPushButton* getClearAbsCanFaultButton() const;

    QPushButton* getPowertrainCanDropoutButton() const;
    QPushButton* getClearPowertrainCanFaultButton() const;

    QPushButton* getSteeringCanDropoutButton() const;
    QPushButton* getClearSteeringCanFaultButton() const;

    QPushButton* getEthernetNodeADropoutButton() const;
    QPushButton* getClearEthernetNodeAFaultButton() const;

    QPushButton* getClearAllFaultsButton() const;

    // ==================================================
    // UDS controls
    // ==================================================

    QPushButton* getSendUdsRequestButton() const;

    UdsRequest buildSelectedUdsRequest() const;

    void showSubmittedRequest(
        const UdsRequest& request
    );

    void showUdsResponse(
        const UdsResponse& response
    );

    // ==================================================
    // Runtime refresh
    // ==================================================

    void updateDiagnostics(
        const DtcManager& dtcManager,
        std::uint8_t udsSession,
        bool udsTransactionPending
    );

    void clearRuntimeView();

    void setDarkMode(
        bool enabled
    );

private:
    // ==================================================
    // Diagnostic summary
    // ==================================================

    QLabel* ecuStatusValueLabel{};
    QLabel* udsStatusValueLabel{};
    QLabel* sessionValueLabel{};
    QLabel* activeDtcCountValueLabel{};
    QLabel* transportValueLabel{};

    // ==================================================
    // Fault injection
    // ==================================================

    QLabel* frontLeftStatusLabel{};
    QLabel* frontRightStatusLabel{};
    QLabel* absCanStatusLabel{};
    QLabel* powertrainCanStatusLabel{};
    QLabel* steeringCanStatusLabel{};
    QLabel* ethernetNodeAStatusLabel{};

    QPushButton* frontLeftDropoutButton{};
    QPushButton* frontRightDropoutButton{};
    QPushButton* absCanDropoutButton{};
    QPushButton* powertrainCanDropoutButton{};
    QPushButton* steeringCanDropoutButton{};
    QPushButton* ethernetNodeADropoutButton{};

    QPushButton* clearFrontLeftFaultButton{};
    QPushButton* clearFrontRightFaultButton{};
    QPushButton* clearAbsCanFaultButton{};
    QPushButton* clearPowertrainCanFaultButton{};
    QPushButton* clearSteeringCanFaultButton{};
    QPushButton* clearEthernetNodeAFaultButton{};
    QPushButton* clearAllFaultsButton{};

    // ==================================================
    // DTC view
    // ==================================================

    QTableWidget* dtcTable{};
    QLabel* dtcEmptyStateLabel{};

    // ==================================================
    // Live diagnostic monitor
    // ==================================================

    QLabel* monitorOverallStateLabel{};

    QLabel* monitorPowertrainEcuLabel{};
    QLabel* monitorAbsEcuLabel{};
    QLabel* monitorSteeringEcuLabel{};
    QLabel* monitorDashboardEcuLabel{};

    QLabel* monitorAbsCanLabel{};
    QLabel* monitorPowertrainCanLabel{};
    QLabel* monitorSteeringCanLabel{};
    QLabel* monitorEthernetLabel{};
    QLabel* monitorFrontLeftLabel{};
    QLabel* monitorFrontRightLabel{};

    QLabel* selectedFaultTitleLabel{};
    QLabel* selectedFaultImpactLabel{};
    QLabel* selectedFaultActionLabel{};

    // ==================================================
    // UDS tester
    // ==================================================

    QComboBox* udsServiceCombo{};
    QLabel* udsServiceDescriptionLabel{};
    QPushButton* sendUdsRequestButton{};

    QLabel* requestCanIdLabel{};
    QLabel* responseCanIdLabel{};

    QLabel* requestBytesLabel{};
    QLabel* responseBytesLabel{};
    QLabel* responseStatusLabel{};

    bool darkModeEnabled{ false };

    // ==================================================
    // Helpers
    // ==================================================

    void updateDtcTable(
        const DtcManager& dtcManager
    );

    void updateFaultStatusCards(
        const DtcManager& dtcManager
    );

    void updateLiveMonitor(
        const DtcManager& dtcManager
    );

    void updateRecommendedAction(
        std::uint32_t dtcCode
    );

    void refreshDynamicStyle(
        QLabel* label,
        const char* state
    );

    void applyThemeStyle();

    static QString formatDtcCode(
        std::uint32_t code
    );

    static QString formatBytes(
        const UdsRequest& request
    );

    static QString formatBytes(
        const UdsResponse& response
    );

    static QString dtcStatusText(
        DtcStatus status
    );

    static QString sessionText(
        std::uint8_t session
    );
};
