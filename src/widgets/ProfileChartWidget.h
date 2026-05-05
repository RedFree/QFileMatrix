#pragma once

#include <QColor>
#include <QPointer>
#include <QString>
#include <QVector>
#include <QWidget>

#include "domain/ProfileData.h"

class QCustomPlot;
class QCPGraph;
class QCPItemStraightLine;
class QCPItemRect;
class QCPItemText;

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
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override;

private:
    void setupPlot();
    void updateGraphData();
    void updateBands();
    void updateTargetLine();
    void updateAxisRanges();

    QPointer<QCustomPlot> m_plot;
    QCPGraph *m_profileGraph = nullptr;
    QCPItemStraightLine *m_targetLine = nullptr;
    QVector<QCPItemRect*> m_bandRects;
    QCPItemText *m_axisLabel = nullptr;

    ProfileData m_profile;
    bool m_measuring = false;
};
