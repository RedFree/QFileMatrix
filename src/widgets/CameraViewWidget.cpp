#include "widgets/CameraViewWidget.h"

#include <QDateTime>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

#include "theme/Theme.h"

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

void CameraViewWidget::setFrameRate(double frameRate)
{
    m_frameRate = frameRate;
    update();
}

void CameraViewWidget::setExposureMs(double exposureMs)
{
    m_exposureMs = exposureMs;
    update();
}

void CameraViewWidget::setGain(double gain)
{
    m_gain = gain;
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

double CameraViewWidget::frameRate() const
{
    return m_frameRate;
}

double CameraViewWidget::exposureMs() const
{
    return m_exposureMs;
}

double CameraViewWidget::gain() const
{
    return m_gain;
}

QPointF CameraViewWidget::crosshairNormalizedPosition() const
{
    return m_crosshair;
}

void CameraViewWidget::paintEvent(QPaintEvent *event)
{
 Q_UNUSED(event)

 const auto &p = Theme::palette();
 QPainter painter(this);
 painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(rect(), p.cameraBg);

  const QList<QPair<QRectF, QColor>> strips {
  { QRectF(width() * 0.07, height() * 0.08, width() * 0.125, height() * 0.84), QColor(QStringLiteral("#3A3F48")) },
  { QRectF(width() * 0.26, height() * 0.08, width() * 0.10, height() * 0.84), QColor(QStringLiteral("#363B44")) },
  { QRectF(width() * 0.42, height() * 0.08, width() * 0.125, height() * 0.84), QColor(QStringLiteral("#3A3F48")) },
  { QRectF(width() * 0.61, height() * 0.08, width() * 0.078, height() * 0.84), QColor(QStringLiteral("#34394A")) }
  };
  for (const auto &strip : strips) {
  painter.fillRect(strip.first, strip.second);
  }

 const QColor roiStroke = p.accentRef;
 painter.setPen(QPen(roiStroke, 1, Qt::DashLine));
  painter.setBrush(Theme::withAlpha(roiStroke, 30));
 painter.drawRect(QRectF(width() * 0.32, height() * 0.15, width() * 0.16, height() * 0.7));

 if (m_measuring) {
 const int offset = (QDateTime::currentMSecsSinceEpoch() / 8) % qMax(1, height());
 const QRect scanRect(0, offset - 18, width(), 36);
 QLinearGradient scan(scanRect.topLeft(), scanRect.bottomLeft());
 const QColor scanColor = p.ok;
  scan.setColorAt(0.0, Theme::withAlpha(scanColor, 0));
  scan.setColorAt(0.5, Theme::withAlpha(scanColor, 80));
  scan.setColorAt(1.0, Theme::withAlpha(scanColor, 0));
 painter.fillRect(scanRect, scan);
 }

 const QPoint center(static_cast<int>(m_crosshair.x() * width()), static_cast<int>(m_crosshair.y() * height()));
 const QColor crosshairColor = p.brand;
 painter.setPen(QPen(crosshairColor, 1));
 painter.drawLine(center.x(), 0, center.x(), height());
 painter.drawLine(0, center.y(), width(), center.y());
 painter.drawEllipse(center, 9, 9);

 const QFont monoFont(QStringLiteral("Consolas"), 11);
 const QFont sansFont(QStringLiteral("Segoe UI"), 11);
 const QFont monoSmall(QStringLiteral("Consolas"), 10);

 painter.setPen(Qt::NoPen);
  painter.setBrush(p.cameraHud);
  painter.drawRoundedRect(QRect(10, 10, 264, 24), 4, 4);
  painter.drawRoundedRect(QRect(width() - 214, 10, 204, 24), 4, 4);
  painter.setPen(p.headerText);
  painter.setBrush(p.ok);
 painter.drawEllipse(QPointF(24, 22), 4, 4);
 painter.setBrush(Qt::NoBrush);
 painter.setFont(sansFont);
 painter.drawText(QRect(32, 10, 238, 24), Qt::AlignVCenter | Qt::AlignLeft,
 QStringLiteral("CAM-01 · %1 · ").arg(m_paused ? QStringLiteral("PAUSED") : QStringLiteral("LIVE")));
 painter.setFont(monoFont);
 painter.drawText(QRect(170, 10, 100, 24), Qt::AlignVCenter | Qt::AlignLeft,
 QStringLiteral("%1 FPS").arg(QString::number(m_frameRate, 'f', 2)));

 painter.setFont(sansFont);
 painter.drawText(QRect(width() - 208, 10, 80, 24), Qt::AlignVCenter | Qt::AlignLeft,
 QStringLiteral("STN-%1 · ").arg(m_station, 2, 10, QLatin1Char('0')));
 painter.setFont(monoFont);
 painter.drawText(QRect(width() - 140, 10, 130, 24), Qt::AlignVCenter | Qt::AlignLeft,
 QStringLiteral("EXP %1ms · GAIN %2x")
 .arg(QString::number(m_exposureMs, 'f', 1))
 .arg(QString::number(m_gain, 'f', 1)));

  painter.setBrush(p.cameraHud);
  painter.setPen(p.ok);
 painter.drawRoundedRect(QRect(center.x() + 8, center.y() + 8, 88, 22), 3, 3);
 painter.setFont(monoSmall);
 painter.drawText(QRect(center.x() + 14, center.y() + 8, 80, 22), Qt::AlignVCenter | Qt::AlignLeft,
 QStringLiteral("%1, %2").arg(center.x()).arg(center.y()));

 painter.setPen(roiStroke);
 painter.setFont(sansFont);
 painter.drawText(QRect(static_cast<int>(width() * 0.32), static_cast<int>(height() * 0.15) - 14, 44, 12), Qt::AlignLeft | Qt::AlignVCenter,
 QStringLiteral("ROI 1"));

  if (m_paused) {
  painter.fillRect(rect(), p.cameraPauseOverlay);
  }
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
