#pragma once

#include <cstdint>

#include <QGroupBox>

#include "../../can/CanStatistics.h"
#include "../../ethernet/EthernetNode.h"
#include "../../ethernet/VirtualEthernetBus.h"

class QLabel;
class QPushButton;
class QTableWidget;

class StatisticsWidget : public QGroupBox
{
public:
    explicit StatisticsWidget(
        QWidget* parent = nullptr
    );

    void updateStatistics(
        const CanStatistics& canStatistics,
        std::uint32_t canBitrate,
        double simulationTimeMs,
        const VirtualEthernetBus& ethernetBus,
        const EthernetNode& ethernetNodeA,
        const EthernetNode& ethernetNodeB,
        bool ethernetNodeAFaultActive
    );

    QPushButton* getSendAToBButton() const;
    QPushButton* getSendBToAButton() const;

    void setDarkMode(
        bool enabled
    );

private:
    // ==================================================
    // Summary
    // ==================================================

    QLabel* ethernetStatusValueLabel{};
    QLabel* ethernetRateValueLabel{};
    QLabel* ethernetFramesValueLabel{};
    QLabel* ethernetPayloadBytesValueLabel{};
    QLabel* ethernetNodesValueLabel{};
    QLabel* networkHealthValueLabel{};

    // ==================================================
    // Topology
    // ==================================================

    QLabel* powertrainNodeStatusLabel{};
    QLabel* absNodeStatusLabel{};
    QLabel* steeringNodeStatusLabel{};
    QLabel* dashboardNodeStatusLabel{};

    QLabel* ethernetNodeAStatusLabel{};
    QLabel* ethernetNodeBStatusLabel{};
    QLabel* ethernetBusStatusLabel{};

    QLabel* ethernetNodeAMacLabel{};
    QLabel* ethernetNodeBMacLabel{};

    QLabel* ethernetLinkStatusValueLabel{};
    QLabel* ethernetLinkDescriptionLabel{};

    // ==================================================
    // Ethernet monitor
    // ==================================================

    QLabel* ethernetPendingFramesValueLabel{};
    QLabel* ethernetBusyUntilValueLabel{};

    QLabel* ethernetNodeATxValueLabel{};
    QLabel* ethernetNodeARxValueLabel{};
    QLabel* ethernetNodeBTxValueLabel{};
    QLabel* ethernetNodeBRxValueLabel{};

    QPushButton* sendAToBButton{};
    QPushButton* sendBToAButton{};

    QTableWidget* ethernetTraceTable{};

    bool darkModeEnabled{ false };

    // ==================================================
    // Helpers
    // ==================================================

    void updateEthernetStatistics(
        const VirtualEthernetBus& ethernetBus,
        const EthernetNode& ethernetNodeA,
        const EthernetNode& ethernetNodeB,
        bool ethernetNodeAFaultActive
    );

    void updateEthernetTrace(
        const VirtualEthernetBus& ethernetBus
    );

    void setStatusStyle(
        QLabel* label,
        const QString& state
    );

    void applyThemeStyle();
};
