#include "widgets/SensorGaugeWidget.h"

#include <QPainter>

#include "theme/Theme.h"

SensorGaugeWidget::SensorGaugeWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(110);
}

void SensorGaugeWidget::setLabel(const QString &label)
{
    m_label = label;
    update();
}

void SensorGaugeWidget::setValue(double value)
{
    m_value = value;
    update();
}

void SensorGaugeWidget::setUnit(const QString &unit)
{
    m_unit = unit;
    update();
}

void SensorGaugeWidget::setRange(double min, double max)
{
    m_min = min;
    m_max = max;
    update();
}

QString SensorGaugeWidget::label() const
{
    return m_label;
}

double SensorGaugeWidget::value() const
{
    return m_value;
}

QString SensorGaugeWidget::unit() const
{
    return m_unit;
}

void SensorGaugeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Theme::palette().bgPanel);

    const QRect barRect(12, 12, 18, height() - 24);
    painter.setPen(Theme::palette().border);
    painter.drawRect(barRect);
    painter.fillRect(barRect.adjusted(1, 1, -1, -1), Theme::palette().bgSunken);

    const double clamped = qBound(0.0, (m_value - m_min) / (m_max - m_min), 1.0);
    const int fillHeight = static_cast<int>((barRect.height() - 4) * clamped);
    const QRect fillRect(barRect.left() + 2, barRect.bottom() - fillHeight - 1, barRect.width() - 4, fillHeight);
    painter.fillRect(fillRect, Theme::palette().brand);

    painter.setPen(Theme::palette().textMuted);
    painter.drawText(QRect(44, 12, width() - 56, 18), Qt::AlignLeft | Qt::AlignVCenter, m_label);
    painter.setPen(Theme::palette().text);
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 16, QFont::DemiBold));
    painter.drawText(QRect(44, 34, width() - 56, 26), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1 %2").arg(QString::number(m_value, 'f', 2), m_unit));
    painter.setPen(Theme::palette().textMuted);
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 9));
    painter.drawText(QRect(44, 68, width() - 56, 18), Qt::AlignLeft | Qt::AlignVCenter,
                     QStringLiteral("%1 .. %2").arg(QString::number(m_min, 'f', 0), QString::number(m_max, 'f', 0)));
}

QSize SensorGaugeWidget::sizeHint() const
{
    return QSize(220, 120);
}
