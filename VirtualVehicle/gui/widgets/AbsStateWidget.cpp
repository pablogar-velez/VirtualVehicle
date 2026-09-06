#include "AbsStateWidget.h"

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPalette>
#include <QStyle>
#include <QVBoxLayout>

class VehicleDiagramWidget : public QWidget
{
public:
    explicit VehicleDiagramWidget(
        QWidget* parent = nullptr)
        : QWidget(parent)
    {
        setMinimumSize(
            150,
            235
        );

        setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding
        );
    }

    void setState(
        const AbsState& state,
        bool brakeApplied,
        float throttlePercent,
        bool coasting)
    {
        absState =
            state;

        braking =
            brakeApplied;

        throttle =
            throttlePercent;

        coastingActive =
            coasting;

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

        const QColor bodyColor =
            darkMode
            ? QColor("#536574")
            : QColor("#CBD4DC");

        const QColor cabinColor =
            darkMode
            ? QColor("#16212B")
            : QColor("#52616E");

        const QColor outlineColor =
            darkMode
            ? QColor("#8393A2")
            : QColor("#637381");

        const QColor normalColor =
            darkMode
            ? QColor("#536A7D")
            : QColor("#8095A7");

        const QColor accelerationColor =
            QColor("#3E7FB9");

        const QColor brakingColor =
            QColor("#D88432");

        const QColor absColor =
            QColor("#2DA5D7");

        const QColor faultColor =
            QColor("#C94D4A");

        const QColor coastColor =
            darkMode
            ? QColor("#6B7C8B")
            : QColor("#788B9C");

        const QRectF body(
            width() *
            0.30,
            height() *
            0.10,
            width() *
            0.40,
            height() *
            0.66
        );

        painter.setPen(
            QPen(
                outlineColor,
                2.0
            )
        );

        painter.setBrush(
            bodyColor
        );

        painter.drawRoundedRect(
            body,
            24.0,
            24.0
        );

        const QRectF cabin(
            body.left() +
            body.width() *
            0.19,
            body.top() +
            body.height() *
            0.19,
            body.width() *
            0.62,
            body.height() *
            0.48
        );

        painter.setBrush(
            cabinColor
        );

        painter.drawRoundedRect(
            cabin,
            16.0,
            16.0
        );

        const qreal wheelWidth =
            width() *
            0.085;

        const qreal wheelHeight =
            height() *
            0.135;

        const qreal leftX =
            body.left() -
            wheelWidth *
            0.70;

        const qreal rightX =
            body.right() -
            wheelWidth *
            0.30;

        const qreal frontY =
            body.top() +
            body.height() *
            0.17;

        const qreal rearY =
            body.top() +
            body.height() *
            0.65;

        const bool accelerating =
            throttle >
            0.5f;

        auto chooseColor =
            [=](
                bool valid,
                bool front)
            {
                if (
                    front &&
                    !valid
                    )
                {
                    return faultColor;
                }

                if (
                    front &&
                    absState.absActive
                    )
                {
                    return absColor;
                }

                if (braking)
                {
                    return brakingColor;
                }

                if (accelerating)
                {
                    return accelerationColor;
                }

                if (coastingActive)
                {
                    return coastColor;
                }

                return normalColor;
            };

        const QRectF fl(
            leftX,
            frontY,
            wheelWidth,
            wheelHeight
        );

        const QRectF fr(
            rightX,
            frontY,
            wheelWidth,
            wheelHeight
        );

        const QRectF rl(
            leftX,
            rearY,
            wheelWidth,
            wheelHeight
        );

        const QRectF rr(
            rightX,
            rearY,
            wheelWidth,
            wheelHeight
        );

        painter.setPen(
            Qt::NoPen
        );

        painter.setBrush(
            chooseColor(
                absState.frontLeftSensorValid,
                true
            )
        );

        painter.drawRoundedRect(
            fl,
            5.0,
            5.0
        );

        painter.setBrush(
            chooseColor(
                absState.frontRightSensorValid,
                true
            )
        );

        painter.drawRoundedRect(
            fr,
            5.0,
            5.0
        );

        painter.setBrush(
            chooseColor(
                true,
                false
            )
        );

        painter.drawRoundedRect(
            rl,
            5.0,
            5.0
        );

        painter.drawRoundedRect(
            rr,
            5.0,
            5.0
        );

        if (
            braking ||
            accelerating ||
            absState.absActive
            )
        {
            QColor ringColor =
                braking
                ? brakingColor
                : accelerationColor;

            if (
                absState.absActive
                )
            {
                ringColor =
                    absColor;
            }

            painter.setBrush(
                Qt::NoBrush
            );

            painter.setPen(
                QPen(
                    ringColor,
                    2.3
                )
            );

            painter.drawRoundedRect(
                fl.adjusted(
                    -4,
                    -4,
                    4,
                    4
                ),
                7,
                7
            );

            painter.drawRoundedRect(
                fr.adjusted(
                    -4,
                    -4,
                    4,
                    4
                ),
                7,
                7
            );

            painter.setPen(
                QPen(
                    ringColor,
                    1.5
                )
            );

            painter.drawRoundedRect(
                rl.adjusted(
                    -3,
                    -3,
                    3,
                    3
                ),
                6,
                6
            );

            painter.drawRoundedRect(
                rr.adjusted(
                    -3,
                    -3,
                    3,
                    3
                ),
                6,
                6
            );
        }

        // ==================================================
        // Wheel labels
        // ==================================================

        QFont labelFont =
            painter.font();

        labelFont.setPointSize(
            9
        );

        labelFont.setBold(
            true
        );

        painter.setFont(
            labelFont
        );

        painter.setPen(
            darkMode
            ? QColor("#D3DDE6")
            : QColor("#43515F")
        );

        painter.drawText(
            QRectF(
                0,
                frontY -
                18,
                width() *
                0.28,
                18
            ),
            Qt::AlignRight |
            Qt::AlignVCenter,
            "FL"
        );

        painter.drawText(
            QRectF(
                width() *
                0.72,
                frontY -
                18,
                width() *
                0.28,
                18
            ),
            Qt::AlignLeft |
            Qt::AlignVCenter,
            "FR"
        );

        painter.drawText(
            QRectF(
                0,
                rearY +
                wheelHeight +
                2,
                width() *
                0.28,
                18
            ),
            Qt::AlignRight |
            Qt::AlignVCenter,
            "RL"
        );

        painter.drawText(
            QRectF(
                width() *
                0.72,
                rearY +
                wheelHeight +
                2,
                width() *
                0.28,
                18
            ),
            Qt::AlignLeft |
            Qt::AlignVCenter,
            "RR"
        );

        QString activity =
            "CRUISE";

        QColor activityColor =
            coastColor;

        if (accelerating)
        {
            activity =
                "ACCELERATING";

            activityColor =
                accelerationColor;
        }
        else if (braking)
        {
            activity =
                absState.absActive
                ? "ABS BRAKING"
                : "BRAKING";

            activityColor =
                absState.absActive
                ? absColor
                : brakingColor;
        }
        else if (coastingActive)
        {
            activity =
                "COASTING";
        }

        painter.setPen(
            activityColor
        );

        painter.drawText(
            QRectF(
                0,
                height() *
                0.89,
                width(),
                22
            ),
            Qt::AlignCenter,
            activity
        );
    }

