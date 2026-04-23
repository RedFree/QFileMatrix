#include "widgets/LedIndicatorWidget.h"

#include <QPainter>

#include "theme/Theme.h"

LedIndicatorWidget::LedIndicatorWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(24);
}

void LedIndicatorWidget::setState(State state)
{
    m_state = state;
    update();
}

void LedIndicatorWidget::setLabel(const QString &label)
{
    m_label = label;
    update();
}

void LedIndicatorWidget::setSubLabel(const QString &subLabel)
{
    m_subLabel = subLabel;
    update();
}

void LedIndicatorWidget::setCompact(bool compact)
{
    m_compact = compact;
    updateGeometry();
    update();
}

LedIndicatorWidget::State LedIndicatorWidget::state() const
{
    return m_state;
}

QString LedIndicatorWidget::label() const
{
    return m_label;
}

QString LedIndicatorWidget::subLabel() const
{
    return m_subLabel;
}

bool LedIndicatorWidget::compact() const
{
    return m_compact;
}

void LedIndicatorWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QRect bulbRect(0, m_compact ? 7 : 4, 10, 10);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bulbColor());
    painter.drawEllipse(bulbRect);

    painter.setPen(Theme::palette().text);
    painter.drawText(QRect(16, 0, width() - 16, m_compact ? height() : 16), Qt::AlignLeft | Qt::AlignVCenter, m_label);

    if (!m_subLabel.isEmpty()) {
        painter.setPen(Theme::palette().textMuted);
        const QRect subRect = m_compact ? QRect(width() / 2, 0, width() / 2, height()) : QRect(16, 14, width() - 16, height() - 14);
        painter.drawText(subRect, Qt::AlignLeft | Qt::AlignVCenter, m_subLabel);
    }
}

QSize LedIndicatorWidget::sizeHint() const
{
    return m_compact ? QSize(180, 24) : QSize(180, 34);
}

QColor LedIndicatorWidget::bulbColor() const
{
    switch (m_state) {
    case State::Ok:
        return Theme::palette().ok;
    case State::Warn:
        return Theme::palette().warn;
    case State::Err:
        return Theme::palette().err;
    case State::Off:
    default:
        return Theme::palette().textMuted;
    }
}
