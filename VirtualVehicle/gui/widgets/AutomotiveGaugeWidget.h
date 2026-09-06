#pragma once

#include <algorithm>
#include <cmath>

#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPalette>
#include <QSizePolicy>
#include <QWidget>
#include <QtMath>

class AutomotiveGaugeWidget : public QWidget
{
public:
    enum class GaugeType
    {
        Speedometer,
        Tachometer
    };

    explicit AutomotiveGaugeWidget(
        GaugeType type,
        QWidget* parent = nullptr
    )
        : QWidget(parent),
        gaugeType(type)
    {
        setMinimumSize(
            180,
            180
        );

        setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding
        );
    }

    void setValue(
        double newValue
    )
    {
        value =
            std::clamp(
                newValue,
                0.0,
                maximumValue()
            );

        update();
    }

protected:
    void paintEvent(
        QPaintEvent*
    ) override
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

        const QColor faceColor =
            darkMode
            ? QColor("#111923")
            : QColor("#F8FAFC");

        const QColor outerRingColor =
            darkMode
            ? QColor("#2B3947")
            : QColor("#D5DEE7");

        const QColor minorTickColor =
            darkMode
            ? QColor("#647586")
            : QColor("#8998A6");

        const QColor majorTickColor =
            darkMode
            ? QColor("#D4DEE7")
            : QColor("#41515F");

        const QColor textColor =
            darkMode
            ? QColor("#F4F7FA")
            : QColor("#13202C");

        const QColor secondaryTextColor =
            darkMode
            ? QColor("#8C9CAB")
            : QColor("#697A8A");

        const QColor blueColor =
            QColor("#3D82BE");

        const QColor redColor =
            QColor("#C94D4A");

        const qreal side =
            std::min(
                width(),
                height()
            );

        const qreal radius =
            side *
            0.405;

        const QPointF center(
            width() /
            2.0,
            height() *
            0.52
        );

        // ==================================================
        // Face
        // ==================================================

        painter.setPen(
            QPen(
                outerRingColor,
                side *
                0.010
            )
        );

        painter.setBrush(
            faceColor
        );

        painter.drawEllipse(
            center,
            radius,
            radius
        );

        painter.setPen(
            QPen(
                blueColor,
                side *
                0.010
            )
        );

        painter.drawArc(
            QRectF(
                center.x() -
                radius *
                0.91,
                center.y() -
                radius *
                0.91,
                radius *
                1.82,
                radius *
                1.82
            ),
            static_cast<int>(
                -150.0 *
                16.0
                ),
            static_cast<int>(
                -240.0 *
                16.0
                )
        );

        if (
            gaugeType ==
            GaugeType::Tachometer
            )
        {
            painter.setPen(
                QPen(
                    redColor,
                    side *
                    0.014,
                    Qt::SolidLine,
                    Qt::RoundCap
                )
            );

            const double redStart =
                angleForRatio(
                    0.80
                );

            const double redEnd =
                angleForRatio(
                    1.0
                );

            painter.drawArc(
                QRectF(
                    center.x() -
                    radius *
                    0.91,
                    center.y() -
                    radius *
                    0.91,
                    radius *
                    1.82,
                    radius *
                    1.82
                ),
                static_cast<int>(
                    -redStart *
                    16.0
                    ),
                static_cast<int>(
                    -(
                        redEnd -
                        redStart
                        ) *
                    16.0
                    )
            );
        }

        // ==================================================
        // Scale
        // ==================================================

        const int majorCount =
            gaugeType ==
            GaugeType::Speedometer
            ? 10
            : 8;

        const int minorPerMajor =
            4;

        for (
            int major = 0;
            major <= majorCount;
            ++major
            )
        {
            const double ratio =
                static_cast<double>(
                    major
                    ) /
                majorCount;

            const double angle =
                angleForRatio(
                    ratio
                );

            drawTick(
                painter,
                center,
                radius,
                angle,
                side *
                0.050,
                side *
                0.009,
                majorTickColor
            );

            drawMajorLabel(
                painter,
                center,
                radius,
                angle,
                major,
                majorCount,
                textColor,
                side
            );

            if (
                major ==
                majorCount
                )
            {
                continue;
            }

            for (
                int minor = 1;
                minor < minorPerMajor;
                ++minor
                )
            {
                const double minorRatio =
                    (
                        major +
                        (
                            static_cast<double>(
                                minor
                                ) /
                            minorPerMajor
                            )
                        ) /
                    majorCount;

                QColor color =
                    minorTickColor;

                if (
                    gaugeType ==
                    GaugeType::Tachometer &&
                    minorRatio >=
                    0.80
                    )
                {
                    color =
                        redColor;
                }

                drawTick(
                    painter,
                    center,
                    radius,
                    angleForRatio(
                        minorRatio
                    ),
                    side *
                    0.027,
                    side *
                    0.005,
                    color
                );
            }
        }

        // ==================================================
        // Needle
        // ==================================================

        const double ratio =
            value /
            maximumValue();

        const double needleAngle =
            angleForRatio(
                ratio
            );

        const double needleRadians =
            qDegreesToRadians(
                needleAngle
            );

        const QPointF needleEnd(
            center.x() +
            std::cos(
                needleRadians
            ) *
            radius *
            0.68,
            center.y() +
            std::sin(
                needleRadians
            ) *
            radius *
            0.68
        );

        painter.setPen(
            QPen(
                blueColor,
                side *
                0.013,
                Qt::SolidLine,
                Qt::RoundCap
            )
        );

        painter.drawLine(
            center,
            needleEnd
        );

        painter.setPen(
            Qt::NoPen
        );

        painter.setBrush(
            blueColor
        );

        painter.drawEllipse(
            center,
            side *
            0.026,
            side *
            0.026
        );

        // ==================================================
        // Title
        // ==================================================

        QFont titleFont =
            painter.font();

        titleFont.setPointSizeF(
            side *
            0.040
        );

        titleFont.setBold(
            true
        );

        painter.setFont(
            titleFont
        );

        painter.setPen(
            secondaryTextColor
        );

        painter.drawText(
            QRectF(
                0,
                height() *
                0.05,
                width(),
                side *
                0.07
            ),
            Qt::AlignCenter,
            gaugeType ==
            GaugeType::Speedometer
            ? "SPEED"
            : "TACHOMETER"
        );

        // ==================================================
        // Digital readout
        // ==================================================

        QFont valueFont =
            painter.font();

        valueFont.setPointSizeF(
            side *
            0.105
        );

        valueFont.setBold(
            true
        );

        painter.setFont(
            valueFont
        );

        painter.setPen(
            textColor
        );

        painter.drawText(
            QRectF(
                center.x() -
                radius *
                0.55,
                center.y() +
                radius *
                0.14,
                radius *
                1.10,
                side *
                0.11
            ),
            Qt::AlignCenter,
            QString::number(
                value,
                'f',
                gaugeType ==
                GaugeType::Speedometer
                ? 0
                : 0
            )
        );

        QFont unitFont =
            painter.font();

        unitFont.setPointSizeF(
            side *
            0.040
        );

        unitFont.setBold(
            false
        );

        painter.setFont(
            unitFont
        );

        painter.setPen(
            secondaryTextColor
        );

        painter.drawText(
            QRectF(
                center.x() -
                radius *
                0.55,
                center.y() +
                radius *
                0.42,
                radius *
                1.10,
                side *
                0.06
            ),
            Qt::AlignCenter,
            gaugeType ==
            GaugeType::Speedometer
            ? "km/h"
            : "RPM"
        );
    }

