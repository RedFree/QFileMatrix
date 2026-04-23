#include "widgets/StatCardWidget.h"

#include <QChar>
#include <QFont>
#include <QPainter>
#include <cmath>

#include "theme/Theme.h"

StatCardWidget::StatCardWidget(QWidget *parent)
    : QWidget(parent)
    , m_accentColor(Theme::palette().brand)
{
    setMinimumHeight(80);
}

void StatCardWidget::setLabel(const QString &label)
{
    m_label = label;
    update();
}

void StatCardWidget::setValue(const QString &value)
{
    m_value = value;
    update();
}

void StatCardWidget::setUnit(const QString &unit)
{
    m_unit = unit;
    update();
}

void StatCardWidget::setMetaText(const QString &metaText)
{
    m_metaText = metaText;
    update();
}

void StatCardWidget::setAccentColor(const QColor &accentColor)
{
    m_accentColor = accentColor;
    update();
}

void StatCardWidget::setTarget(const QString &target)
{
    m_target = target;
    update();
}

void StatCardWidget::setTrend(double trend)
{
    m_trend = trend;
    update();
}

QString StatCardWidget::label() const
{
    return m_label;
}

QString StatCardWidget::value() const
{
    return m_value;
}

QString StatCardWidget::unit() const
{
    return m_unit;
}

QString StatCardWidget::metaText() const
{
    return m_metaText;
}

QColor StatCardWidget::accentColor() const
{
    return m_accentColor;
}

QString StatCardWidget::target() const
{
    return m_target;
}

double StatCardWidget::trend() const
{
    return m_trend;
}

void StatCardWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Theme::palette().bgPanel);
    painter.fillRect(QRect(0, 0, 4, height()), m_accentColor);
    painter.setPen(Theme::palette().border);
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 10, 10);
    painter.setPen(Theme::palette().textMuted);
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 9, QFont::DemiBold));
    painter.drawText(QRect(12, 9, width() - 24, 16), Qt::AlignLeft | Qt::AlignVCenter, m_label);
    painter.setPen(m_accentColor);
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 16, QFont::DemiBold));
    painter.drawText(QRect(12, 26, width() - 24, 24), Qt::AlignLeft | Qt::AlignVCenter, m_value);
    painter.setPen(Theme::palette().textMuted);
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 9));
    painter.drawText(QRect(12, 48, width() - 24, 14), Qt::AlignLeft | Qt::AlignVCenter, m_unit);
    if (!m_target.isEmpty()) {
        painter.setFont(QFont(QStringLiteral("Consolas"), 9));
        const auto targetStr = QStringLiteral("TARGET %1").arg(m_target);
        painter.setPen(Theme::palette().textMuted);
        painter.drawText(QRect(12, 62, width() - 24, 14), Qt::AlignLeft | Qt::AlignVCenter, targetStr);
        const auto targetWidth = painter.fontMetrics().horizontalAdvance(targetStr);
        const bool significant = m_trend > 0.0005 || m_trend < -0.0005;
        QString trendStr;
        QColor trendColor;
        if (significant && m_trend > 0) {
            trendStr = QStringLiteral("%1 %2").arg(QChar(0x25B2), QString::number(m_trend, 'f', 3));
            trendColor = Theme::palette().ok;
        } else if (significant && m_trend < 0) {
            trendStr = QStringLiteral("%1 %2").arg(QChar(0x25BC), QString::number(qAbs(m_trend), 'f', 3));
            trendColor = Theme::palette().err;
        } else {
            trendStr = QStringLiteral("— 0.000");
            trendColor = Theme::palette().textMuted;
        }
        painter.setPen(trendColor);
        painter.drawText(QRect(14 + targetWidth, 62, width() - 24 - targetWidth, 14), Qt::AlignLeft | Qt::AlignVCenter, trendStr);
    } else if (!m_metaText.isEmpty()) {
        painter.setPen(Theme::palette().text);
        painter.drawText(QRect(12, 62, width() - 24, 14), Qt::AlignLeft | Qt::AlignVCenter, m_metaText);
    }
}

QSize StatCardWidget::sizeHint() const
{
    return QSize(156, 80);
}
