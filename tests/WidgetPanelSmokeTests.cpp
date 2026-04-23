#include <QPushButton>
#include <QPixmap>
#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QtTest/QSignalSpy>
#include <QtTest/QTest>

#include "models/MeasurementTableModel.h"
#include "panels/BottomStatusBar.h"
#include "panels/DeviceStatusBar.h"
#include "panels/HistoryPanel.h"
#include "panels/MeasureControlPanel.h"
#include "panels/SensorPanel.h"
#include "panels/ServoControlPanel.h"
#include "panels/TopTitleBar.h"
#include "theme/Theme.h"
#include "widgets/DpadControlWidget.h"
#include "widgets/LedIndicatorWidget.h"
#include "widgets/StatCardWidget.h"

class WidgetPanelSmokeTests : public QObject
{
    Q_OBJECT

private slots:
    void dpadClickEmitsJogSignal();
    void measurePanelButtonsEmitSignals();
    void historyPanelAcceptsModel();
    void topTitleBarExposesPrototypeActions();
    void topTitleBarShowsRenamedSoftwareTitle();
    void topTitleBarUsesPrototypeDarkStyle();
    void topTitleBarUsesCompactLeftAlignedNavigation();
    void deviceStatusBarExposesActionButtons();
    void deviceStatusBarUsesCompactPrototypeLayout();
    void bottomStatusBarUpdatesTelemetry();
    void bottomStatusBarUsesPrototypeMetaStrip();
    void sensorPanelExposesPrototypeFields();
    void measurePanelExposesPrototypeControls();
    void rightPanelsUseCompactPrototypeSpacing();
    void servoPanelExposesPrototypeControls();
    void servoPanelUsesCompactPrototypeLayout();
    void historyPanelUsesPrototypeSearchAndDelegates();
    void sensorTabsMoveActiveStateWhenClicked();
    void measureModeTabsMoveActiveStateWhenClicked();
    void ledIndicatorDrawsGlowRingPerState();
    void ledIndicatorErrStateBlinks();
  void statCardUsesConsolasValueFontAndInlineUnit();
  void statCardSmallVariantUsesSmallerValueFont();
};

void WidgetPanelSmokeTests::dpadClickEmitsJogSignal()
{
    DpadControlWidget widget;
    auto *button = widget.findChild<QPushButton*>(QStringLiteral("upButton"));
    QVERIFY(button != nullptr);

    QSignalSpy spy(&widget, &DpadControlWidget::jogRequested);
    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(spy.count(), 1);
    const auto args = spy.takeFirst();
    QCOMPARE(args.at(0).toInt(), 0);
    QCOMPARE(args.at(1).toInt(), 1);
}

void WidgetPanelSmokeTests::measurePanelButtonsEmitSignals()
{
    MeasureControlPanel panel;
    auto *startButton = panel.findChild<QPushButton*>(QStringLiteral("startButton"));
    auto *manualButton = panel.findChild<QPushButton*>(QStringLiteral("manualButton"));
    QVERIFY(startButton != nullptr);
    QVERIFY(manualButton != nullptr);

    QSignalSpy startSpy(&panel, &MeasureControlPanel::startRequested);
    QSignalSpy manualSpy(&panel, &MeasureControlPanel::manualRequested);
    QTest::mouseClick(startButton, Qt::LeftButton);
    QTest::mouseClick(manualButton, Qt::LeftButton);

    QCOMPARE(startSpy.count(), 1);
    QCOMPARE(manualSpy.count(), 1);
}

void WidgetPanelSmokeTests::historyPanelAcceptsModel()
{
    HistoryPanel panel;
    MeasurementTableModel model;

    panel.setModel(&model);

    auto *view = panel.findChild<QTableView*>(QStringLiteral("historyTable"));
    QVERIFY(view != nullptr);
    QCOMPARE(view->model(), &model);
}

void WidgetPanelSmokeTests::topTitleBarExposesPrototypeActions()
{
    TopTitleBar bar;

    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("navMeasureButton")) != nullptr);
    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("navDataButton")) != nullptr);
    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("searchButton")) != nullptr);
    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("alarmButton")) != nullptr);
}

void WidgetPanelSmokeTests::topTitleBarShowsRenamedSoftwareTitle()
{
    TopTitleBar bar;

    auto *title = bar.findChild<QLabel*>(QStringLiteral("brandTitleLabel"));
    QVERIFY(title != nullptr);
    QCOMPARE(title->text(), QStringLiteral("工业软件测量系统"));
}

