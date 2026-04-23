#pragma once

#include <QMainWindow>

#include "domain/MeasurementRecord.h"

class AppController;
class BottomStatusBar;
class CameraViewWidget;
class DeviceStatusBar;
class HistoryPanel;
class MeasureControlPanel;
class ProfileChartWidget;
class SensorPanel;
class ServoControlPanel;
class StatCardWidget;
class TopTitleBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void buildUi();
    void wireUi();
    void updateFromController();
    QWidget *createLeftRail();
    QWidget *createCameraPanel();
    QWidget *createProfilePanel();
    QWidget *createStatsPanel();
    void applyRecordToSummary(const MeasurementRecord &record);

    AppController *m_controller = nullptr;
    TopTitleBar *m_topTitleBar = nullptr;
    DeviceStatusBar *m_deviceStatusBar = nullptr;
    BottomStatusBar *m_bottomStatusBar = nullptr;
    CameraViewWidget *m_cameraView = nullptr;
    ProfileChartWidget *m_profileChart = nullptr;
    HistoryPanel *m_historyPanel = nullptr;
    SensorPanel *m_sensorPanel = nullptr;
    MeasureControlPanel *m_measurePanel = nullptr;
    ServoControlPanel *m_servoPanel = nullptr;
    StatCardWidget *m_thicknessCard = nullptr;
    StatCardWidget *m_maxCard = nullptr;
    StatCardWidget *m_minCard = nullptr;
    StatCardWidget *m_deltaCard = nullptr;
    StatCardWidget *m_rightCard = nullptr;
    int m_selectedHistoryRow = 0;
};
