#include "widgets/SensorGaugeWidget.h"

#include <QFont>
#include <QLinearGradient>
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
    painter.drawRoundedRect(barRect, 3, 3);
    painter.fillRect(barRect.adjusted(1, 1, -1, -1), Theme::palette().bgSunken);

    const double ticks[] = {0.0, 0.25, 0.5, 0.75, 1.0};
    for (double t : ticks) {
        const int y = barRect.bottom() - static_cast<int>(t * barRect.height());
        painter.setPen(QPen(Theme::palette().border, 1));
        painter.setOpacity(t == 0.5 ? 1.0 : 0.55);
        painter.drawLine(QPoint(barRect.left(), y), QPoint(barRect.right(), y));
    }
    painter.setOpacity(1.0);

    const double clamped = qBound(0.0, (m_value - m_min) / (m_max - m_min), 1.0);
    const int fillHeight = static_cast<int>((barRect.height() - 4) * clamped);
    const QRect fillRect(barRect.left() + 2, barRect.bottom() - fillHeight - 1, barRect.width() - 4, fillHeight);
    QLinearGradient gradient(fillRect.bottomLeft(), fillRect.topLeft());
    gradient.setColorAt(0.0, Theme::palette().brand);
    gradient.setColorAt(1.0, QColor(106, 139, 230));
    painter.fillRect(fillRect, gradient);

    painter.setPen(Theme::palette().textMuted);
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 9, QFont::DemiBold));
    painter.drawText(QRect(44, 12, width() - 56, 16), Qt::AlignLeft | Qt::AlignVCenter, m_label);
painter.setPen(Theme::palette().text);
QFont valFont(QStringLiteral("Consolas"), 22, QFont::DemiBold);
valFont.setLetterSpacing(QFont::AbsoluteSpacing, -1);
painter.setFont(valFont);
painter.drawText(QRect(44, 28, width() - 56, 28), Qt::AlignLeft | Qt::AlignVCenter, QString::number(m_value, 'f', 2));
const int valWidth = painter.fontMetrics().horizontalAdvance(QString::number(m_value, 'f', 2));
painter.setPen(Theme::palette().textMuted);
painter.setFont(QFont(QStringLiteral("Consolas"), 10));
painter.drawText(QRect(46 + valWidth, 40, 40, 14), Qt::AlignLeft | Qt::AlignVCenter, m_unit);
painter.setFont(QFont(QStringLiteral("Consolas"), 10));
painter.drawText(QRect(44, 68, width() / 2 - 50, 16), Qt::AlignLeft | Qt::AlignVCenter, QString::number(m_min, 'f', 0));
painter.drawText(QRect(width() / 2, 68, width() / 2 - 12, 16), Qt::AlignRight | Qt::AlignVCenter, QString::number(m_max, 'f', 0));
}

QSize SensorGaugeWidget::sizeHint() const
{
    return QSize(220, 120);
}