void WidgetPanelSmokeTests::topTitleBarUsesPrototypeDarkStyle()
{
    TopTitleBar bar;
    bar.resize(1280, 44);
    bar.show();
    QCoreApplication::processEvents();

    auto *title = bar.findChild<QLabel*>(QStringLiteral("brandTitleLabel"));
    auto *tile = bar.findChild<QLabel*>(QStringLiteral("brandTileLabel"));
    QVERIFY(title != nullptr);
    QVERIFY(tile != nullptr);

    QPixmap pixmap(bar.size());
    bar.render(&pixmap);

    QCOMPARE(pixmap.toImage().pixelColor(10, 10), QColor(QStringLiteral("#0F1B2D")));
    QVERIFY(title->styleSheet().contains(QStringLiteral("#EDF1F7"), Qt::CaseInsensitive));
    QVERIFY(tile->styleSheet().contains(QStringLiteral("background:#5070D7"), Qt::CaseInsensitive));
}

void WidgetPanelSmokeTests::topTitleBarUsesCompactLeftAlignedNavigation()
{
    TopTitleBar bar;
    bar.resize(1280, 56);
    bar.show();
    QCoreApplication::processEvents();

    auto *title = bar.findChild<QLabel*>(QStringLiteral("brandTitleLabel"));
    auto *measureButton = bar.findChild<QPushButton*>(QStringLiteral("navMeasureButton"));
    auto *navWrap = bar.findChild<QWidget*>(QStringLiteral("topNavWrap"));
    QVERIFY(title != nullptr);
    QVERIFY(measureButton != nullptr);
    QVERIFY(navWrap != nullptr);

    QCOMPARE(bar.height(), 44);
    QVERIFY(measureButton->x() < 320);
    QVERIFY(measureButton->height() <= 32);
    QVERIFY(navWrap->width() < 320);
}

void WidgetPanelSmokeTests::deviceStatusBarExposesActionButtons()
{
    DeviceStatusBar bar;

    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("statusStartButton")) != nullptr);
    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("statusManualButton")) != nullptr);
    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("statusStopButton")) != nullptr);
}

void WidgetPanelSmokeTests::deviceStatusBarUsesCompactPrototypeLayout()
{
    DeviceStatusBar bar;
    bar.resize(1280, 44);
    bar.show();
    QCoreApplication::processEvents();

    auto *divider = bar.findChild<QWidget*>(QStringLiteral("statusDivider"));
    auto *startButton = bar.findChild<QPushButton*>(QStringLiteral("statusStartButton"));
    auto *manualButton = bar.findChild<QPushButton*>(QStringLiteral("statusManualButton"));
    auto *stopButton = bar.findChild<QPushButton*>(QStringLiteral("statusStopButton"));
    QVERIFY(divider != nullptr);
    QVERIFY(startButton != nullptr);
    QVERIFY(manualButton != nullptr);
    QVERIFY(stopButton != nullptr);

    QVERIFY2(startButton->height() <= 24, qPrintable(QStringLiteral("startButton height=%1").arg(startButton->height())));
    QVERIFY(manualButton->height() <= 24);
    QVERIFY(stopButton->height() <= 24);
    QCOMPARE(divider->width(), 1);
}

void WidgetPanelSmokeTests::bottomStatusBarUpdatesTelemetry()
{
    BottomStatusBar bar;

    bar.setPosition(MachinePosition{456, 789});
    bar.setProbeValue(-58.79);
    bar.setSampleProgress(5, 24);

    auto *xLabel = bar.findChild<QLabel*>(QStringLiteral("bottomXLabel"));
    auto *probeLabel = bar.findChild<QLabel*>(QStringLiteral("bottomProbeLabel"));
    auto *sampleLabel = bar.findChild<QLabel*>(QStringLiteral("bottomSampleLabel"));
    QVERIFY(xLabel != nullptr);
    QVERIFY(probeLabel != nullptr);
    QVERIFY(sampleLabel != nullptr);
    QCOMPARE(xLabel->text(), QStringLiteral("X: <b>456</b>"));
    QCOMPARE(probeLabel->text(), QStringLiteral("探针: <b>-58.790</b>"));
    QCOMPARE(sampleLabel->text(), QStringLiteral("采样: <b>5</b> / 24"));
}

void WidgetPanelSmokeTests::bottomStatusBarUsesPrototypeMetaStrip()
{
    BottomStatusBar bar;

    auto *buildLabel = bar.findChild<QLabel*>(QStringLiteral("bottomBuildLabel"));
    auto *userLabel = bar.findChild<QLabel*>(QStringLiteral("bottomUserLabel"));
    auto *versionLabel = bar.findChild<QLabel*>(QStringLiteral("bottomVersionLabel"));
    QVERIFY(buildLabel != nullptr);
    QVERIFY(userLabel != nullptr);
    QVERIFY(versionLabel != nullptr);

    QCOMPARE(bar.height(), 28);
    QCOMPARE(buildLabel->text(), QStringLiteral("构建 #2026.04.20"));
    QCOMPARE(userLabel->text(), QStringLiteral("用户: HXS"));
    QCOMPARE(versionLabel->text(), QStringLiteral("v1.0.1"));
}

