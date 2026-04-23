#include <QtTest/QSignalSpy>
#include <QtTest/QTest>

#include "app/AppController.h"

class AppControllerTests : public QObject
{
    Q_OBJECT

private slots:
    void startsMeasurementFromIdle();
    void completesMeasurementAndPrependsRecord();
    void manualSamplePrependsRecord();
    void jogAndHomeUpdatePosition();
};

void AppControllerTests::startsMeasurementFromIdle()
{
    AppController controller;

    controller.startMeasurement();

    QCOMPARE(controller.state().measuring, true);
    QCOMPARE(controller.state().progress, 0.0);
}

void AppControllerTests::completesMeasurementAndPrependsRecord()
{
    AppController controller;
    const auto initialRows = controller.tableModel()->rowCount();
    const auto initialTopSeq = controller.tableModel()->recordAt(0).seq;

    controller.startMeasurement();
    while (controller.state().measuring) {
        controller.tickMeasurement();
    }

    QCOMPARE(controller.tableModel()->rowCount(), initialRows + 1);
    QCOMPARE(controller.tableModel()->recordAt(0).seq, initialTopSeq + 1);
    QCOMPARE(controller.state().progress, 0.0);
}

void AppControllerTests::manualSamplePrependsRecord()
{
    AppController controller;
    const auto initialRows = controller.tableModel()->rowCount();

    controller.manualSample();

    QCOMPARE(controller.tableModel()->rowCount(), initialRows + 1);
    QCOMPARE(controller.tableModel()->recordAt(0).label, QStringLiteral("手动"));
}

void AppControllerTests::jogAndHomeUpdatePosition()
{
    AppController controller;

    controller.jog(100, -200);
    QCOMPARE(controller.state().pos.x, 23989);
    QCOMPARE(controller.state().pos.y, 26159);

    controller.goHome();
    QCOMPARE(controller.state().pos.x, 10000);
    QCOMPARE(controller.state().pos.y, 10000);
}

QTEST_APPLESS_MAIN(AppControllerTests)

#include "AppControllerTests.moc"
