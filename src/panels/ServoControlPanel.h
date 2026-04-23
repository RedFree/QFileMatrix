#pragma once

#include <QWidget>

#include "app/AppTypes.h"

class DpadControlWidget;
class QLabel;
class QSpinBox;

class ServoControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ServoControlPanel(QWidget *parent = nullptr);

    void setPosition(const MachinePosition &position);
    void setStep(int step);

signals:
    void jogRequested(int dx, int dy);
    void homeRequested();
    void gotoRequested(int x, int y);
    void stepChanged(int step);

private:
    DpadControlWidget *m_dpad = nullptr;
    QLabel *m_jogHintLabel = nullptr;
    QSpinBox *m_stepSpin = nullptr;
    QSpinBox *m_xSpin = nullptr;
    QSpinBox *m_ySpin = nullptr;
};