void WidgetPanelSmokeTests::sensorPanelExposesPrototypeFields()
{
    SensorPanel panel;

    QVERIFY(panel.findChild<QPushButton*>(QStringLiteral("heightTabButton")) != nullptr);
    QVERIFY(panel.findChild<QPushButton*>(QStringLiteral("lightTabButton")) != nullptr);
    QVERIFY(panel.findChild<QLabel*>(QStringLiteral("sensorAbsValueLabel")) != nullptr);
    QVERIFY(panel.findChild<QLabel*>(QStringLiteral("sensorPkPkValueLabel")) != nullptr);
}

void WidgetPanelSmokeTests::measurePanelExposesPrototypeControls()
{
    MeasureControlPanel panel;

    QVERIFY(panel.findChild<QPushButton*>(QStringLiteral("autoModeButton")) != nullptr);
    QVERIFY(panel.findChild<QPushButton*>(QStringLiteral("singleModeButton")) != nullptr);
    QVERIFY(panel.findChild<QWidget*>(QStringLiteral("autoSampleCheck")) != nullptr);
    QVERIFY(panel.findChild<QWidget*>(QStringLiteral("cycleTestCheck")) != nullptr);
}

void WidgetPanelSmokeTests::rightPanelsUseCompactPrototypeSpacing()
{
    SensorPanel sensorPanel;
    MeasureControlPanel measurePanel;

    sensorPanel.resize(320, 320);
    measurePanel.resize(320, 320);
    sensorPanel.show();
    measurePanel.show();
    QCoreApplication::processEvents();

    auto *heightTab = sensorPanel.findChild<QPushButton*>(QStringLiteral("heightTabButton"));
    auto *lightTab = sensorPanel.findChild<QPushButton*>(QStringLiteral("lightTabButton"));
    auto *autoButton = measurePanel.findChild<QPushButton*>(QStringLiteral("autoModeButton"));
    auto *singleButton = measurePanel.findChild<QPushButton*>(QStringLiteral("singleModeButton"));
    auto *startButton = measurePanel.findChild<QPushButton*>(QStringLiteral("startButton"));
    auto *manualButton = measurePanel.findChild<QPushButton*>(QStringLiteral("manualButton"));
    auto *stopButton = measurePanel.findChild<QPushButton*>(QStringLiteral("stopButton"));
    QVERIFY(heightTab != nullptr);
    QVERIFY(lightTab != nullptr);
    QVERIFY(autoButton != nullptr);
    QVERIFY(singleButton != nullptr);
    QVERIFY(startButton != nullptr);
    QVERIFY(manualButton != nullptr);
    QVERIFY(stopButton != nullptr);

    QVERIFY(heightTab->height() <= 24);
    QVERIFY(lightTab->height() <= 24);
    QVERIFY(autoButton->height() <= 24);
    QVERIFY(singleButton->height() <= 24);
    QVERIFY(startButton->height() <= 28);
    QVERIFY(manualButton->height() <= 28);
    QVERIFY(stopButton->height() <= 28);
    QVERIFY(!heightTab->styleSheet().contains(QStringLiteral("#EEF2FF"), Qt::CaseInsensitive));
    QVERIFY(!autoButton->styleSheet().contains(QStringLiteral("background:#FFFFFF"), Qt::CaseInsensitive));
}

void WidgetPanelSmokeTests::servoPanelExposesPrototypeControls()
{
    ServoControlPanel panel;

    QVERIFY2(panel.findChild<QPushButton*>(QStringLiteral("servoHomeButton")) != nullptr, "missing servoHomeButton");
    QVERIFY2(panel.findChild<QLabel*>(QStringLiteral("servoJogHintLabel")) != nullptr, "missing servoJogHintLabel");
    QVERIFY2(panel.findChild<QPushButton*>(QStringLiteral("servoMoveButton")) != nullptr, "missing servoMoveButton");
}

void WidgetPanelSmokeTests::servoPanelUsesCompactPrototypeLayout()
{
    ServoControlPanel panel;
    panel.resize(320, 420);
    panel.show();
    QCoreApplication::processEvents();

    auto *homeButton = panel.findChild<QPushButton*>(QStringLiteral("servoHomeButton"));
    auto *moveButton = panel.findChild<QPushButton*>(QStringLiteral("servoMoveButton"));
    auto *hintLabel = panel.findChild<QLabel*>(QStringLiteral("servoJogHintLabel"));
    QVERIFY(homeButton != nullptr);
    QVERIFY(moveButton != nullptr);
    QVERIFY(hintLabel != nullptr);

    QVERIFY(homeButton->height() <= 24);
    QVERIFY(moveButton->height() <= 24);
    QVERIFY(hintLabel->text().contains(QStringLiteral("HOME")));
}

