#pragma once

#include <QPaintEvent>
#include <QSize>
#include <QString>
#include <QWidget>

class SensorGaugeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SensorGaugeWidget(QWidget *parent = nullptr);

    void setLabel(const QString &label);
    void setValue(double value);
    void setUnit(const QString &unit);
    void setRange(double min, double max);

    [[nodiscard]] QString label() const;
    [[nodiscard]] double value() const;
    [[nodiscard]] QString unit() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QString m_label;
    double m_value = 0.0;
    QString m_unit;
    double m_min = -1000.0;
    double m_max = 1000.0;
};
