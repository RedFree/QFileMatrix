#include "widgets/CameraViewWidget.h"

#include <QDateTime>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

CameraViewWidget::CameraViewWidget(QWidget *parent)
    : QWidget(parent)
    , m_refreshTimer(new QTimer(this))
{
    setMinimumSize(480, 280);
    setMouseTracking(true);
    connect(m_refreshTimer, &QTimer::timeout, this, qOverload<>(&CameraViewWidget::update));
    m_refreshTimer->setInterval(60);
}

void CameraViewWidget::setPaused(bool paused)
{
    m_paused = paused;
    if (m_measuring && !m_paused) {
        m_refreshTimer->start();
    } else {
        m_refreshTimer->stop();
    }
    update();
}

void CameraViewWidget::setMeasuring(bool measuring)
{
    m_measuring = measuring;
    if (m_measuring && !m_paused) {
        m_refreshTimer->start();
    } else {
        m_refreshTimer->stop();
    }
    update();
}

void CameraViewWidget::setStation(int station)
{
    m_station = station;
    update();
}

void CameraViewWidget::setCrosshairNormalizedPosition(const QPointF &position)
{
    m_crosshair.setX(qBound(0.0, position.x(), 1.0));
    m_crosshair.setY(qBound(0.0, position.y(), 1.0));
    emit crosshairChanged(m_crosshair);
    update();
}

bool CameraViewWidget::paused() const
{
    return m_paused;
}

bool CameraViewWidget::measuring() const
{
    return m_measuring;
}

int CameraViewWidget::station() const
{
    return m_station;
}

QPointF CameraViewWidget::crosshairNormalizedPosition() const
{
    return m_crosshair;
}

void CameraViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#0E1218"));

    for (int x = 0; x < width(); x += 8) {
        const int brightness = 25 + ((x / 8) % 5) * 6;
        painter.fillRect(QRect(x, 0, 4, height()), QColor(brightness, brightness + 2, brightness + 4));
    }

    const QList<QRectF> strips {
        QRectF(width() * 0.05, 0, width() * 0.18, height()),
        QRectF(width() * 0.33, 0, width() * 0.14, height()),
        QRectF(width() * 0.56, 0, width() * 0.17, height()),
        QRectF(width() * 0.84, 0, width() * 0.10, height())
    };
    for (int i = 0; i < strips.size(); ++i) {
        QLinearGradient grad(strips[i].topLeft(), strips[i].topRight());
        const int base = 180 + i * 12;
        grad.setColorAt(0.0, QColor(base - 20, base - 20, base - 15));
        grad.setColorAt(0.5, QColor(base, base, base - 5));
        grad.setColorAt(1.0, QColor(base - 28, base - 28, base - 24));
        painter.fillRect(strips[i], grad);
    }

    painter.setPen(QPen(QColor(110, 170, 255, 180), 1, Qt::DashLine));
    painter.setBrush(QColor(110, 170, 255, 30));
    painter.drawRect(QRectF(width() * 0.32, height() * 0.15, width() * 0.16, height() * 0.7));

    if (m_measuring) {
        const int offset = (QDateTime::currentMSecsSinceEpoch() / 8) % qMax(1, height());
        const QRect scanRect(0, offset - 18, width(), 36);
        QLinearGradient scan(scanRect.topLeft(), scanRect.bottomLeft());
        scan.setColorAt(0.0, QColor(120, 255, 180, 0));
        scan.setColorAt(0.5, QColor(120, 255, 180, 80));
        scan.setColorAt(1.0, QColor(120, 255, 180, 0));
        painter.fillRect(scanRect, scan);
    }

    const QPoint center(static_cast<int>(m_crosshair.x() * width()), static_cast<int>(m_crosshair.y() * height()));
    painter.setPen(QPen(QColor(110, 255, 180), 1));
    painter.drawLine(center.x(), 0, center.x(), height());
    painter.drawLine(0, center.y(), width(), center.y());
    painter.drawEllipse(center, 10, 10);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(6, 12, 22, 180));
    painter.drawRoundedRect(QRect(10, 10, 210, 24), 4, 4);
    painter.drawRoundedRect(QRect(width() - 160, 10, 150, 24), 4, 4);
    painter.setPen(Qt::white);
    painter.drawText(QRect(16, 10, 200, 24), Qt::AlignVCenter | Qt::AlignLeft,
                     QStringLiteral("CAM-01 · %1 · 22.00 FPS").arg(m_paused ? QStringLiteral("PAUSED") : QStringLiteral("LIVE")));
    painter.drawText(QRect(width() - 154, 10, 140, 24), Qt::AlignVCenter | Qt::AlignLeft,
                     QStringLiteral("STN-%1 · 2560x1590").arg(m_station, 2, 10, QLatin1Char('0')));

    painter.setBrush(QColor(6, 12, 22, 180));
    painter.setPen(QColor(200, 247, 212));
    painter.drawRoundedRect(QRect(center.x() + 8, center.y() + 8, 88, 22), 3, 3);
    painter.drawText(QRect(center.x() + 14, center.y() + 8, 80, 22), Qt::AlignVCenter | Qt::AlignLeft,
                     QStringLiteral("%1, %2").arg(center.x()).arg(center.y()));
}

void CameraViewWidget::mousePressEvent(QMouseEvent *event)
{
    updateCrosshair(event->pos());
}

void CameraViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton)) {
        updateCrosshair(event->pos());
    }
}

QSize CameraViewWidget::sizeHint() const
{
    return QSize(700, 420);
}

void CameraViewWidget::updateCrosshair(const QPoint &point)
{
    if (width() <= 0 || height() <= 0) {
        return;
    }

    setCrosshairNormalizedPosition(QPointF(static_cast<double>(point.x()) / width(), static_cast<double>(point.y()) / height()));
}
