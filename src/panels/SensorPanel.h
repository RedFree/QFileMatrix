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
    void refreshDerivedLabels(double value);

    SensorGaugeWidget *m_gauge = nullptr;
};
