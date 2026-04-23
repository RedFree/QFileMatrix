#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QComboBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QScrollArea>
#include <QtTest/QTest>

#include "app/MainWindow.h"
#include "panels/SensorPanel.h"
#include "theme/Theme.h"
#include "widgets/StatCardWidget.h"

class MainWindowTests : public QObject
{
    Q_OBJECT

private slots:
    void mainWindowBuildsPrimaryRegions();
    void manualSampleButtonPrependsHistoryRow();
    void selectingHistoryRowRefreshesThicknessCard();
    void currentGroupPanelShowsPrototypeFields();
    void currentGroupPanelUsesCompactStatCards();
    void currentGroupPanelUsesCompactVerdictPill();
    void currentGroupPanelUsesVerdictDrivenAccent();
    void cameraPanelShowsPrototypeHeaderControls();
    void profilePanelShowsPrototypeHeaderControls();
    void historyPanelShowsPrototypeToolbar();
    void leftRailShowsPrototypeNavigation();
    void leftRailActiveStateMovesWhenButtonClicked();
    void leftRailUsesPrototypeDenseButtons();
    void shellUsesCompactHeaderControlSizing();
    void historyTableUsesDenseRowSizing();
    void mainWindowUsesRenamedSoftwareTitle();
    void mainWindowStartsMaximized();
    void rightControlColumnUsesScrollContainer();
    void thicknessCardShowsTargetAndTrend();
    void rightColumnPanelsUseFlushStack();
};

void MainWindowTests::mainWindowBuildsPrimaryRegions()
{
    MainWindow window;

    QVERIFY(window.findChild<QWidget*>(QStringLiteral("cameraView")) != nullptr);
    QVERIFY(window.findChild<QWidget*>(QStringLiteral("profileChart")) != nullptr);
    QVERIFY(window.findChild<QTableView*>(QStringLiteral("historyTable")) != nullptr);
    QVERIFY(window.findChild<QWidget*>(QStringLiteral("measureControlPanel")) != nullptr);
}

void MainWindowTests::manualSampleButtonPrependsHistoryRow()
{
    MainWindow window;
    auto *table = window.findChild<QTableView*>(QStringLiteral("historyTable"));
    auto *manualButton = window.findChild<QPushButton*>(QStringLiteral("manualButton"));
    QVERIFY(table != nullptr);
    QVERIFY(manualButton != nullptr);

    const int initialRows = table->model()->rowCount();
    QTest::mouseClick(manualButton, Qt::LeftButton);

    QCOMPARE(table->model()->rowCount(), initialRows + 1);
}

void MainWindowTests::selectingHistoryRowRefreshesThicknessCard()
{
    MainWindow window;
    auto *table = window.findChild<QTableView*>(QStringLiteral("historyTable"));
    auto *card = window.findChild<StatCardWidget*>(QStringLiteral("thicknessCard"));
    QVERIFY(table != nullptr);
    QVERIFY(card != nullptr);
    QVERIFY(table->model()->rowCount() > 1);

    const auto initialValue = card->value();
    table->selectRow(1);
    QCoreApplication::processEvents();

    QVERIFY(card->value() != initialValue);
}

void MainWindowTests::currentGroupPanelShowsPrototypeFields()
{
    MainWindow window;

    QVERIFY(window.findChild<QLabel*>(QStringLiteral("currentGroupTitleLabel")) != nullptr);
    QVERIFY(window.findChild<QLabel*>(QStringLiteral("currentGroupVerdictLabel")) != nullptr);
    QVERIFY(window.findChild<StatCardWidget*>(QStringLiteral("leftHeightCard")) != nullptr);
    QVERIFY(window.findChild<StatCardWidget*>(QStringLiteral("rightHeightCard")) != nullptr);
}

void MainWindowTests::currentGroupPanelUsesCompactStatCards()
{
    MainWindow window;
    auto *thicknessCard = window.findChild<StatCardWidget*>(QStringLiteral("thicknessCard"));
    auto *maxCard = window.findChild<StatCardWidget*>(QStringLiteral("maxCard"));
    QVERIFY(thicknessCard != nullptr);
    QVERIFY(maxCard != nullptr);

    QVERIFY(thicknessCard->minimumHeight() <= 84);
    QVERIFY(maxCard->minimumHeight() <= 84);
}

