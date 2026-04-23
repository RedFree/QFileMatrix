#pragma once

#include <QString>
#include <QVector>

#include "domain/MeasurementRecord.h"
#include "domain/ProfileData.h"
#include "domain/MeasurementSpec.h"

class MeasurementSimulator
{
public:
    MeasurementSimulator();

    [[nodiscard]] QVector<MeasurementRecord> seedHistory() const;
    [[nodiscard]] ProfileData generateProfile(double seed) const;
    [[nodiscard]] double randomThickness() const;
    [[nodiscard]] MeasurementRecord createNextRecord(int seq, int nextPoint, const QString &label = QString()) const;

private:
    MeasurementSpec m_spec;
};
