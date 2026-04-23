#pragma once

#include <QWidget>

class QLabel;
class QCheckBox;
class QProgressBar;
class QSpinBox;

class MeasureControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit MeasureControlPanel(QWidget *parent = nullptr);

    void setProgress(double progress);
    void setMeasuring(bool measuring);
    void setSampleCount(int sampleCount);

signals:
    void startRequested();
    void stopRequested();
    void manualRequested();
    void sampleCountChanged(int sampleCount);

private:
    QCheckBox *m_autoSampleCheck = nullptr;
    QCheckBox *m_cycleTestCheck = nullptr;
    QLabel *m_stateLabel = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QSpinBox *m_sampleCount = nullptr;
};
