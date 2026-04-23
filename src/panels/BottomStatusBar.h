#pragma once

#include <QWidget>

#include "app/AppTypes.h"

class QLabel;

class BottomStatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit BottomStatusBar(QWidget *parent = nullptr);

    void setPosition(const MachinePosition &position);
    void setProbeValue(double probeValue);
    void setSampleProgress(int current, int total);
    void setFrameRate(double fps);
    void setConnected(bool connected);
    void setMeasuring(bool measuring);

private:
    QLabel *m_probeLabel = nullptr;
    QLabel *m_xLabel = nullptr;
    QLabel *m_yLabel = nullptr;
    QLabel *m_sampleLabel = nullptr;
    QLabel *m_fpsLabel = nullptr;
    QLabel *m_connectionLabel = nullptr;
    QLabel *m_measureStateLabel = nullptr;
};