private:
    GaugeType gaugeType;
    double value{ 0.0 };

    double maximumValue() const
    {
        return gaugeType ==
            GaugeType::Speedometer
            ? 200.0
            : 8000.0;
    }

    double angleForRatio(
        double ratio
    ) const
    {
        return 150.0 +
            std::clamp(
                ratio,
                0.0,
                1.0
            ) *
            240.0;
    }

    void drawTick(
        QPainter& painter,
        const QPointF& center,
        qreal radius,
        double angleDeg,
        qreal length,
        qreal thickness,
        const QColor& color
    ) const
    {
        const double radians =
            qDegreesToRadians(
                angleDeg
            );

        const qreal outerRadius =
            radius *
            0.88;

        const qreal innerRadius =
            outerRadius -
            length;

        const QPointF outer(
            center.x() +
            std::cos(
                radians
            ) *
            outerRadius,
            center.y() +
            std::sin(
                radians
            ) *
            outerRadius
        );

        const QPointF inner(
            center.x() +
            std::cos(
                radians
            ) *
            innerRadius,
            center.y() +
            std::sin(
                radians
            ) *
            innerRadius
        );

        painter.setPen(
            QPen(
                color,
                thickness,
                Qt::SolidLine,
                Qt::RoundCap
            )
        );

        painter.drawLine(
            inner,
            outer
        );
    }

    void drawMajorLabel(
        QPainter& painter,
        const QPointF& center,
        qreal radius,
        double angleDeg,
        int major,
        int majorCount,
        const QColor& color,
        qreal side
    ) const
    {
        const double radians =
            qDegreesToRadians(
                angleDeg
            );

        const qreal labelRadius =
            radius *
            0.67;

        const QPointF labelCenter(
            center.x() +
            std::cos(
                radians
            ) *
            labelRadius,
            center.y() +
            std::sin(
                radians
            ) *
            labelRadius
        );

        QFont font =
            painter.font();

        font.setPointSizeF(
            side *
            0.035
        );

        font.setBold(
            false
        );

        painter.setFont(
            font
        );

        painter.setPen(
            color
        );

        QString label;

        if (
            gaugeType ==
            GaugeType::Speedometer
            )
        {
            label =
                QString::number(
                    major *
                    20
                );
        }
        else
        {
            label =
                QString::number(
                    major
                );
        }

        const qreal boxSize =
            side *
            0.10;

        painter.drawText(
            QRectF(
                labelCenter.x() -
                boxSize /
                2.0,
                labelCenter.y() -
                boxSize /
                2.0,
                boxSize,
                boxSize
            ),
            Qt::AlignCenter,
            label
        );
    }
};
