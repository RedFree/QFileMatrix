#include "app/AppController.h"

#include <QtGlobal>

AppController::AppController(QObject *parent)
    : QObject(parent)
    , m_model(this)
{
    m_history = m_simulator.seedHistory();
    if (!m_history.isEmpty()) {
        m_state.seq = m_history.front().seq;
    }
    m_profile = m_simulator.generateProfile(static_cast<double>(m_state.seq));
    syncModel();

    m_timer.setInterval(90);
    connect(&m_timer, &QTimer::timeout, this, &AppController::tickMeasurement);
}

const AppState &AppController::state() const
{
    return m_state;
}

MeasurementTableModel *AppController::tableModel()
{
    return &m_model;
}

ProfileData AppController::currentProfile() const
{
    return m_profile;
}

MeasurementRecord AppController::latestRecord() const
{
    return m_history.value(0);
}

void AppController::startMeasurement()
{
    m_state.measuring = true;
    m_state.progress = 0.0;
    m_timer.start();
    emit stateChanged(m_state);
}

void AppController::stopMeasurement()
{
    m_timer.stop();
    m_state.measuring = false;
    m_state.progress = 0.0;
    emit stateChanged(m_state);
}

void AppController::manualSample()
{
    prependRecord(QStringLiteral("手动"));
}

void AppController::setPaused(bool paused)
{
    if (m_state.paused == paused) {
        return;
    }

    m_state.paused = paused;
    emit stateChanged(m_state);
}

void AppController::setSampleCount(int sampleCount)
{
    m_state.sampleCount = qMax(1, sampleCount);
    emit stateChanged(m_state);
}

void AppController::setStep(int step)
{
    m_state.step = qMax(1, step);
    emit stateChanged(m_state);
}

void AppController::jog(int dx, int dy)
{
    m_state.pos.x += dx;
    m_state.pos.y += dy;
    emit stateChanged(m_state);
}

void AppController::goHome()
{
    m_state.pos = MachinePosition{10000, 10000};
    emit stateChanged(m_state);
}

void AppController::tickMeasurement()
{
    if (!m_state.measuring) {
        return;
    }

    m_state.progress += 5.0;
    if (m_state.progress < 100.0) {
        emit stateChanged(m_state);
        return;
    }

    prependRecord(QStringLiteral("自动"));
    stopMeasurement();
}

void AppController::prependRecord(const QString &label)
{
    const int nextSeq = m_state.seq + 1;
    const int nextPoint = m_history.isEmpty() ? 1 : m_history.front().point + 1;
    const auto record = m_simulator.createNextRecord(nextSeq, nextPoint, label);

    m_history.prepend(record);
    m_state.seq = nextSeq;
    m_profile = m_simulator.generateProfile(static_cast<double>(m_state.seq));
    m_model.prependRecord(record);

    emit historyChanged();
    emit profileChanged(m_profile);
    emit latestRecordChanged(record);
}

void AppController::syncModel()
{
    m_model.setRecords(m_history);
}
