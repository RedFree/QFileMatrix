#include "widgets/ProfileChartWidget.h"

#include <QFont>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "theme/Theme.h"

ProfileChartWidget::ProfileChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(420, 260);
    setMouseTracking(true);
    const auto &p = Theme::palette();
    m_bands = {
        {250.0, 80.0, QStringLiteral("起始"), Theme::withAlpha(p.accentRef, 40), Theme::withAlpha(p.accentRef, 180)},
        {600.0, 120.0, QStringLiteral("数据"), Theme::withAlpha(p.ok, 30), Theme::withAlpha(p.ok, 180)},
        {900.0, 90.0, QStringLiteral("结束"), Theme::withAlpha(p.accentRef, 40), Theme::withAlpha(p.accentRef, 180)}
    };
}

void ProfileChartWidget::setProfile(const ProfileData &profile)
{
    m_profile = profile;
    update();
}

void ProfileChartWidget::setMeasuring(bool measuring)
{
    m_measuring = measuring;
    update();
}

ProfileData ProfileChartWidget::profile() const
{
    return m_profile;
}

QVector<RefBand> ProfileChartWidget::bands() const
{
    return m_bands;
}

void ProfileChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Theme::palette().bgPanel);

    const QRectF plot = plotRect();
    painter.setPen(QPen(Theme::palette().divider, 1, Qt::DotLine));
    for (double gy : {-20.0, 0.0, 20.0, 40.0, 60.0, 80.0, 100.0}) {
        if (gy == 0.0) {
            painter.setPen(QPen(Theme::palette().divider, 1, Qt::SolidLine));
        } else {
            painter.setPen(QPen(Theme::palette().divider, 1, Qt::DotLine));
        }
        painter.drawLine(QPointF(plot.left(), yToPixel(gy)), QPointF(plot.right(), yToPixel(gy)));
    }
    painter.setPen(QPen(Theme::palette().divider, 1, Qt::DotLine));
    for (double gx : {0.0, 250.0, 500.0, 750.0, 1000.0, 1250.0}) {
        painter.drawLine(QPointF(xToPixel(gx), plot.top()), QPointF(xToPixel(gx), plot.bottom()));
    }

    painter.setPen(QPen(Theme::palette().textMuted, 1, Qt::SolidLine));
    painter.setOpacity(0.4);
    painter.drawLine(QPointF(plot.left(), yToPixel(0.0)), QPointF(plot.right(), yToPixel(0.0)));
    painter.setOpacity(1.0);

    painter.setPen(Theme::palette().textMuted);
    painter.setFont(QFont(QStringLiteral("Consolas"), 8));
    for (double gy : {-20.0, 0.0, 20.0, 40.0, 60.0, 80.0, 100.0}) {
        const double py = yToPixel(gy);
        painter.drawText(QRectF(0, py - 8, plot.left() - 4, 16), Qt::AlignRight | Qt::AlignVCenter, QString::number(static_cast<int>(gy)));
    }
    for (double gx : {0.0, 250.0, 500.0, 750.0, 1000.0, 1250.0}) {
        const double px = xToPixel(gx);
        painter.drawText(QRectF(px - 20, plot.bottom() + 2, 40, 14), Qt::AlignHCenter | Qt::AlignTop, QString::number(static_cast<int>(gx)));
    }

    for (const auto &band : m_bands) {
        const QRectF bandRect(xToPixel(band.x), plot.top(), xToPixel(band.x + band.width) - xToPixel(band.x), plot.height());
        painter.fillRect(bandRect, band.fill);
        painter.setPen(QPen(band.stroke, 1.5));
        painter.drawRect(bandRect);
        painter.drawText(QRectF(bandRect.left() + 4, plot.top() + 4, 40, 16), band.label);
        painter.setPen(band.stroke);
        painter.setFont(QFont(QStringLiteral("Consolas"), 7));
        painter.drawText(QRectF(bandRect.left() + 2, plot.bottom() - 16, 40, 14), Qt::AlignLeft | Qt::AlignVCenter, QString::number(static_cast<int>(band.x)));
        painter.drawText(QRectF(bandRect.right() - 42, plot.bottom() - 16, 40, 14), Qt::AlignRight | Qt::AlignVCenter, QString::number(static_cast<int>(band.x + band.width)));
    }

    painter.setPen(QPen(Theme::palette().ok, 1, Qt::DashLine));
    const double targetY = yToPixel(11.5);
    painter.drawLine(QPointF(plot.left(), targetY), QPointF(plot.right(), targetY));
    painter.setPen(Theme::palette().ok);
    painter.setFont(QFont(QStringLiteral("Consolas"), 8));
    painter.drawText(QRectF(plot.right() - 72, targetY - 10, 68, 14), Qt::AlignRight | Qt::AlignVCenter, QStringLiteral("TARGET 11.5"));

    if (!m_profile.isEmpty()) {
        const auto &p = Theme::palette();
        QPainterPath path;
        path.moveTo(xToPixel(m_profile.first().x), yToPixel(m_profile.first().y));
        for (int i = 1; i < m_profile.size(); ++i) {
            path.lineTo(xToPixel(m_profile[i].x), yToPixel(m_profile[i].y));
        }

        QPainterPath fillPath = path;
        fillPath.lineTo(xToPixel(m_profile.last().x), plot.bottom());
        fillPath.lineTo(xToPixel(m_profile.first().x), plot.bottom());
        fillPath.closeSubpath();
        painter.fillPath(fillPath, Theme::withAlpha(p.accentTrace, 15));

        painter.setPen(QPen(p.accentTrace, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(path);
    }

    painter.setPen(QPen(Theme::palette().border));
        painter.setBrush(Theme::palette().bgPanel);
        painter.drawRoundedRect(QRectF(plot.left() + 6, plot.top() + 4, 74, 18), 3, 3);
        painter.setPen(Theme::palette().textMuted);
        painter.setFont(QFont(QStringLiteral("Consolas"), 9));
        painter.drawText(QRectF(plot.left() + 10, plot.top() + 4, 66, 18), Qt::AlignVCenter | Qt::AlignHCenter, QStringLiteral("μm vs px"));

        if (m_measuring) {
        const auto &p = Theme::palette();
        painter.fillRect(QRectF(plot.left(), plot.top(), plot.width() * 0.25, plot.height()), Theme::withAlpha(p.ok, 20));
        }

        if (m_hovering) {
        const auto &p = Theme::palette();
        painter.setPen(QPen(p.brand, 1, Qt::DashLine));
        painter.drawLine(QPointF(xToPixel(m_hoverPoint.x()), plot.top()), QPointF(xToPixel(m_hoverPoint.x()), plot.bottom()));
        painter.setBrush(p.brand);
        painter.drawEllipse(QPointF(xToPixel(m_hoverPoint.x()), yToPixel(m_hoverPoint.y())), 3, 3);
        painter.setBrush(Theme::palette().bgPanel);
        painter.setPen(Theme::palette().border);
        painter.drawRoundedRect(QRectF(width() - 180, 10, 170, 22), 4, 4);
        painter.setPen(Theme::palette().textMuted);
        painter.setFont(QFont(QStringLiteral("Consolas"), 10));
        painter.drawText(QRectF(width() - 174, 10, 164, 22), Qt::AlignVCenter | Qt::AlignLeft,
            QStringLiteral("X: %1 px  Y: ").arg(QString::number(m_hoverPoint.x(), 'f', 0)));
        const auto xPartWidth = painter.fontMetrics().horizontalAdvance(
            QStringLiteral("X: %1 px  Y: ").arg(QString::number(m_hoverPoint.x(), 'f', 0)));
        painter.setPen(p.accentTrace);
        painter.drawText(QRectF(width() - 174 + xPartWidth, 10, 164 - xPartWidth, 22), Qt::AlignVCenter | Qt::AlignLeft,
            QStringLiteral("%1 μm").arg(QString::number(m_hoverPoint.y(), 'f', 3)));
        }
}

void ProfileChartWidget::mousePressEvent(QMouseEvent *event)
{
    DragState::Mode mode = DragState::None;
    const int index = hitBand(event->pos(), &mode);
    if (index >= 0) {
        m_drag.bandIndex = index;
        m_drag.mode = mode;
        m_drag.startBandX = m_bands[index].x;
        m_drag.startBandWidth = m_bands[index].width;
        m_drag.startDataX = pixelToDataX(event->position().x());
    }
    updateHover(event->pos());
}

void ProfileChartWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drag.bandIndex >= 0) {
        auto &band = m_bands[m_drag.bandIndex];
        const double currentX = pixelToDataX(event->position().x());
        const double dx = currentX - m_drag.startDataX;

        if (m_drag.mode == DragState::Move) {
            band.x = qBound(0.0, m_drag.startBandX + dx, 1280.0 - band.width);
        } else if (m_drag.mode == DragState::Left) {
            const double newX = qBound(0.0, m_drag.startBandX + dx, m_drag.startBandX + m_drag.startBandWidth - 10.0);
            band.width = m_drag.startBandWidth - (newX - m_drag.startBandX);
            band.x = newX;
        } else if (m_drag.mode == DragState::Right) {
            band.width = qBound(10.0, m_drag.startBandWidth + dx, 1280.0 - band.x);
        }
        update();
        return;
    }

    updateHover(event->pos());
}

void ProfileChartWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    m_drag = {};
}

QSize ProfileChartWidget::sizeHint() const
{
    return QSize(560, 320);
}

QRectF ProfileChartWidget::plotRect() const
{
    return QRectF(40, 14, width() - 54, height() - 38);
}

double ProfileChartWidget::xToPixel(double x) const
{
    const QRectF plot = plotRect();
    return plot.left() + (x / 1280.0) * plot.width();
}

double ProfileChartWidget::yToPixel(double y) const
{
    const QRectF plot = plotRect();
    constexpr double minY = -25.0;
    constexpr double maxY = 100.0;
    return plot.top() + (1.0 - (y - minY) / (maxY - minY)) * plot.height();
}

double ProfileChartWidget::pixelToDataX(double x) const
{
    const QRectF plot = plotRect();
    return qBound(0.0, ((x - plot.left()) / plot.width()) * 1280.0, 1280.0);
}

void ProfileChartWidget::updateHover(const QPoint &pos)
{
    if (m_profile.isEmpty() || !plotRect().contains(pos)) {
        m_hovering = false;
        update();
        return;
    }

    const double dataX = pixelToDataX(pos.x());
    const int index = qBound(0, static_cast<int>(qRound(dataX / 4.0)), m_profile.size() - 1);
    m_hoverPoint = QPointF(m_profile[index].x, m_profile[index].y);
    m_hovering = true;
    update();
}

int ProfileChartWidget::hitBand(const QPoint &pos, DragState::Mode *mode) const
{
    const QRectF plot = plotRect();
    for (int i = 0; i < m_bands.size(); ++i) {
        const QRectF bandRect(xToPixel(m_bands[i].x), plot.top(), xToPixel(m_bands[i].x + m_bands[i].width) - xToPixel(m_bands[i].x), plot.height());
        const QRectF leftHandle(bandRect.left() - 4, bandRect.top(), 8, bandRect.height());
        const QRectF rightHandle(bandRect.right() - 4, bandRect.top(), 8, bandRect.height());
        if (leftHandle.contains(pos)) {
            *mode = DragState::Left;
            return i;
        }
        if (rightHandle.contains(pos)) {
            *mode = DragState::Right;
            return i;
        }
        if (bandRect.contains(pos)) {
            *mode = DragState::Move;
            return i;
        }
    }

    *mode = DragState::None;
    return -1;
}
