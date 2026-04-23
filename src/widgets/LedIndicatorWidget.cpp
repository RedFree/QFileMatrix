#include "widgets/LedIndicatorWidget.h"

#include <QPainter>
#include <QPainterPath>

#include "theme/Theme.h"

LedIndicatorWidget::LedIndicatorWidget(QWidget *parent)
: QWidget(parent)
{
    setMinimumHeight(24);
    m_blinkTimer.setInterval(500);
    connect(&m_blinkTimer, &QTimer::timeout, this, &LedIndicatorWidget::onBlinkTick);
}

void LedIndicatorWidget::setState(State state)
{
    if (m_state == state)
        return;
    m_state = state;
    m_blinkOn = true;
    const bool shouldBlink = (state == State::Err);
    if (shouldBlink && !m_blinkTimer.isActive())
        m_blinkTimer.start();
    else if (!shouldBlink && m_blinkTimer.isActive())
        m_blinkTimer.stop();
    emit blinkingChanged();
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

bool LedIndicatorWidget::isBlinking() const
{
    return m_state == State::Err;
}

void LedIndicatorWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int bulbDiam = 9;
    const int bulbX = 0;
    const int bulbY = m_compact ? (height() - bulbDiam) / 2 : 4;
    const QRectF bulbRect(bulbX, bulbY, bulbDiam, bulbDiam);
    const QPointF bulbCenter = bulbRect.center();

    if (m_state == State::Err && !m_blinkOn) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(bulbColor().red(), bulbColor().green(), bulbColor().blue(), 89));
        painter.drawEllipse(bulbRect);
    } else if (m_state == State::Off) {
        painter.setPen(Qt::NoPen);
        QColor offColor = bulbColor();
        offColor.setAlpha(102);
        painter.setBrush(offColor);
        painter.drawEllipse(bulbRect);
    } else {
        QColor glow = glowColor();
        if (glow.isValid()) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(glow);
            painter.drawEllipse(bulbCenter, bulbDiam / 2.0 + 2.0, bulbDiam / 2.0 + 2.0);
        }

        painter.setPen(Qt::NoPen);
        painter.setBrush(bulbColor());
        painter.drawEllipse(bulbRect);

        painter.setPen(QPen(QColor(0, 0, 0, 25), 1.0));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(bulbRect.adjusted(0.5, 0.5, -0.5, -0.5));
    }

    const int textX = bulbDiam + 6;
    painter.setPen(Theme::palette().text);
    QFont labelFont = painter.font();
    labelFont.setPixelSize(11);
    labelFont.setWeight(QFont::Medium);
    painter.setFont(labelFont);
    painter.drawText(QRect(textX, 0, width() - textX, m_compact ? height() : 16),
                     Qt::AlignLeft | Qt::AlignVCenter, m_label);

    if (!m_subLabel.isEmpty()) {
        QFont subFont = painter.font();
        subFont.setPixelSize(10);
        subFont.setWeight(QFont::Normal);
        subFont.setFamily(QStringLiteral("Consolas"));
        painter.setFont(subFont);
        painter.setPen(Theme::palette().textMuted);
        const QRect subRect = m_compact
            ? QRect(width() / 2, 0, width() / 2, height())
            : QRect(textX, 14, width() - textX, height() - 14);
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

QColor LedIndicatorWidget::glowColor() const
{
    const auto &p = Theme::palette();
    switch (m_state) {
    case State::Ok: {
        QColor c = p.ok;
        c.setAlpha(51);
        return c;
    }
    case State::Warn: {
        QColor c = p.warn;
        c.setAlpha(51);
        return c;
    }
    case State::Err: {
        QColor c = p.err;
        c.setAlpha(64);
        return c;
    }
    case State::Off:
    default:
        return {};
    }
}

void LedIndicatorWidget::onBlinkTick()
{
    m_blinkOn = !m_blinkOn;
    update();
}
