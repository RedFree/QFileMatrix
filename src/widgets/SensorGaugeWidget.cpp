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

    const bool lightMode = m_label.contains(QStringLiteral("光强"));
    const QColor accent = lightMode ? Theme::palette().brandAccent : Theme::palette().brand;
    const QColor accentStrong = lightMode ? Theme::palette().brandStrong : Theme::palette().gaugeHighlight;
    const QColor accentWeak = Theme::withAlpha(accent, lightMode ? 34 : 30);
    const QColor accentBorder = Theme::withAlpha(accent, lightMode ? 170 : 150);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Theme::palette().bgPanel);
    const QRect cardRect = rect().adjusted(0, 0, -1, -1);
    painter.setPen(QPen(accentBorder, 1.2));
    painter.setBrush(accentWeak);
    painter.drawRoundedRect(cardRect, 6, 6);

    const QRect barRect(14, 14, 14, height() - 28);
    painter.setPen(QPen(accentBorder, 1));
    painter.drawRoundedRect(barRect, 4, 4);
    painter.fillRect(barRect.adjusted(1, 1, -1, -1), Theme::withAlpha(accent, 26));

    const double ticks[] = {0.0, 0.25, 0.5, 0.75, 1.0};
    for (double t : ticks) {
        const int y = barRect.bottom() - static_cast<int>(t * barRect.height());
        painter.setPen(QPen(Theme::withAlpha(accent, 120), 1));
        painter.setOpacity(t == 0.5 ? 1.0 : 0.55);
        painter.drawLine(QPoint(barRect.left(), y), QPoint(barRect.right(), y));
    }
    painter.setOpacity(1.0);

    const double clamped = qBound(0.0, (m_value - m_min) / (m_max - m_min), 1.0);
    const int fillHeight = static_cast<int>((barRect.height() - 4) * clamped);
    const QRect fillRect(barRect.left() + 2, barRect.bottom() - fillHeight - 1, barRect.width() - 4, fillHeight);
    QLinearGradient gradient(fillRect.bottomLeft(), fillRect.topLeft());
    gradient.setColorAt(0.0, accent);
    gradient.setColorAt(1.0, accentStrong);
    painter.fillRect(fillRect, gradient);

    painter.setPen(accentStrong);
    QFont labelFont(QStringLiteral("Segoe UI"), 9, QFont::DemiBold);
    labelFont.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    painter.setFont(labelFont);
    painter.drawText(QRect(44, 14, width() - 58, 16), Qt::AlignLeft | Qt::AlignVCenter, m_label.toUpper());
    painter.setPen(Theme::palette().text);
    QFont valFont(QStringLiteral("Consolas"), 24, QFont::DemiBold);
    valFont.setLetterSpacing(QFont::AbsoluteSpacing, -1);
    painter.setFont(valFont);
    const QString valueText = QString::number(m_value, 'f', 2);
    painter.drawText(QRect(44, 30, width() - 58, 30), Qt::AlignLeft | Qt::AlignVCenter, valueText);
    const int valWidth = painter.fontMetrics().horizontalAdvance(valueText);
    painter.setPen(accentStrong);
    painter.setFont(QFont(QStringLiteral("Consolas"), 10, QFont::DemiBold));
    painter.drawText(QRect(48 + valWidth, 43, 40, 14), Qt::AlignLeft | Qt::AlignVCenter, m_unit);
    painter.setFont(QFont(QStringLiteral("Consolas"), 10));
    painter.setPen(Theme::palette().textMuted);
    painter.drawText(QRect(44, 74, width() / 2 - 50, 16), Qt::AlignLeft | Qt::AlignVCenter, QString::number(m_min, 'f', 0));
    painter.drawText(QRect(width() / 2, 74, width() / 2 - 14, 16), Qt::AlignRight | Qt::AlignVCenter, QString::number(m_max, 'f', 0));
}

QSize SensorGaugeWidget::sizeHint() const
{
    return QSize(220, 120);
}