void MainWindowTests::currentGroupPanelUsesCompactVerdictPill()
{
    MainWindow window;
    auto *verdict = window.findChild<QLabel*>(QStringLiteral("currentGroupVerdictLabel"));
    QVERIFY(verdict != nullptr);

    QVERIFY(verdict->styleSheet().contains(QStringLiteral("padding:1px 6px")));
    QVERIFY(verdict->styleSheet().contains(QStringLiteral("font-size:9.5px")));
}

void MainWindowTests::currentGroupPanelUsesVerdictDrivenAccent()
{
    MainWindow window;
    auto *table = window.findChild<QTableView*>(QStringLiteral("historyTable"));
    auto *thicknessCard = window.findChild<StatCardWidget*>(QStringLiteral("thicknessCard"));
    QVERIFY(table != nullptr);
    QVERIFY(thicknessCard != nullptr);
    QVERIFY(table->model()->rowCount() > 1);

    table->selectRow(1);
    QCoreApplication::processEvents();

    const QString verdict = table->model()->index(1, 5).data().toString();
    const QColor expected = verdict == QStringLiteral("ok")
                                ? Theme::palette().ok
                                : (verdict == QStringLiteral("warn") ? Theme::palette().warn : Theme::palette().err);
    QCOMPARE(thicknessCard->accentColor(), expected);
}

