#include <QtTest/QTest>

#include "domain/MeasurementSpec.h"

class MeasurementSpecTests : public QObject
{
    Q_OBJECT

private slots:
    void verdictMarksOutOfRangeAsErr();
    void verdictMarksEdgeBandAsWarn();
    void verdictMarksCenterBandAsOk();
};

void MeasurementSpecTests::verdictMarksOutOfRangeAsErr()
{
    const MeasurementSpec spec;

    QCOMPARE(spec.verdict(9.9), QStringLiteral("err"));
    QCOMPARE(spec.verdict(13.1), QStringLiteral("err"));
}

void MeasurementSpecTests::verdictMarksEdgeBandAsWarn()
{
    const MeasurementSpec spec;

    QCOMPARE(spec.verdict(10.2), QStringLiteral("warn"));
    QCOMPARE(spec.verdict(12.8), QStringLiteral("warn"));
}

void MeasurementSpecTests::verdictMarksCenterBandAsOk()
{
    const MeasurementSpec spec;

    QCOMPARE(spec.verdict(11.5), QStringLiteral("ok"));
}

QTEST_APPLESS_MAIN(MeasurementSpecTests)

#include "MeasurementSpecTests.moc"
