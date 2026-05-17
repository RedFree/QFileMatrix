#include "widgets/SmallIconButton.h"

#include <QPainter>

#include "theme/Theme.h"

SmallIconButton::SmallIconButton(Icon icon, const QString &text, QWidget *parent)
    : QPushButton(text, parent)
    , m_icon(icon)
{
    const auto &p = Theme::palette();
    setFixedHeight(24);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(QStringLiteral(
        "QPushButton{background:transparent;border:none;border-radius:6px;"
        "padding:0 8px 0 22px;color:%1;font-size:11px;}"
        "QPushButton:hover{background:%2;color:%3;}"
    ).arg(p.textMuted.name(), p.bgSunken.name(), p.text1.name()));
}

void SmallIconButton::setIconType(Icon icon)
{
    m_icon = icon;
    update();
}

void SmallIconButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    const auto &p = Theme::palette();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QColor iconColor = underMouse() ? p.text1 : p.textMuted;
    QPen pen(iconColor);
    pen.setWidthF(1.4);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    const qreal cx = 10, cy = height() / 2.0;
    switch (m_icon) {
    case Icon::Play:
        painter.setBrush(iconColor);
        painter.drawPolygon(QPolygonF({
            QPointF(cx - 3, cy - 4),
            QPointF(cx - 3, cy + 4),
            QPointF(cx + 4, cy)}));
        break;
    case Icon::Pause:
        painter.setBrush(iconColor);
        painter.drawRect(QRectF(cx - 4, cy - 3.5, 2.8, 7));
        painter.drawRect(QRectF(cx + 1.2, cy - 3.5, 2.8, 7));
        break;
    case Icon::Expand:
        painter.drawLine(QPointF(cx - 4, cy - 4), QPointF(cx - 1, cy - 4));
        painter.drawLine(QPointF(cx - 4, cy - 4), QPointF(cx - 4, cy - 1));
        painter.drawLine(QPointF(cx + 4, cy - 4), QPointF(cx + 1, cy - 4));
        painter.drawLine(QPointF(cx + 4, cy - 4), QPointF(cx + 4, cy - 1));
        painter.drawLine(QPointF(cx - 4, cy + 4), QPointF(cx - 1, cy + 4));
        painter.drawLine(QPointF(cx - 4, cy + 4), QPointF(cx - 4, cy + 1));
        painter.drawLine(QPointF(cx + 4, cy + 4), QPointF(cx + 1, cy + 4));
        painter.drawLine(QPointF(cx + 4, cy + 4), QPointF(cx + 4, cy + 1));
        break;
    }
}
