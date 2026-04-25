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
    void setTargetPoints(int points);
    void setCollectedPoints(int points);
    void setTriggerFrames(int frames);
    void setFrameRate(double fps);
    void setConnected(bool connected);
    void setMeasuring(bool measuring);

private:
    QLabel *m_probeLabel = nullptr;
    QLabel *m_xLabel = nullptr;
    QLabel *m_yLabel = nullptr;
    QLabel *m_targetPointsLabel = nullptr;
    QLabel *m_collectedPointsLabel = nullptr;
    QLabel *m_triggerFramesLabel = nullptr;
    QLabel *m_sampleLabel = nullptr;
    QLabel *m_fpsLabel = nullptr;
    QLabel *m_connectionLabel = nullptr;
    QLabel *m_measureStateLabel = nullptr;
    QWidget *m_connectionLed = nullptr;
};