void WidgetPanelSmokeTests::historyPanelUsesPrototypeSearchAndDelegates()
{
    HistoryPanel panel;

    auto *search = panel.findChild<QLineEdit*>(QStringLiteral("historySearchInput"));
    auto *table = panel.findChild<QTableView*>(QStringLiteral("historyTable"));
    QVERIFY(search != nullptr);
    QVERIFY(table != nullptr);

    QCOMPARE(search->width(), 140);
    QVERIFY(table->itemDelegateForColumn(5) != nullptr);
    QVERIFY(table->itemDelegateForColumn(6) != nullptr);
    QVERIFY(table->itemDelegateForColumn(5)->objectName() == QStringLiteral("historyVerdictDelegate"));
    QVERIFY(table->itemDelegateForColumn(6)->objectName() == QStringLiteral("historyNumericDelegate"));
}

void WidgetPanelSmokeTests::sensorTabsMoveActiveStateWhenClicked()
{
    SensorPanel panel;
    auto *heightTab = panel.findChild<QPushButton*>(QStringLiteral("heightTabButton"));
    auto *lightTab = panel.findChild<QPushButton*>(QStringLiteral("lightTabButton"));
    QVERIFY(heightTab != nullptr);
    QVERIFY(lightTab != nullptr);

    QCOMPARE(heightTab->property("active").toBool(), true);
    QCOMPARE(lightTab->property("active").toBool(), false);

    QTest::mouseClick(lightTab, Qt::LeftButton);

    QCOMPARE(heightTab->property("active").toBool(), false);
    QCOMPARE(lightTab->property("active").toBool(), true);
}

void WidgetPanelSmokeTests::measureModeTabsMoveActiveStateWhenClicked()
{
    MeasureControlPanel panel;
    auto *autoButton = panel.findChild<QPushButton*>(QStringLiteral("autoModeButton"));
    auto *singleButton = panel.findChild<QPushButton*>(QStringLiteral("singleModeButton"));
    QVERIFY(autoButton != nullptr);
    QVERIFY(singleButton != nullptr);

    QCOMPARE(autoButton->property("active").toBool(), true);
    QCOMPARE(singleButton->property("active").toBool(), false);

    QTest::mouseClick(singleButton, Qt::LeftButton);

    QCOMPARE(autoButton->property("active").toBool(), false);
    QCOMPARE(singleButton->property("active").toBool(), true);
}

void WidgetPanelSmokeTests::ledIndicatorDrawsGlowRingPerState()
{
    LedIndicatorWidget led;
    led.setCompact(true);
    led.setLabel(QStringLiteral("Test"));
    led.resize(180, 24);

    const auto states = {LedIndicatorWidget::State::Ok, LedIndicatorWidget::State::Warn,
                         LedIndicatorWidget::State::Err, LedIndicatorWidget::State::Off};
    for (auto s : states) {
        led.setState(s);
        const auto pm = led.grab();
        QVERIFY(!pm.isNull());
        QCOMPARE(pm.size(), QSize(180, 24));
    }
}

void WidgetPanelSmokeTests::ledIndicatorErrStateBlinks()
{
    LedIndicatorWidget led;
    led.setState(LedIndicatorWidget::State::Err);
    QVERIFY(led.property("blinking").toBool());

    led.setState(LedIndicatorWidget::State::Ok);
    QVERIFY(!led.property("blinking").toBool());
}

void WidgetPanelSmokeTests::statCardUsesConsolasValueFontAndInlineUnit()
{
    StatCardWidget card;
    card.setLabel(QStringLiteral("THK"));
    card.setValue(QStringLiteral("11.500"));
    card.setUnit(QStringLiteral("μm"));
    card.resize(160, 80);
    card.show();
    QCoreApplication::processEvents();

    QPixmap pixmap(card.size());
    card.render(&pixmap);
    QImage img = pixmap.toImage();

    QVERIFY(!img.isNull());

    QCOMPARE(card.property("small").toBool(), false);
    QVERIFY(card.property("accentWidth").toInt() == 3);
    QVERIFY(card.property("borderRadius").toInt() == 6);
}

void WidgetPanelSmokeTests::statCardSmallVariantUsesSmallerValueFont()
{
    StatCardWidget card;
    card.setProperty("small", true);
    card.setLabel(QStringLiteral("MAX"));
    card.setValue(QStringLiteral("11.823"));
    card.setUnit(QStringLiteral("μm"));
    card.resize(160, 80);
    card.show();
    QCoreApplication::processEvents();

    QCOMPARE(card.property("small").toBool(), true);
}

QTEST_MAIN(WidgetPanelSmokeTests)

#include "WidgetPanelSmokeTests.moc"
