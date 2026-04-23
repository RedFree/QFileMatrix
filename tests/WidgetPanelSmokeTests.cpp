#include <QPushButton>
#include <QTableView>
#include <QLabel>
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

class WidgetPanelSmokeTests : public QObject
{
    Q_OBJECT

private slots:
    void dpadClickEmitsJogSignal();
    void measurePanelButtonsEmitSignals();
    void historyPanelAcceptsModel();
    void topTitleBarExposesPrototypeActions();
    void topTitleBarShowsRenamedSoftwareTitle();
    void topTitleBarUsesReadableLightStyle();
    void topTitleBarUsesCompactLeftAlignedNavigation();
    void deviceStatusBarExposesActionButtons();
    void bottomStatusBarUpdatesTelemetry();
    void sensorPanelExposesPrototypeFields();
    void measurePanelExposesPrototypeControls();
    void servoPanelExposesPrototypeControls();
    void sensorTabsMoveActiveStateWhenClicked();
    void measureModeTabsMoveActiveStateWhenClicked();
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

void WidgetPanelSmokeTests::topTitleBarUsesReadableLightStyle()
{
    TopTitleBar bar;
    auto *title = bar.findChild<QLabel*>(QStringLiteral("brandTitleLabel"));
    auto *tile = bar.findChild<QLabel*>(QStringLiteral("brandTileLabel"));
    QVERIFY(title != nullptr);
    QVERIFY(tile != nullptr);

    QCOMPARE(bar.palette().color(QPalette::Window), QColor(QStringLiteral("#E7ECF4")));
    QVERIFY(title->styleSheet().contains(Theme::palette().text.name(), Qt::CaseInsensitive));
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
    QVERIFY(measureButton->height() <= 24);
    QVERIFY(navWrap->width() < 320);
}

void WidgetPanelSmokeTests::deviceStatusBarExposesActionButtons()
{
    DeviceStatusBar bar;

    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("statusStartButton")) != nullptr);
    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("statusManualButton")) != nullptr);
    QVERIFY(bar.findChild<QPushButton*>(QStringLiteral("statusStopButton")) != nullptr);
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
    QCOMPARE(xLabel->text(), QStringLiteral("X 456"));
    QCOMPARE(probeLabel->text(), QStringLiteral("探针 -58.790 μm"));
    QCOMPARE(sampleLabel->text(), QStringLiteral("采样 5 / 24"));
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

void WidgetPanelSmokeTests::servoPanelExposesPrototypeControls()
{
    ServoControlPanel panel;

    QVERIFY2(panel.findChild<QPushButton*>(QStringLiteral("servoHomeButton")) != nullptr, "missing servoHomeButton");
    QVERIFY2(panel.findChild<QLabel*>(QStringLiteral("servoJogHintLabel")) != nullptr, "missing servoJogHintLabel");
    QVERIFY2(panel.findChild<QPushButton*>(QStringLiteral("servoMoveButton")) != nullptr, "missing servoMoveButton");
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

QTEST_MAIN(WidgetPanelSmokeTests)

#include "WidgetPanelSmokeTests.moc"
