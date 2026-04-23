#include "widgets/StatCardWidget.h"

#include <QFont>
#include <QPainter>

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
    if (!m_metaText.isEmpty()) {
        painter.setPen(Theme::palette().text);
        painter.drawText(QRect(12, 62, width() - 24, 14), Qt::AlignLeft | Qt::AlignVCenter, m_metaText);
    }
}

QSize StatCardWidget::sizeHint() const
{
    return QSize(156, 80);
}