private:
    AbsState absState{};
    bool braking{ false };
    float throttle{ 0.0f };
    bool coastingActive{ false };
};

namespace
{
    QWidget* createWheelCard(
        const QString& title,
        QLabel*& valueLabel
    )
    {
        QFrame* frame =
            new QFrame();

        frame->setObjectName(
            "WheelMetricCard"
        );

        QVBoxLayout* layout =
            new QVBoxLayout(
                frame
            );

        layout->setContentsMargins(
            10,
            8,
            10,
            8
        );

        layout->setSpacing(
            3
        );

        QLabel* label =
            new QLabel(
                title
            );

        label->setObjectName(
            "WheelMetricLabel"
        );

        valueLabel =
            new QLabel(
                "0.0 km/h"
            );

        valueLabel->setObjectName(
            "WheelMetricValue"
        );

        layout->addWidget(
            label
        );

        layout->addWidget(
            valueLabel
        );

        return frame;
    }

    QWidget* createStatusCard(
        const QString& title,
        QLabel*& valueLabel
    )
    {
        QFrame* frame =
            new QFrame();

        frame->setObjectName(
            "WheelMetricCard"
        );

        QVBoxLayout* layout =
            new QVBoxLayout(
                frame
            );

        layout->setContentsMargins(
            10,
            8,
            10,
            8
        );

        layout->setSpacing(
            3
        );

        QLabel* label =
            new QLabel(
                title
            );

        label->setObjectName(
            "WheelMetricLabel"
        );

        valueLabel =
            new QLabel();

        valueLabel->setObjectName(
            "StatusValue"
        );

        layout->addWidget(
            label
        );

        layout->addWidget(
            valueLabel
        );

        return frame;
    }
}

