#include "app/MainWindow.h"

#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QVBoxLayout>

#include "app/AppController.h"
#include "panels/BottomStatusBar.h"
#include "panels/DeviceStatusBar.h"
#include "panels/HistoryPanel.h"
#include "panels/MeasureControlPanel.h"
#include "panels/SensorPanel.h"
#include "panels/ServoControlPanel.h"
#include "panels/TopTitleBar.h"
#include "theme/Theme.h"
#include "widgets/CameraViewWidget.h"
#include "widgets/ProfileChartWidget.h"
#include "widgets/StatCardWidget.h"

namespace {
QFrame *makePanel(const QString &title, QWidget *content)
{
    auto *frame = new QFrame;
    frame->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(10, 10, 10, 10);
    auto *label = new QLabel(title);
    label->setStyleSheet(Theme::titleStyle());
    layout->addWidget(label);
    layout->addWidget(content, 1);
    return frame;
}

QFrame *makeBodyPanel(QWidget *content)
{
    auto *frame = new QFrame;
    frame->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(content, 1);
    return frame;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_controller(new AppController(this))
{
    buildUi();
    wireUi();
    updateFromController();
    resize(1520, 940);
    setWindowTitle(QStringLiteral("工业软件测量系统"));
    setWindowState(windowState() | Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    if (m_historyPanel != nullptr) {
        m_historyPanel->disconnect(this);
    }
    if (m_controller != nullptr) {
        m_controller->disconnect(this);
    }
}

void MainWindow::buildUi()
{
    auto *central = new QWidget;
    central->setStyleSheet(QStringLiteral("background:%1;").arg(Theme::palette().bgApp.name()));
    auto *root = new QVBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    m_topTitleBar = new TopTitleBar;
    m_deviceStatusBar = new DeviceStatusBar;
    m_bottomStatusBar = new BottomStatusBar;
    root->addWidget(m_topTitleBar);
    root->addWidget(m_deviceStatusBar);

    auto *bodySplitter = new QSplitter(Qt::Horizontal);
    bodySplitter->setChildrenCollapsible(false);
    bodySplitter->addWidget(createLeftRail());

    auto *centerSplitter = new QSplitter(Qt::Vertical);
    centerSplitter->setChildrenCollapsible(false);

    auto *topSplitter = new QSplitter(Qt::Horizontal);
    topSplitter->setChildrenCollapsible(false);
    topSplitter->addWidget(createCameraPanel());

    auto *rightTop = new QWidget;
    auto *rightTopLayout = new QVBoxLayout(rightTop);
    rightTopLayout->setContentsMargins(0, 0, 0, 0);
    rightTopLayout->setSpacing(8);
    rightTopLayout->addWidget(createStatsPanel());
    rightTopLayout->addWidget(createProfilePanel(), 1);
    topSplitter->addWidget(rightTop);
    topSplitter->setStretchFactor(0, 3);
    topSplitter->setStretchFactor(1, 2);

    centerSplitter->addWidget(topSplitter);
    m_historyPanel = new HistoryPanel;
    centerSplitter->addWidget(makeBodyPanel(m_historyPanel));
    centerSplitter->setStretchFactor(0, 3);
    centerSplitter->setStretchFactor(1, 2);

    bodySplitter->addWidget(centerSplitter);

    auto *rightPanel = new QWidget;
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(8, 8, 8, 8);
    rightLayout->setSpacing(8);
    m_sensorPanel = new SensorPanel;
    m_measurePanel = new MeasureControlPanel;
    m_measurePanel->setObjectName(QStringLiteral("measureControlPanel"));
    m_servoPanel = new ServoControlPanel;
    auto *sensorWrap = makeBodyPanel(m_sensorPanel);
    auto *measureWrap = makeBodyPanel(m_measurePanel);
    auto *servoWrap = makeBodyPanel(m_servoPanel);
    sensorWrap->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    measureWrap->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    servoWrap->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    rightLayout->addWidget(sensorWrap);
    rightLayout->addWidget(measureWrap);
    rightLayout->addWidget(servoWrap);
    rightLayout->addStretch();

    auto *rightScrollArea = new QScrollArea;
    rightScrollArea->setObjectName(QStringLiteral("rightControlScrollArea"));
    rightScrollArea->setFrameShape(QFrame::NoFrame);
    rightScrollArea->setWidgetResizable(true);
    rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rightScrollArea->setWidget(rightPanel);
    bodySplitter->addWidget(rightScrollArea);

    bodySplitter->setStretchFactor(0, 0);
    bodySplitter->setStretchFactor(1, 1);
    bodySplitter->setStretchFactor(2, 0);
    bodySplitter->setSizes({52, 1080, 320});

    root->addWidget(bodySplitter, 1);
    root->addWidget(m_bottomStatusBar);

    setCentralWidget(central);
}

void MainWindow::wireUi()
{
    m_historyPanel->setModel(m_controller->tableModel());

    connect(m_measurePanel, &MeasureControlPanel::startRequested, m_controller, &AppController::startMeasurement);
    connect(m_measurePanel, &MeasureControlPanel::stopRequested, m_controller, &AppController::stopMeasurement);
    connect(m_measurePanel, &MeasureControlPanel::manualRequested, m_controller, &AppController::manualSample);
    connect(m_measurePanel, &MeasureControlPanel::sampleCountChanged, m_controller, &AppController::setSampleCount);

    connect(m_deviceStatusBar, &DeviceStatusBar::startRequested, m_controller, &AppController::startMeasurement);
    connect(m_deviceStatusBar, &DeviceStatusBar::manualRequested, m_controller, &AppController::manualSample);
    connect(m_deviceStatusBar, &DeviceStatusBar::stopRequested, m_controller, &AppController::stopMeasurement);

    connect(m_servoPanel, &ServoControlPanel::jogRequested, this, [this](int dx, int dy) {
        const auto step = m_controller->state().step;
        m_controller->jog(dx * step, dy * step);
    });
    connect(m_servoPanel, &ServoControlPanel::homeRequested, m_controller, &AppController::goHome);
    connect(m_servoPanel, &ServoControlPanel::gotoRequested, this, [this](int x, int y) {
        const auto pos = m_controller->state().pos;
        m_controller->jog(x - pos.x, y - pos.y);
    });
    connect(m_servoPanel, &ServoControlPanel::stepChanged, m_controller, &AppController::setStep);

    connect(m_controller, &AppController::stateChanged, this, [this](const AppState &) { updateFromController(); });
    connect(m_controller, &AppController::latestRecordChanged, this, [this](const MeasurementRecord &) { updateFromController(); });
    connect(m_controller, &AppController::profileChanged, this, [this](const ProfileData &profile) { m_profileChart->setProfile(profile); });
    connect(m_historyPanel, &HistoryPanel::currentRowChanged, this, [this](int row) {
        if (row < 0) {
            return;
        }
        m_selectedHistoryRow = row;
        updateFromController();
    });
}

void MainWindow::updateFromController()
{
    const auto &state = m_controller->state();
    const auto rowCount = m_controller->tableModel()->rowCount();
    if (rowCount > 0) {
        m_selectedHistoryRow = qBound(0, m_selectedHistoryRow, rowCount - 1);
    } else {
        m_selectedHistoryRow = 0;
    }
    const auto record = rowCount > 0 ? m_controller->tableModel()->recordAt(m_selectedHistoryRow) : m_controller->latestRecord();

    m_deviceStatusBar->setProgress(state.progress);
    m_deviceStatusBar->setMeasuring(state.measuring);
    m_measurePanel->setProgress(state.progress);
    m_measurePanel->setMeasuring(state.measuring);
    m_measurePanel->setSampleCount(state.sampleCount);
    m_bottomStatusBar->setPosition(state.pos);
    m_bottomStatusBar->setProbeValue(record.left);
    m_bottomStatusBar->setSampleProgress(record.point, 24);
    m_bottomStatusBar->setFrameRate(22.0);
    m_bottomStatusBar->setConnected(true);
    m_bottomStatusBar->setMeasuring(state.measuring);
    m_servoPanel->setPosition(state.pos);
    m_servoPanel->setStep(state.step);
    m_cameraView->setPaused(state.paused);
    m_cameraView->setMeasuring(state.measuring);
    m_profileChart->setMeasuring(state.measuring);
    m_profileChart->setProfile(m_controller->currentProfile());
    if (auto *pauseButton = findChild<QPushButton*>(QStringLiteral("cameraPauseButton"))) {
        pauseButton->setText(state.paused ? QStringLiteral("继续") : QStringLiteral("暂停"));
    }
    applyRecordToSummary(record);
}

QWidget *MainWindow::createLeftRail()
{
    auto *rail = new QWidget;
    rail->setFixedWidth(52);
    rail->setStyleSheet(QStringLiteral("background:%1;border-right:1px solid %2;")
                            .arg(Theme::palette().bgRail.name(), Theme::palette().border.name()));
    auto *layout = new QVBoxLayout(rail);
    layout->setContentsMargins(6, 8, 6, 8);
    layout->setSpacing(6);

    const struct {
        const char *name;
        const char *label;
        const char *tooltip;
        bool active;
    } navButtons[] = {
        {"railCameraButton", "CAM", "相机", true},
        {"railCurveButton", "CRV", "曲线", false},
        {"railServoButton", "XY", "伺服", false},
        {"railHistoryButton", "REC", "记录", false},
        {"railStatsButton", "STA", "统计", false},
        {"railGaugeButton", "GAG", "仪表", false},
    };

    QList<QPushButton*> navButtonWidgets;
    for (const auto &spec : navButtons) {
        auto *button = new QPushButton(QString::fromLatin1(spec.label));
        button->setObjectName(QString::fromLatin1(spec.name));
        button->setToolTip(QString::fromUtf8(spec.tooltip));
        button->setFixedSize(36, 36);
        button->setCursor(Qt::PointingHandCursor);
        button->setProperty("active", spec.active);
        button->setStyleSheet(Theme::railButtonStyle(spec.active));
        layout->addWidget(button);
        navButtonWidgets.append(button);
    }

    for (auto *button : navButtonWidgets) {
        connect(button, &QPushButton::clicked, rail, [button, navButtonWidgets] {
            for (auto *candidate : navButtonWidgets) {
                const bool active = candidate == button;
                candidate->setProperty("active", active);
                candidate->setStyleSheet(Theme::railButtonStyle(active));
            }
        });
    }

    layout->addStretch();

    auto *settingsButton = new QPushButton(QStringLiteral("CFG"));
    settingsButton->setObjectName(QStringLiteral("railSettingsButton"));
    settingsButton->setToolTip(QStringLiteral("设置"));
    settingsButton->setFixedSize(36, 36);
    settingsButton->setCursor(Qt::PointingHandCursor);
    settingsButton->setStyleSheet(Theme::railButtonStyle(false));
    layout->addWidget(settingsButton);

    auto *userButton = new QPushButton(QStringLiteral("USR"));
    userButton->setObjectName(QStringLiteral("railUserButton"));
    userButton->setToolTip(QStringLiteral("用户"));
    userButton->setFixedSize(36, 36);
    userButton->setCursor(Qt::PointingHandCursor);
    userButton->setStyleSheet(Theme::railButtonStyle(false));
    layout->addWidget(userButton);

    return rail;
}

QWidget *MainWindow::createCameraPanel()
{
    auto *frame = new QFrame;
    frame->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    auto *header = new QHBoxLayout;
    header->setContentsMargins(0, 0, 0, 0);
    header->setSpacing(8);
    auto *title = new QLabel(QStringLiteral("实时图像 · CAM-01"));
    title->setObjectName(QStringLiteral("cameraPanelTitleLabel"));
    title->setStyleSheet(QStringLiteral("font-size:12px;font-weight:600;color:%1;").arg(Theme::palette().text.name()));
    header->addWidget(title);
    header->addStretch();

    auto *programLabel = new QLabel(QStringLiteral("程序号"));
    auto *programInput = new QLineEdit(QStringLiteral("1010"));
    programInput->setObjectName(QStringLiteral("cameraProgramInput"));
    programInput->setFixedWidth(54);
    programInput->setFixedHeight(26);
    auto *batchLabel = new QLabel(QStringLiteral("批号"));
    auto *batchInput = new QLineEdit(QStringLiteral("3"));
    batchInput->setObjectName(QStringLiteral("cameraBatchInput"));
    batchInput->setFixedWidth(40);
    batchInput->setFixedHeight(26);
    auto *pauseButton = new QPushButton(QStringLiteral("暂停"));
    pauseButton->setObjectName(QStringLiteral("cameraPauseButton"));
    pauseButton->setFixedHeight(26);
    auto *expandButton = new QPushButton(QStringLiteral("展开"));
    expandButton->setObjectName(QStringLiteral("cameraExpandButton"));
    expandButton->setFixedHeight(26);
    const auto fieldStyle = QStringLiteral("QLineEdit{background:%1;border:1px solid %2;border-radius:6px;padding:4px 8px;color:%3;font-size:11px;}")
                                .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text.name());
    programInput->setStyleSheet(fieldStyle);
    batchInput->setStyleSheet(fieldStyle);
    const auto buttonStyle = QStringLiteral("QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:5px 10px;color:%3;font-size:11px;}"
                                            "QPushButton:hover{background:%4;}")
                                 .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text.name(), Theme::palette().bgSunken.name());
    pauseButton->setStyleSheet(buttonStyle);
    expandButton->setStyleSheet(buttonStyle);
    connect(pauseButton, &QPushButton::clicked, this, [this] {
        m_controller->setPaused(!m_controller->state().paused);
    });
    header->addWidget(programLabel);
    header->addWidget(programInput);
    header->addWidget(batchLabel);
    header->addWidget(batchInput);
    header->addWidget(pauseButton);
    header->addWidget(expandButton);
    layout->addLayout(header);

    m_cameraView = new CameraViewWidget;
    m_cameraView->setObjectName(QStringLiteral("cameraView"));
    layout->addWidget(m_cameraView, 1);
    return frame;
}

QWidget *MainWindow::createProfilePanel()
{
    auto *frame = new QFrame;
    frame->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    auto *header = new QHBoxLayout;
    header->setContentsMargins(0, 0, 0, 0);
    header->setSpacing(8);
    auto *title = new QLabel(QStringLiteral("膜厚轮廓曲线"));
    title->setObjectName(QStringLiteral("profilePanelTitleLabel"));
    title->setStyleSheet(QStringLiteral("font-size:12px;font-weight:600;color:%1;").arg(Theme::palette().text.name()));
    header->addWidget(title);
    header->addStretch();

    auto *scaleCombo = new QComboBox;
    scaleCombo->setObjectName(QStringLiteral("profileScaleCombo"));
    scaleCombo->addItems({QStringLiteral("1:1"), QStringLiteral("1:2"), QStringLiteral("2:1")});
    scaleCombo->setFixedWidth(68);
    scaleCombo->setFixedHeight(26);
    scaleCombo->setStyleSheet(QStringLiteral("QComboBox{background:%1;border:1px solid %2;border-radius:6px;padding:4px 8px;color:%3;font-size:11px;}"
                                             "QComboBox::drop-down{border:none;width:18px;}")
                                 .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text.name()));
    auto *hint = new QLabel(QStringLiteral("Y μm · X px"));
    hint->setObjectName(QStringLiteral("profileAxisHintLabel"));
    hint->setStyleSheet(QStringLiteral("font-size:11px;color:%1;").arg(Theme::palette().textMuted.name()));
    header->addWidget(scaleCombo);
    header->addWidget(hint);
    layout->addLayout(header);

