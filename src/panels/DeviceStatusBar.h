#pragma once

#include <QWidget>

class QProgressBar;
class QLabel;

class DeviceStatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceStatusBar(QWidget *parent = nullptr);

    void setProgress(double progress);
    void setMeasuring(bool measuring);

signals:
    void startRequested();
    void manualRequested();
    void stopRequested();

private:
    QLabel *m_percentLabel = nullptr;
    QLabel *m_progressTitleLabel = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QLabel *m_stateLabel = nullptr;
};
