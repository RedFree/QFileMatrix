#pragma once

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QWidget>

class QTimer;

class CameraViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraViewWidget(QWidget *parent = nullptr);

    void setPaused(bool paused);
    void setMeasuring(bool measuring);
    void setStation(int station);
    void setFrameRate(double frameRate);
    void setExposureMs(double exposureMs);
    void setGain(double gain);
    void setCrosshairNormalizedPosition(const QPointF &position);

    [[nodiscard]] bool paused() const;
    [[nodiscard]] bool measuring() const;
    [[nodiscard]] int station() const;
    [[nodiscard]] double frameRate() const;
    [[nodiscard]] double exposureMs() const;
    [[nodiscard]] double gain() const;
    [[nodiscard]] QPointF crosshairNormalizedPosition() const;

signals:
    void crosshairChanged(const QPointF &position);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;

private:
    void updateCrosshair(const QPoint &point);

    bool m_paused = false;
    bool m_measuring = false;
    int m_station = 1;
    double m_frameRate = 22.0;
    double m_exposureMs = 3.2;
    double m_gain = 1.4;
    QPointF m_crosshair {0.38, 0.48};
    QTimer *m_refreshTimer = nullptr;
};
