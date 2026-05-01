#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QComboBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QScrollArea>
#include <QtTest/QTest>
#include <QSignalSpy>

#include "app/MainWindow.h"
#include "panels/SensorPanel.h"
#include "theme/Theme.h"
#include "widgets/StatCardWidget.h"
#include "dialogs/ManualSampleDialog.h"
#include "dialogs/AlarmCenterDialog.h"

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
    void leftRailButtonsHaveToolTipsAndPointerCursor();
    void shellUsesCompactHeaderControlSizing();
    void shellUsesPaletteDrivenControlStyles();
    void shellUsesPrototypeOuterFrame();
    void historyTableUsesDenseRowSizing();
    void mainWindowUsesRenamedSoftwareTitle();
    void mainWindowStartsMaximized();
    void rightControlColumnUsesScrollContainer();
    void thicknessCardShowsTargetAndTrend();
    void rightColumnPanelsUseFlushStack();
    void manualSampleDialogShowsPositionAndConfirm();
    void manualSampleDialogUsesThemeStyles();
    void alarmCenterDialogShowsAlarmEntries();
    void alarmCenterDialogUsesThemeStyles();
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
    QVERIFY(window.findChild<QWidget*>(QStringLiteral("currentGroupVerdictLabel")) != nullptr);
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
    auto *verdict = window.findChild<QWidget*>(QStringLiteral("currentGroupVerdictLabel"));
    QVERIFY(verdict != nullptr);

    QVERIFY(verdict->height() <= 22);
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

void MainWindowTests::shellUsesPaletteDrivenControlStyles()
{
    MainWindow window;
    auto *programInput = window.findChild<QLineEdit*>(QStringLiteral("cameraProgramInput"));
    auto *batchInput = window.findChild<QLineEdit*>(QStringLiteral("cameraBatchInput"));
    auto *pauseButton = window.findChild<QPushButton*>(QStringLiteral("cameraPauseButton"));
    auto *scaleCombo = window.findChild<QComboBox*>(QStringLiteral("profileScaleCombo"));
    auto *rightColumn = window.findChild<QWidget*>(QStringLiteral("rightControlColumn"));
    QVERIFY(programInput != nullptr);
    QVERIFY(batchInput != nullptr);
    QVERIFY(pauseButton != nullptr);
    QVERIFY(scaleCombo != nullptr);
    QVERIFY(rightColumn != nullptr);

    QVERIFY(programInput->styleSheet().contains(Theme::palette().borderStrong.name(), Qt::CaseInsensitive));
    QVERIFY(batchInput->styleSheet().contains(Theme::palette().text1.name(), Qt::CaseInsensitive));
    QVERIFY(scaleCombo->styleSheet().contains(Theme::palette().borderStrong.name(), Qt::CaseInsensitive));
    QVERIFY(scaleCombo->styleSheet().contains(Theme::palette().bgPanel.name(), Qt::CaseInsensitive));
    QVERIFY(pauseButton->styleSheet().contains(Theme::palette().text1.name(), Qt::CaseInsensitive));
    QVERIFY(rightColumn->styleSheet().contains(Theme::palette().bgRail.name(), Qt::CaseInsensitive));
}