AbsStateWidget::AbsStateWidget(
    QWidget* parent)
    : QGroupBox(
        "Vehicle / Wheel State",
        parent
    )
{
    setObjectName(
        "AbsStateCard"
    );

    setMinimumHeight(
        355
    );

    QGridLayout* layout =
        new QGridLayout(
            this
        );

    layout->setContentsMargins(
        12,
        18,
        12,
        12
    );

    layout->setHorizontalSpacing(
        8
    );

    layout->setVerticalSpacing(
        8
    );

    QWidget* frontLeftCard =
        createWheelCard(
            "Front Left",
            frontLeftWheelValueLabel
        );

    QWidget* rearLeftCard =
        createWheelCard(
            "Rear Left",
            rearLeftWheelValueLabel
        );

    vehicleDiagramWidget =
        new VehicleDiagramWidget();

    QWidget* frontRightCard =
        createWheelCard(
            "Front Right",
            frontRightWheelValueLabel
        );

    QWidget* rearRightCard =
        createWheelCard(
            "Rear Right",
            rearRightWheelValueLabel
        );

    layout->addWidget(
        frontLeftCard,
        0,
        0
    );

    layout->addWidget(
        rearLeftCard,
        1,
        0
    );

    layout->addWidget(
        vehicleDiagramWidget,
        0,
        1,
        2,
        1
    );

    layout->addWidget(
        frontRightCard,
        0,
        2
    );

    layout->addWidget(
        rearRightCard,
        1,
        2
    );

    QWidget* absCard =
        createStatusCard(
            "ABS",
            absActiveValueLabel
        );

    QWidget* healthCard =
        createStatusCard(
            "Health",
            absHealthValueLabel
        );

    layout->addWidget(
        absCard,
        2,
        0,
        1,
        2
    );

    layout->addWidget(
        healthCard,
        2,
        2
    );

    layout->setColumnStretch(
        0,
        2
    );

    layout->setColumnStretch(
        1,
        3
    );

    layout->setColumnStretch(
        2,
        2
    );

    absActiveValueLabel->setText(
        "INACTIVE"
    );

    absHealthValueLabel->setText(
        "HEALTHY"
    );

    updateStatusStyle(
        absActiveValueLabel,
        "neutral"
    );

    updateStatusStyle(
        absHealthValueLabel,
        "healthy"
    );
}

void AbsStateWidget::updateState(
    const AbsState& state,
    bool brakeApplied,
    float throttlePercent,
    bool coasting,
    float vehicleSpeedKmh
)
{
    vehicleDiagramWidget->setState(
        state,
        brakeApplied,
        throttlePercent,
        coasting
    );

    if (
        state.frontLeftSensorValid
        )
    {
        frontLeftWheelValueLabel->setText(
            QString::number(
                state.wheelSpeedFrontLeft,
                'f',
                1
            ) +
            " km/h"
        );

        updateStatusStyle(
            frontLeftWheelValueLabel,
            "normal"
        );
    }
    else
    {
        frontLeftWheelValueLabel->setText(
            "INVALID"
        );

        updateStatusStyle(
            frontLeftWheelValueLabel,
            "warning"
        );
    }

    if (
        state.frontRightSensorValid
        )
    {
        frontRightWheelValueLabel->setText(
            QString::number(
                state.wheelSpeedFrontRight,
                'f',
                1
            ) +
            " km/h"
        );

        updateStatusStyle(
            frontRightWheelValueLabel,
            "normal"
        );
    }
    else
    {
        frontRightWheelValueLabel->setText(
            "INVALID"
        );

        updateStatusStyle(
            frontRightWheelValueLabel,
            "warning"
        );
    }

    // Rear wheel speed is not separately modeled in the
    // current backend. For the dashboard, both rear wheels
    // are represented by the vehicle longitudinal speed.
    rearLeftWheelValueLabel->setText(
        QString::number(
            vehicleSpeedKmh,
            'f',
            1
        ) +
        " km/h"
    );

    rearRightWheelValueLabel->setText(
        QString::number(
            vehicleSpeedKmh,
            'f',
            1
        ) +
        " km/h"
    );

    updateStatusStyle(
        rearLeftWheelValueLabel,
        "normal"
    );

    updateStatusStyle(
        rearRightWheelValueLabel,
        "normal"
    );

    absActiveValueLabel->setText(
        state.absActive
        ? "ACTIVE"
        : "INACTIVE"
    );

    updateStatusStyle(
        absActiveValueLabel,
        state.absActive
        ? "healthy"
        : "neutral"
    );

    const bool healthy =
        state.healthStatus ==
        AbsHealthStatus::Healthy;

    absHealthValueLabel->setText(
        healthy
        ? "HEALTHY"
        : "DEGRADED"
    );

    updateStatusStyle(
        absHealthValueLabel,
        healthy
        ? "healthy"
        : "warning"
    );
}

void AbsStateWidget::updateStatusStyle(
    QLabel* label,
    const char* status
)
{
    label->setProperty(
        "status",
        status
    );

    label->style()->unpolish(
        label
    );

    label->style()->polish(
        label
    );

    label->update();
}
