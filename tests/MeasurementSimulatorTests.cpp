#include <QtTest/QTest>

#include "domain/MeasurementSimulator.h"

class MeasurementSimulatorTests : public QObject
{
    Q_OBJECT

private slots:
    void seedHistoryCreatesTwentyFourRows();
    void generateProfileCoversExpectedRange();
};

void MeasurementSimulatorTests::seedHistoryCreatesTwentyFourRows()
{
    MeasurementSimulator simulator;

    const auto history = simulator.seedHistory();

    QCOMPARE(history.size(), 24);
    QCOMPARE(history.front().seq, 1010);
    QCOMPARE(history.back().seq, 987);
}

void MeasurementSimulatorTests::generateProfileCoversExpectedRange()
{
    MeasurementSimulator simulator;

    const auto profile = simulator.generateProfile(1.0);

    QVERIFY(!profile.empty());
    QCOMPARE(profile.front().x, 0.0);
    QCOMPARE(profile.back().x, 1280.0);
}

QTEST_APPLESS_MAIN(MeasurementSimulatorTests)

#include "MeasurementSimulatorTests.moc"
