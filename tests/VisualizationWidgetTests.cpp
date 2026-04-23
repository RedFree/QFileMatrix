#include <QtTest/QTest>

#include "domain/MeasurementSimulator.h"
#include "widgets/CameraViewWidget.h"
#include "widgets/ProfileChartWidget.h"

class VisualizationWidgetTests : public QObject
{
    Q_OBJECT

private slots:
    void cameraViewTracksCrosshairPosition();
    void cameraViewStoresPrototypeHudTelemetry();
    void profileChartStoresProfileData();
};

void VisualizationWidgetTests::cameraViewTracksCrosshairPosition()
{
    CameraViewWidget widget;

    widget.setCrosshairNormalizedPosition(QPointF(0.25, 0.75));

    QCOMPARE(widget.crosshairNormalizedPosition(), QPointF(0.25, 0.75));
}

void VisualizationWidgetTests::cameraViewStoresPrototypeHudTelemetry()
{
    CameraViewWidget widget;

    widget.setStation(3);
    widget.setFrameRate(22.0);
    widget.setExposureMs(3.2);
    widget.setGain(1.4);

    QCOMPARE(widget.station(), 3);
    QCOMPARE(widget.frameRate(), 22.0);
    QCOMPARE(widget.exposureMs(), 3.2);
    QCOMPARE(widget.gain(), 1.4);
}

void VisualizationWidgetTests::profileChartStoresProfileData()
{
    MeasurementSimulator simulator;
    ProfileChartWidget widget;
    const auto profile = simulator.generateProfile(1.0);

    widget.setProfile(profile);

    QCOMPARE(widget.profile().size(), profile.size());
}

QTEST_MAIN(VisualizationWidgetTests)

#include "VisualizationWidgetTests.moc"