void MainWindowTests::cameraPanelShowsPrototypeHeaderControls()
{
    MainWindow window;

    QVERIFY(window.findChild<QLabel*>(QStringLiteral("cameraPanelTitleLabel")) != nullptr);
    QVERIFY(window.findChild<QLineEdit*>(QStringLiteral("cameraProgramInput")) != nullptr);
    QVERIFY(window.findChild<QLineEdit*>(QStringLiteral("cameraBatchInput")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("cameraPauseButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("cameraExpandButton")) != nullptr);
}

void MainWindowTests::profilePanelShowsPrototypeHeaderControls()
{
    MainWindow window;

    QVERIFY(window.findChild<QLabel*>(QStringLiteral("profilePanelTitleLabel")) != nullptr);
    QVERIFY(window.findChild<QComboBox*>(QStringLiteral("profileScaleCombo")) != nullptr);
    QVERIFY(window.findChild<QLabel*>(QStringLiteral("profileAxisHintLabel")) != nullptr);
}

void MainWindowTests::historyPanelShowsPrototypeToolbar()
{
    MainWindow window;

    QVERIFY(window.findChild<QLabel*>(QStringLiteral("historyTitleLabel")) != nullptr);
    QVERIFY(window.findChild<QLineEdit*>(QStringLiteral("historySearchInput")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("historyFilterButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("historyStatsButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("historyExportButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("historyDeleteButton")) != nullptr);
}

void MainWindowTests::leftRailShowsPrototypeNavigation()
{
    MainWindow window;

    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("railCameraButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("railCurveButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("railServoButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("railHistoryButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("railStatsButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("railGaugeButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("railSettingsButton")) != nullptr);
    QVERIFY(window.findChild<QPushButton*>(QStringLiteral("railUserButton")) != nullptr);
}

void MainWindowTests::leftRailActiveStateMovesWhenButtonClicked()
{
    MainWindow window;
    auto *cameraButton = window.findChild<QPushButton*>(QStringLiteral("railCameraButton"));
    auto *curveButton = window.findChild<QPushButton*>(QStringLiteral("railCurveButton"));
    QVERIFY(cameraButton != nullptr);
    QVERIFY(curveButton != nullptr);

    QCOMPARE(cameraButton->property("active").toBool(), true);
    QCOMPARE(curveButton->property("active").toBool(), false);

    QTest::mouseClick(curveButton, Qt::LeftButton);

    QCOMPARE(cameraButton->property("active").toBool(), false);
    QCOMPARE(curveButton->property("active").toBool(), true);
}

void MainWindowTests::leftRailUsesPrototypeDenseButtons()
{
    MainWindow window;
    auto *cameraButton = window.findChild<QPushButton*>(QStringLiteral("railCameraButton"));
    auto *curveButton = window.findChild<QPushButton*>(QStringLiteral("railCurveButton"));
    auto *settingsButton = window.findChild<QPushButton*>(QStringLiteral("railSettingsButton"));
    QVERIFY(cameraButton != nullptr);
    QVERIFY(curveButton != nullptr);
    QVERIFY(settingsButton != nullptr);

    QCOMPARE(cameraButton->size(), QSize(40, 40));
    QCOMPARE(curveButton->size(), QSize(40, 40));
    QCOMPARE(settingsButton->size(), QSize(40, 40));
    QVERIFY(cameraButton->text().isEmpty());
    QVERIFY(curveButton->text().isEmpty());
    QCOMPARE(cameraButton->property("railIcon").toString(), QStringLiteral("camera"));
    QCOMPARE(curveButton->property("railIcon").toString(), QStringLiteral("curve"));
}

void MainWindowTests::shellUsesCompactHeaderControlSizing()
{
    MainWindow window;
    auto *railButton = window.findChild<QPushButton*>(QStringLiteral("railCameraButton"));
    auto *programInput = window.findChild<QLineEdit*>(QStringLiteral("cameraProgramInput"));
    auto *batchInput = window.findChild<QLineEdit*>(QStringLiteral("cameraBatchInput"));
    auto *pauseButton = window.findChild<QPushButton*>(QStringLiteral("cameraPauseButton"));
    auto *scaleCombo = window.findChild<QComboBox*>(QStringLiteral("profileScaleCombo"));
    auto *searchInput = window.findChild<QLineEdit*>(QStringLiteral("historySearchInput"));
    auto *filterButton = window.findChild<QPushButton*>(QStringLiteral("historyFilterButton"));
    QVERIFY(railButton != nullptr);
    QVERIFY(programInput != nullptr);
    QVERIFY(batchInput != nullptr);
    QVERIFY(pauseButton != nullptr);
    QVERIFY(scaleCombo != nullptr);
    QVERIFY(searchInput != nullptr);
    QVERIFY(filterButton != nullptr);

    QCOMPARE(railButton->size(), QSize(40, 40));
    QCOMPARE(programInput->height(), 24);
    QCOMPARE(batchInput->width(), 30);
    QCOMPARE(pauseButton->height(), 24);
    QCOMPARE(scaleCombo->height(), 24);
    QCOMPARE(searchInput->height(), 24);
    QCOMPARE(filterButton->height(), 24);
}

void MainWindowTests::historyTableUsesDenseRowSizing()
{
    MainWindow window;
    auto *table = window.findChild<QTableView*>(QStringLiteral("historyTable"));
    QVERIFY(table != nullptr);

    QCOMPARE(table->verticalHeader()->defaultSectionSize(), 28);
}

void MainWindowTests::mainWindowUsesRenamedSoftwareTitle()
{
    MainWindow window;

    QCOMPARE(window.windowTitle(), QStringLiteral("工业软件测量系统"));
}

void MainWindowTests::mainWindowStartsMaximized()
{
    MainWindow window;
    window.show();
    QCoreApplication::processEvents();

    QVERIFY(window.isMaximized());
}

void MainWindowTests::rightControlColumnUsesScrollContainer()
{
    MainWindow window;

    auto *scrollArea = window.findChild<QScrollArea*>(QStringLiteral("rightControlScrollArea"));
    QVERIFY(scrollArea != nullptr);
    QCOMPARE(scrollArea->widgetResizable(), true);
    QCOMPARE(scrollArea->horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
}

void MainWindowTests::thicknessCardShowsTargetAndTrend()
{
    MainWindow window;
    auto *thicknessCard = window.findChild<StatCardWidget*>(QStringLiteral("thicknessCard"));
    QVERIFY(thicknessCard != nullptr);

    QCOMPARE(thicknessCard->target(), QStringLiteral("11.500"));
    QVERIFY(thicknessCard->metaText().isEmpty());
}

void MainWindowTests::rightColumnPanelsUseFlushStack()
{
    MainWindow window;
    auto *sensorPanel = window.findChild<SensorPanel*>(QStringLiteral("sensorPanel"));
    auto *servoPanel = window.findChild<QWidget*>(QStringLiteral("servoPanel"));
    QVERIFY(sensorPanel != nullptr);
    QVERIFY(servoPanel != nullptr);
    QVERIFY(!sensorPanel->styleSheet().contains(QStringLiteral("border-radius:10px")));
    QVERIFY(servoPanel->styleSheet().contains(QStringLiteral("border-bottom:1px solid")));
}

QTEST_MAIN(MainWindowTests)

#include "MainWindowTests.moc"
