#include <QtTest/QTest>

#include "domain/MeasurementSimulator.h"
#include "theme/Theme.h"
#include "widgets/CameraViewWidget.h"
#include "widgets/ProfileChartWidget.h"

class VisualizationWidgetTests : public QObject
{
    Q_OBJECT

private slots:
    void cameraViewTracksCrosshairPosition();
    void cameraViewStoresPrototypeHudTelemetry();
    void themeUsesStablePrototypeSemanticColors();
    void themeExposesStableHeaderAndCameraColors();
    void profileChartStoresProfileData();
    void profileChartHasAxisLabelMargins();
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

void VisualizationWidgetTests::themeUsesStablePrototypeSemanticColors()
{
    const auto &p = Theme::palette();

    QCOMPARE(p.brand.name(QColor::HexRgb), QStringLiteral("#3375e0"));
    QCOMPARE(p.brandStrong.name(QColor::HexRgb), QStringLiteral("#2c65c3"));
    QCOMPARE(p.brandWeak.name(QColor::HexRgb), QStringLiteral("#eef4ff"));
    QCOMPARE(p.ok.name(QColor::HexRgb), QStringLiteral("#357a4d"));
    QCOMPARE(p.warn.name(QColor::HexRgb), QStringLiteral("#e0a21a"));
    QCOMPARE(p.err.name(QColor::HexRgb), QStringLiteral("#c44a38"));
    QCOMPARE(p.accentTrace.name(QColor::HexRgb), QStringLiteral("#c44a38"));
    QCOMPARE(p.accentRef.name(QColor::HexRgb), QStringLiteral("#547aff"));
}

void VisualizationWidgetTests::themeExposesStableHeaderAndCameraColors()
{
    const auto &p = Theme::palette();

    QCOMPARE(p.headerText.name(QColor::HexRgb), QStringLiteral("#edf1f7"));
    QCOMPARE(p.headerTextMuted.name(QColor::HexRgb), QStringLiteral("#b8c1d0"));
    QCOMPARE(p.headerTextSubtle.name(QColor::HexRgb), QStringLiteral("#d3dae6"));
    QCOMPARE(p.brandAccent.name(QColor::HexRgb), QStringLiteral("#37b26c"));
    QCOMPARE(p.cameraBg.name(QColor::HexRgb), QStringLiteral("#1b1e25"));
    QCOMPARE(p.cameraHud.name(QColor::HexArgb), QStringLiteral("#b4060c16"));
    QCOMPARE(p.cameraStripe.name(QColor::HexArgb), QStringLiteral("#48242a34"));
    QCOMPARE(p.cameraStrip.name(QColor::HexRgb), QStringLiteral("#c3c7cf"));
    QCOMPARE(p.cameraPauseOverlay.name(QColor::HexArgb), QStringLiteral("#5c070a10"));
    QCOMPARE(p.gaugeHighlight.name(QColor::HexRgb), QStringLiteral("#6a8be6"));
    QCOMPARE(p.indicatorOutline.name(QColor::HexArgb), QStringLiteral("#19000000"));
    QCOMPARE(Theme::withAlpha(p.brand, 128).name(QColor::HexArgb), QStringLiteral("#803375e0"));
    QCOMPARE(Theme::withAlpha(p.ok, 0).name(QColor::HexArgb), QStringLiteral("#00357a4d"));
}

void VisualizationWidgetTests::profileChartStoresProfileData()
{
    MeasurementSimulator simulator;
    ProfileChartWidget widget;
    const auto profile = simulator.generateProfile(1.0);

    widget.setProfile(profile);

    QCOMPARE(widget.profile().size(), profile.size());
}

void VisualizationWidgetTests::profileChartHasAxisLabelMargins()
{
    ProfileChartWidget widget;
    widget.resize(560, 320);
    QVERIFY(widget.minimumSize().width() >= 420);
}

QTEST_MAIN(VisualizationWidgetTests)

#include "VisualizationWidgetTests.moc"
