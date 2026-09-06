#include "VehicleStateWidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPalette>
#include <QStyle>
#include <QVBoxLayout>

class GearIndicatorWidget : public QWidget
{
public:
    explicit GearIndicatorWidget(
        QWidget* parent = nullptr)
        : QWidget(parent)
    {
        setMinimumHeight(
            46
        );

        setMaximumHeight(
            54
        );
    }

    void setGear(
        int gear)
    {
        currentGear =
            gear;

        update();
    }

protected:
    void paintEvent(
        QPaintEvent*) override
    {
        QPainter painter(this);

        painter.setRenderHint(
            QPainter::Antialiasing,
            true
        );

        const bool darkMode =
            palette()
            .color(QPalette::Window)
            .lightness() <
            128;

        const QColor normalFill =
            darkMode
            ? QColor("#17222D")
            : QColor("#F5F8FA");

        const QColor borderColor =
            darkMode
            ? QColor("#344555")
            : QColor("#D7E0E8");

        const QColor textColor =
            darkMode
            ? QColor("#D8E2EA")
            : QColor("#465562");

        const QColor activeColor =
            QColor("#3E7FB9");

        const int gearCount =
            5;

        const qreal gap =
            8.0;

        const qreal available =
            width() -
            gap *
            (
                gearCount -
                1
                );

        const qreal itemWidth =
            available /
            gearCount;

        for (
            int index = 0;
            index < gearCount;
            ++index
            )
        {
            const int gear =
                index +
                1;

            const QRectF rect(
                index *
                (
                    itemWidth +
                    gap
                    ),
                3.0,
                itemWidth,
                height() -
                6.0
            );

            const bool active =
                gear ==
                currentGear;

            painter.setPen(
                QPen(
                    active
                    ? activeColor
                    : borderColor,
                    1.0
                )
            );

            painter.setBrush(
                active
                ? activeColor
                : normalFill
            );

            painter.drawRoundedRect(
                rect,
                7.0,
                7.0
            );

            QFont font =
                painter.font();

            font.setPointSize(
                10
            );

            font.setBold(
                active
            );

            painter.setFont(
                font
            );

            painter.setPen(
                active
                ? QColor("#FFFFFF")
                : textColor
            );

            painter.drawText(
                rect,
                Qt::AlignCenter,
                QString::number(
                    gear
                )
            );
        }
    }

private:
    int currentGear{ 1 };
};

VehicleStateWidget::VehicleStateWidget(
    QWidget* parent)
    : QGroupBox(
        "Vehicle State",
        parent
    )
{
    setObjectName(
        "VehicleStateCard"
    );

    setMinimumWidth(
        430
    );

    setMinimumHeight(
        355
    );

    QVBoxLayout* layout =
        new QVBoxLayout(
            this
        );

    layout->setContentsMargins(
        12,
        18,
        12,
        12
    );

    layout->setSpacing(
        8
    );

    // ==================================================
    // Gauges
    // ==================================================

    QFrame* clusterFrame =
        new QFrame();

    clusterFrame->setObjectName(
        "InstrumentCluster"
    );

    QHBoxLayout* gaugeLayout =
        new QHBoxLayout(
            clusterFrame
        );

    gaugeLayout->setContentsMargins(
        4,
        4,
        4,
        4
    );

    gaugeLayout->setSpacing(
        8
    );

    speedGauge =
        new AutomotiveGaugeWidget(
            AutomotiveGaugeWidget::
            GaugeType::Speedometer
        );

    rpmGauge =
        new AutomotiveGaugeWidget(
            AutomotiveGaugeWidget::
            GaugeType::Tachometer
        );

    gaugeLayout->addWidget(
        speedGauge,
        1
    );

    gaugeLayout->addWidget(
        rpmGauge,
        1
    );

    layout->addWidget(
        clusterFrame,
        1
    );

    // ==================================================
    // Transmission
    // ==================================================

    QFrame* transmissionFrame =
        new QFrame();

    transmissionFrame->setObjectName(
        "TransmissionPanel"
    );

    QVBoxLayout* transmissionLayout =
        new QVBoxLayout(
            transmissionFrame
        );

    transmissionLayout->setContentsMargins(
        10,
        7,
        10,
        7
    );

    transmissionLayout->setSpacing(
        4
    );

    QLabel* transmissionTitle =
        new QLabel(
            "Transmission"
        );

    transmissionTitle->setObjectName(
        "MetricLabel"
    );

    gearIndicatorWidget =
        new GearIndicatorWidget();

    transmissionLayout->addWidget(
        transmissionTitle
    );

    transmissionLayout->addWidget(
        gearIndicatorWidget
    );

    layout->addWidget(
        transmissionFrame
    );

    // ==================================================
    // Brake
    // ==================================================

    QFrame* brakeFrame =
        new QFrame();

    brakeFrame->setObjectName(
        "BrakePanel"
    );

    QHBoxLayout* brakeLayout =
        new QHBoxLayout(
            brakeFrame
        );

    brakeLayout->setContentsMargins(
        10,
        7,
        10,
        7
    );

    QLabel* brakeIcon =
        new QLabel(
            "(!)"
        );

    brakeIcon->setObjectName(
        "BrakeIcon"
    );

    QLabel* brakeLabel =
        new QLabel(
            "Brake"
        );

    brakeLabel->setObjectName(
        "MetricLabel"
    );

    brakeValueLabel =
        new QLabel(
            "RELEASED"
        );

    brakeValueLabel->setObjectName(
        "MetricValue"
    );

    brakeValueLabel->setAlignment(
        Qt::AlignRight |
        Qt::AlignVCenter
    );

    brakeLayout->addWidget(
        brakeIcon
    );

    brakeLayout->addWidget(
        brakeLabel
    );

    brakeLayout->addStretch();

    brakeLayout->addWidget(
        brakeValueLabel
    );

    layout->addWidget(
        brakeFrame
    );

    gearIndicatorWidget->setGear(
        1
    );

    updateBrakeStyle(
        false
    );
}

void VehicleStateWidget::updateState(
    const VehicleState& state)
{
    speedGauge->setValue(
        state.speedKmh
    );

    rpmGauge->setValue(
        state.engineRpm
    );

    gearIndicatorWidget->setGear(
        static_cast<int>(
            state.gear
            )
    );

    brakeValueLabel->setText(
        state.brakeApplied
        ? "APPLIED"
        : "RELEASED"
    );

    updateBrakeStyle(
        state.brakeApplied
    );
}

void VehicleStateWidget::updateBrakeStyle(
    bool brakeApplied)
{
    brakeValueLabel->setProperty(
        "status",
        brakeApplied
        ? "warning"
        : "healthy"
    );

    brakeValueLabel->style()->unpolish(
        brakeValueLabel
    );

    brakeValueLabel->style()->polish(
        brakeValueLabel
    );

    brakeValueLabel->update();
}
