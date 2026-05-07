#pragma once

#include <QWidget>

class SensorGaugeWidget;

class SensorPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SensorPanel(QWidget *parent = nullptr);

    void setSensorValue(double value);
    [[nodiscard]] double sensorValue() const;

private:
    enum class DisplayMode {
        Height,
        Light,
    };

    void refreshDerivedLabels(double value);
    void setDisplayMode(DisplayMode mode);
    void refreshDisplay();

    SensorGaugeWidget *m_gauge = nullptr;
    QWidget *m_detailWrap = nullptr;
    DisplayMode m_displayMode = DisplayMode::Height;
    double m_heightValue = -58.79;
    double m_lightValue = 78.0;
};
