#pragma once

#include <QString>
#include <QWidget>

#include "app/AppTypes.h"

class QLabel;
class QPushButton;

class DpadControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DpadControlWidget(QWidget *parent = nullptr);

    void setPosition(const MachinePosition &position);
    void setStep(int step);

    [[nodiscard]] MachinePosition position() const;
    [[nodiscard]] int step() const;

signals:
    void jogRequested(int dx, int dy);
    void homeRequested();

private:
    void updateReadout();

    MachinePosition m_position;
    int m_step = 200;
    QLabel *m_xValue = nullptr;
    QLabel *m_yValue = nullptr;
    QLabel *m_stepValue = nullptr;
};
