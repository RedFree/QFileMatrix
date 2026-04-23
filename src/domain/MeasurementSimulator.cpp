#include "domain/MeasurementSimulator.h"

#include <QDateTime>
#include <QtMath>

namespace {
QString pad2(int value)
{
    return QStringLiteral("%1").arg(value, 2, 10, QLatin1Char('0'));
}

QString formatTime(const QTime &time)
{
    return QStringLiteral("%1:%2:%3").arg(pad2(time.hour()), pad2(time.minute()), pad2(time.second()));
}
}

MeasurementSimulator::MeasurementSimulator() = default;

QVector<MeasurementRecord> MeasurementSimulator::seedHistory() const
{
    QVector<MeasurementRecord> rows;
    rows.reserve(24);

    const auto now = QDateTime::currentDateTime();

    for (int i = 0; i < 24; ++i) {
        const auto timestamp = now.addSecs(-i * 45);
        const auto thick = 11.5 + (qSin(i * 1.3) + qCos(i * 0.7)) * 0.9;
        const auto hmax = thick + 0.7;
        const auto hmin = thick - 0.9;
        const auto left = qSin(i * 0.4) * 1.5;
        const auto right = qCos(i * 0.5) * 1.5;

        MeasurementRecord row;
        row.seq = 1010 - i;
        row.batch = 3;
        row.point = 24 - i;
        row.group = 1;
        row.label = QStringLiteral("合格");
        row.thick = thick;
        row.hmin = hmin;
        row.hmax = hmax;
        row.left = left;
        row.right = right;
        row.station = 1;
        row.date = timestamp.date().toString(QStringLiteral("yyyy-MM-dd"));
        row.time = formatTime(timestamp.time());
        row.verdict = m_spec.verdict(thick);
        rows.push_back(row);
    }

    return rows;
}

ProfileData MeasurementSimulator::generateProfile(double seed) const
{
    ProfileData points;
    points.reserve(321);

    for (int x = 0; x <= 1280; x += 4) {
        double y = qSin(x * 0.031 + seed) * 0.4 + qCos(x * 0.07 + seed * 2.0) * 0.25;
        if (x > 300 && x < 1050) {
            const double t = (x - 300.0) / 750.0;
            y += 11.0 + qSin(t * M_PI) * 0.8 - qAbs(t - 0.5) * 2.2;
            y += qSin(x * 0.02 + seed) * 0.3;
        }
        points.push_back(ProfilePoint{static_cast<double>(x), y});
    }

    return points;
}

double MeasurementSimulator::randomThickness() const
{
    return 11.5;
}

MeasurementRecord MeasurementSimulator::createNextRecord(int seq, int nextPoint, const QString &label) const
{
    const auto now = QDateTime::currentDateTime();
    const auto thick = randomThickness();

    MeasurementRecord row;
    row.seq = seq;
    row.batch = 3;
    row.point = nextPoint;
    row.group = 1;
    row.label = label.isEmpty() ? QStringLiteral("自动") : label;
    row.thick = thick;
    row.hmin = thick - 0.7;
    row.hmax = thick + 0.6;
    row.left = 0.0;
    row.right = 0.0;
    row.station = 1;
    row.date = now.date().toString(QStringLiteral("yyyy-MM-dd"));
    row.time = formatTime(now.time());
    row.verdict = m_spec.verdict(thick);
    return row;
}