    m_profileChart = new ProfileChartWidget;
    m_profileChart->setObjectName(QStringLiteral("profileChart"));
    layout->addWidget(m_profileChart, 1);
    return frame;
}

QWidget *MainWindow::createStatsPanel()
{
    auto *container = new QFrame;
    container->setStyleSheet(Theme::frameStyle());
    auto *outer = new QVBoxLayout(container);
    outer->setContentsMargins(10, 10, 10, 10);
    outer->setSpacing(8);

    auto *header = new QHBoxLayout;
    header->setContentsMargins(0, 0, 0, 0);
    auto *title = new QLabel(QStringLiteral("当前组 · GROUP-01"));
    title->setObjectName(QStringLiteral("currentGroupTitleLabel"));
    title->setStyleSheet(QStringLiteral("font-size:12px;font-weight:600;color:%1;").arg(Theme::palette().text.name()));
    auto *verdict = new QLabel(QStringLiteral("合格"));
    verdict->setObjectName(QStringLiteral("currentGroupVerdictLabel"));
    verdict->setStyleSheet(QStringLiteral("QLabel{background:#EEF7F0;border:1px solid #D7ECDC;border-radius:10px;padding:2px 8px;color:#357A4D;font-size:10px;font-weight:600;}"));
    header->addWidget(title);
    header->addStretch();
    header->addWidget(verdict);
    outer->addLayout(header);

    auto *layout = new QGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);

    m_thicknessCard = new StatCardWidget;
    m_thicknessCard->setObjectName(QStringLiteral("thicknessCard"));
    m_thicknessCard->setLabel(QStringLiteral("厚度 THK"));
    m_thicknessCard->setUnit(QStringLiteral("μm"));
    m_thicknessCard->setMetaText(QStringLiteral("Target 11.500"));
    m_maxCard = new StatCardWidget;
    m_maxCard->setObjectName(QStringLiteral("maxCard"));
    m_maxCard->setLabel(QStringLiteral("MAX"));
    m_maxCard->setUnit(QStringLiteral("μm"));
    m_minCard = new StatCardWidget;
    m_minCard->setObjectName(QStringLiteral("minCard"));
    m_minCard->setLabel(QStringLiteral("MIN"));
    m_minCard->setUnit(QStringLiteral("μm"));
    m_deltaCard = new StatCardWidget;
    m_deltaCard->setObjectName(QStringLiteral("leftHeightCard"));
    m_deltaCard->setLabel(QStringLiteral("左高度"));
    m_deltaCard->setUnit(QStringLiteral("μm"));
    m_rightCard = new StatCardWidget;
    m_rightCard->setObjectName(QStringLiteral("rightHeightCard"));
    m_rightCard->setLabel(QStringLiteral("右高度"));
    m_rightCard->setUnit(QStringLiteral("μm"));

    layout->addWidget(m_thicknessCard, 0, 0, 2, 1);
    layout->addWidget(m_maxCard, 0, 1);
    layout->addWidget(m_minCard, 1, 1);
    layout->addWidget(m_deltaCard, 0, 2);
    layout->addWidget(m_rightCard, 1, 2);
    outer->addLayout(layout);
    return container;
}

