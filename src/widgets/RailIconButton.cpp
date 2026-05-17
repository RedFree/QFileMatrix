#include "widgets/RailIconButton.h"

#include <QPainter>

#include "theme/Theme.h"

RailIconButton::RailIconButton(IconKind kind, const QString &text, QWidget *parent)
    : QPushButton(parent)
    , m_kind(kind)
    , m_text(text)
{
    setProperty("railIcon", iconName(kind));
    setAccessibleName(text.isEmpty() ? iconName(kind) : text);
}

QString RailIconButton::iconName(IconKind kind)
{
    switch (kind) {
    case IconKind::Camera: return QStringLiteral("camera");
    case IconKind::Curve: return QStringLiteral("curve");
    case IconKind::Servo: return QStringLiteral("servo");
    case IconKind::History: return QStringLiteral("history");
    case IconKind::Stats: return QStringLiteral("stats");
    case IconKind::Gauge: return QStringLiteral("gauge");
    case IconKind::Settings: return QStringLiteral("settings");
    case IconKind::User: return QStringLiteral("user");
    }
    return QString();
}

void RailIconButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const bool active = property("active").toBool();
    const QColor color = active ? Theme::palette().brand : Theme::palette().textMuted;
    QPen pen(color);
    pen.setWidthF(1.2);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    constexpr int iconSize = 14;
    const int iconX = (width() - iconSize) / 2;
    const int iconY = 5;
    const QRectF r(iconX, iconY, iconSize, iconSize);

    switch (m_kind) {
    case IconKind::Camera:
        painter.drawRoundedRect(r.adjusted(0, 2, 0, 0), 2, 2);
        painter.drawEllipse(r.center(), 3.2, 3.2);
        painter.drawLine(QPointF(r.left() + 2, r.top() + 2), QPointF(r.left() + 5, r.top() + 2));
        break;
    case IconKind::Curve:
        painter.drawPolyline(QPolygonF({QPointF(r.left(), r.center().y() + 2), QPointF(r.left() + 3, r.center().y() + 1), QPointF(r.left() + 6, r.center().y() - 4), QPointF(r.left() + 9, r.center().y() + 4), QPointF(r.right(), r.center().y() - 1)}));
        break;
    case IconKind::Servo:
        painter.drawEllipse(r.center(), 6.0, 6.0);
        painter.drawEllipse(r.center(), 1.6, 1.6);
        painter.drawLine(QPointF(r.center().x(), r.top()), QPointF(r.center().x(), r.top() + 3));
        painter.drawLine(QPointF(r.center().x(), r.bottom()), QPointF(r.center().x(), r.bottom() - 3));
        painter.drawLine(QPointF(r.left(), r.center().y()), QPointF(r.left() + 3, r.center().y()));
        painter.drawLine(QPointF(r.right(), r.center().y()), QPointF(r.right() - 3, r.center().y()));
        break;
    case IconKind::History:
        painter.drawLine(QPointF(r.left() + 2, r.top() + 2), QPointF(r.right(), r.top() + 2));
        painter.drawLine(QPointF(r.left() + 2, r.center().y()), QPointF(r.right(), r.center().y()));
        painter.drawLine(QPointF(r.left() + 2, r.bottom() - 2), QPointF(r.right(), r.bottom() - 2));
        painter.drawEllipse(QPointF(r.left(), r.top() + 2), 0.8, 0.8);
        painter.drawEllipse(QPointF(r.left(), r.center().y()), 0.8, 0.8);
        painter.drawEllipse(QPointF(r.left(), r.bottom() - 2), 0.8, 0.8);
        break;
    case IconKind::Stats:
        painter.drawLine(QPointF(r.left(), r.bottom()), QPointF(r.right(), r.bottom()));
        painter.drawLine(QPointF(r.left() + 1, r.bottom()), QPointF(r.left() + 1, r.center().y() + 2));
        painter.drawLine(QPointF(r.center().x(), r.bottom()), QPointF(r.center().x(), r.top() + 2));
        painter.drawLine(QPointF(r.right() - 1, r.bottom()), QPointF(r.right() - 1, r.center().y() - 1));
        break;
    case IconKind::Gauge:
        painter.drawArc(r.adjusted(0, 2, 0, 6), 0, 180 * 16);
        painter.drawLine(QPointF(r.center().x(), r.center().y() + 2), QPointF(r.right() - 1, r.top() + 4));
        break;
    case IconKind::Settings:
        painter.drawEllipse(r.center(), 4.4, 4.4);
        painter.drawEllipse(r.center(), 1.5, 1.5);
        painter.drawLine(QPointF(r.center().x(), r.top()), QPointF(r.center().x(), r.top() + 2));
        painter.drawLine(QPointF(r.center().x(), r.bottom()), QPointF(r.center().x(), r.bottom() - 2));
        painter.drawLine(QPointF(r.left(), r.center().y()), QPointF(r.left() + 2, r.center().y()));
        painter.drawLine(QPointF(r.right(), r.center().y()), QPointF(r.right() - 2, r.center().y()));
        break;
    case IconKind::User:
        painter.drawEllipse(QPointF(r.center().x(), r.top() + 4), 3.0, 3.0);
        painter.drawArc(QRectF(r.left() + 1, r.center().y() - 1, r.width() - 2, r.height() - 3), 30 * 16, 120 * 16);
        break;
    }

    if (!m_text.isEmpty()) {
        painter.setPen(active ? Theme::palette().brand : Theme::palette().textMuted);
        QFont f = font();
        f.setPointSizeF(8.5);
        f.setBold(active);
        painter.setFont(f);
        const int textTop = iconY + iconSize + 3;
        painter.drawText(QRectF(0, textTop, width(), height() - textTop - 3),
                         Qt::AlignHCenter | Qt::AlignTop, m_text);
    }
}
