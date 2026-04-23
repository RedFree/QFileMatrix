#pragma once

#include <QColor>
#include <QPaintEvent>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QWidget>

#include "domain/ProfileData.h"

struct RefBand {
    double x {};
    double width {};
    QString label;
    QColor fill;
    QColor stroke;
};

class ProfileChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileChartWidget(QWidget *parent = nullptr);

    void setProfile(const ProfileData &profile);
    void setMeasuring(bool measuring);

    [[nodiscard]] ProfileData profile() const;
    [[nodiscard]] QVector<RefBand> bands() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;

private:
    struct DragState {
        int bandIndex = -1;
        enum Mode {
            None,
            Move,
            Left,
            Right
        } mode = None;
        double startBandX = 0.0;
        double startBandWidth = 0.0;
        double startDataX = 0.0;
    };

    QRectF plotRect() const;
    double xToPixel(double x) const;
    double yToPixel(double y) const;
    double pixelToDataX(double x) const;
    void updateHover(const QPoint &pos);
    int hitBand(const QPoint &pos, DragState::Mode *mode) const;

    ProfileData m_profile;
    QVector<RefBand> m_bands;
    bool m_measuring = false;
    bool m_hovering = false;
    QPointF m_hoverPoint;
    DragState m_drag;
};