void MainWindowTests::shellUsesPrototypeOuterFrame()
{
    MainWindow window;
    auto *shell = window.findChild<QFrame*>(QStringLiteral("mainShellFrame"));
    QVERIFY(shell != nullptr);

    QVERIFY(shell->styleSheet().contains(QStringLiteral("border-radius:16px"), Qt::CaseInsensitive));
    QVERIFY(shell->styleSheet().contains(Theme::palette().borderStrong.name(), Qt::CaseInsensitive));
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

void MainWindowTests::leftRailButtonsHaveToolTipsAndPointerCursor()
{
    MainWindow window;
    const auto buttons = window.findChildren<QPushButton*>(QStringLiteral("railCameraButton"));
    QVERIFY(!buttons.isEmpty());
    auto *btn = buttons.first();
    QVERIFY(!btn->toolTip().isEmpty());
    QCOMPARE(btn->cursor().shape(), Qt::PointingHandCursor);
}

void MainWindowTests::manualSampleDialogShowsPositionAndConfirm()
{
    ManualSampleDialog dialog;
    dialog.setPosition(QStringLiteral("12.345"), QStringLiteral("67.890"));
    dialog.setProgram(QStringLiteral("1010"));
    dialog.setBatch(QStringLiteral("3"));

    auto *confirmButton = dialog.findChild<QPushButton*>(QStringLiteral("manualSampleConfirmButton"));
    QVERIFY(confirmButton != nullptr);
    QCOMPARE(confirmButton->text(), QStringLiteral("确认采样"));

    auto *cancelButton = dialog.findChild<QPushButton*>(QStringLiteral("manualSampleCancelButton"));
    QVERIFY(cancelButton != nullptr);

    QVERIFY(dialog.program() == QStringLiteral("1010"));
    QVERIFY(dialog.batch() == QStringLiteral("3"));
}

void MainWindowTests::manualSampleDialogUsesThemeStyles()
{
    ManualSampleDialog dialog;

    auto *closeButton = dialog.findChild<QPushButton*>(QStringLiteral("manualSampleCloseButton"));
    auto *confirmButton = dialog.findChild<QPushButton*>(QStringLiteral("manualSampleConfirmButton"));
    auto *cancelButton = dialog.findChild<QPushButton*>(QStringLiteral("manualSampleCancelButton"));
    QVERIFY(closeButton != nullptr);
    QVERIFY(confirmButton != nullptr);
    QVERIFY(cancelButton != nullptr);

    QVERIFY(closeButton->styleSheet().contains(Theme::palette().headerTextMuted.name(), Qt::CaseInsensitive));
    QVERIFY(closeButton->styleSheet().contains(Theme::palette().headerText.name(), Qt::CaseInsensitive));
    QVERIFY(dialog.styleSheet().contains(Theme::palette().borderStrong.name(), Qt::CaseInsensitive));
    QVERIFY(dialog.styleSheet().contains(Theme::palette().brandStrong.name(), Qt::CaseInsensitive));
    QCOMPARE(confirmButton->property("role").toString(), QStringLiteral("primary"));
    QVERIFY(cancelButton->styleSheet().isEmpty());
}

void MainWindowTests::alarmCenterDialogShowsAlarmEntries()
{
    AlarmCenterDialog dialog;
    QCOMPARE(dialog.alarms().size(), 3);
    QCOMPARE(dialog.alarms()[0].type, QStringLiteral("warn"));
    QCOMPARE(dialog.alarms()[1].type, QStringLiteral("err"));
}

void MainWindowTests::alarmCenterDialogUsesThemeStyles()
{
    AlarmCenterDialog dialog;

    auto *closeButton = dialog.findChild<QPushButton*>(QStringLiteral("alarmCenterCloseButton"));
    auto *scrollArea = dialog.findChild<QScrollArea*>();
    QVERIFY(closeButton != nullptr);
    QVERIFY(scrollArea != nullptr);

    QVERIFY(closeButton->styleSheet().contains(Theme::palette().headerTextMuted.name(), Qt::CaseInsensitive));
    QVERIFY(closeButton->styleSheet().contains(Theme::palette().headerText.name(), Qt::CaseInsensitive));
    QVERIFY(dialog.styleSheet().contains(Theme::palette().borderStrong.name(), Qt::CaseInsensitive));
    QVERIFY(dialog.styleSheet().contains(Theme::palette().bgPanel.name(), Qt::CaseInsensitive));
    QVERIFY(scrollArea->styleSheet().contains(Theme::palette().bgPanel.name(), Qt::CaseInsensitive));
}

QTEST_MAIN(MainWindowTests)

#include "MainWindowTests.moc"