void MainWindow::applyRecordToSummary(const MeasurementRecord &record)
{
    auto *verdict = findChild<QLabel*>(QStringLiteral("currentGroupVerdictLabel"));
    m_thicknessCard->setValue(QString::number(record.thick, 'f', 3));
    m_maxCard->setValue(QString::number(record.hmax, 'f', 3));
    m_minCard->setValue(QString::number(record.hmin, 'f', 3));
    m_deltaCard->setValue(QString::number(record.left, 'f', 3));
    m_rightCard->setValue(QString::number(record.right, 'f', 3));
    m_thicknessCard->setMetaText(QStringLiteral("Target 11.500  Δ %1").arg(QString::number(record.thick - 11.5, 'f', 3)));
    m_sensorPanel->setSensorValue(record.left);
    if (verdict != nullptr) {
        const bool ok = record.verdict == QStringLiteral("ok");
        const bool warn = record.verdict == QStringLiteral("warn");
        verdict->setText(ok ? QStringLiteral("合格") : (warn ? QStringLiteral("临界") : QStringLiteral("超差")));
        verdict->setStyleSheet(ok
                                   ? QStringLiteral("QLabel{background:#EEF7F0;border:1px solid #D7ECDC;border-radius:10px;padding:2px 8px;color:#357A4D;font-size:10px;font-weight:600;}")
                                   : (warn
                                          ? QStringLiteral("QLabel{background:#FFF9EA;border:1px solid #F2E2AC;border-radius:10px;padding:2px 8px;color:#9A6B00;font-size:10px;font-weight:600;}")
                                          : QStringLiteral("QLabel{background:#FFF4F1;border:1px solid #F1D0C5;border-radius:10px;padding:2px 8px;color:#A54E2F;font-size:10px;font-weight:600;}")));
    }
}
