#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QVector>

#include "app/AppTypes.h"
#include "domain/MeasurementRecord.h"
#include "domain/MeasurementSimulator.h"
#include "models/MeasurementTableModel.h"

class AppController : public QObject
{
    Q_OBJECT

public:
    explicit AppController(QObject *parent = nullptr);

    [[nodiscard]] const AppState &state() const;
    [[nodiscard]] MeasurementTableModel *tableModel();
    [[nodiscard]] ProfileData currentProfile() const;
    [[nodiscard]] MeasurementRecord latestRecord() const;

public slots:
    void startMeasurement();
    void stopMeasurement();
    void manualSample();
    void setPaused(bool paused);
    void setSampleCount(int sampleCount);
    void setStep(int step);
    void jog(int dx, int dy);
    void goHome();
    void tickMeasurement();

signals:
    void stateChanged(const AppState &state);
    void historyChanged();
    void profileChanged(const ProfileData &profile);
    void latestRecordChanged(const MeasurementRecord &record);

private:
    void prependRecord(const QString &label);
    void syncModel();

    AppState m_state;
    MeasurementSimulator m_simulator;
    MeasurementTableModel m_model;
    QVector<MeasurementRecord> m_history;
    ProfileData m_profile;
    QTimer m_timer;
};
