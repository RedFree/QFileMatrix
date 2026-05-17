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
#include <QStackedWidget>
#include <QStyleFactory>
#include <QTimer>
#include <QVBoxLayout>

#include "app/AppController.h"
#include "dialogs/AlarmCenterDialog.h"
#include "dialogs/ManualSampleDialog.h"
#include "domain/MeasurementConstants.h"
#include "panels/BottomStatusBar.h"
#include "panels/DeviceStatusBar.h"
#include "panels/HistoryPanel.h"
#include "panels/MeasureControlPanel.h"
#include "panels/SensorPanel.h"
#include "panels/ServoControlPanel.h"
#include "panels/TopTitleBar.h"
#include "theme/Theme.h"
#include "widgets/CameraViewWidget.h"
#include "widgets/PanelHeaderWidget.h"
#include "widgets/ProfileChartWidget.h"
#include "widgets/RailIconButton.h"
#include "widgets/SmallIconButton.h"
#include "widgets/StatCardWidget.h"
#include "widgets/VerdictPillWidget.h"

namespace {

QFrame *makeBodyPanel(QWidget *content)
{
    auto *frame = new QFrame;
    frame->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(content, 1);
    return frame;
}

} // namespace

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
    if (m_historyPanel)
        m_historyPanel->disconnect(this);
    if (m_controller)
        m_controller->disconnect(this);
}

void MainWindow::buildUi()
{
    auto *central = new QWidget;
    central->setStyleSheet(QStringLiteral("background:%1;").arg(Theme::palette().bgApp.name()));
    auto *root = new QVBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    auto *shell = new QFrame;
    shell->setObjectName(QStringLiteral("mainShellFrame"));
    shell->setStyleSheet(QStringLiteral(
        "QFrame#mainShellFrame{background:%1;border-top:1px solid %2;"
        "border-right:1px solid %2;border-bottom:1px solid %2;"
        "border-left:none;border-radius:0px;}")
        .arg(Theme::palette().bgPanel.name(), Theme::palette().borderStrong.name()));
    auto *shellLayout = new QVBoxLayout(shell);
    shellLayout->setContentsMargins(0, 0, 0, 0);
    shellLayout->setSpacing(0);

    m_topTitleBar = new TopTitleBar;
    m_deviceStatusBar = new DeviceStatusBar;
    m_bottomStatusBar = new BottomStatusBar;
    shellLayout->addWidget(m_topTitleBar);
    shellLayout->addWidget(m_deviceStatusBar);

    auto *bodySplitter = new QSplitter(Qt::Horizontal);
    bodySplitter->setChildrenCollapsible(false);
    bodySplitter->setHandleWidth(1);
    bodySplitter->setStyleSheet(QStringLiteral("QSplitter::handle{background:transparent;}"));
    bodySplitter->addWidget(createLeftRail());

    m_workspaceStack = new QStackedWidget;
    m_workspaceStack->setObjectName(QStringLiteral("mainWorkspaceStack"));
    m_workspaceStack->addWidget(createMeasureWorkspace());
    m_workspaceStack->addWidget(createPlaceholderWorkspace(
        QStringLiteral("数据"), QStringLiteral("数据查询、历史记录与导出功能将在这里替换显示。")));
    m_workspaceStack->addWidget(createPlaceholderWorkspace(
        QStringLiteral("配方"), QStringLiteral("配方列表、参数编辑与下发功能将在这里替换显示。")));
    m_workspaceStack->addWidget(createPlaceholderWorkspace(
        QStringLiteral("维护"), QStringLiteral("设备诊断、校准与维护操作将在这里替换显示。")));
    m_workspaceStack->addWidget(createPlaceholderWorkspace(
        QStringLiteral("审计"), QStringLiteral("操作日志、权限审计与追溯记录将在这里替换显示。")));
    bodySplitter->addWidget(m_workspaceStack);

    bodySplitter->setStretchFactor(0, 0);
    bodySplitter->setStretchFactor(1, 1);
    bodySplitter->setSizes({48, 1400});

    shellLayout->addWidget(bodySplitter, 1);
    shellLayout->addWidget(m_bottomStatusBar);
    root->addWidget(shell, 1);

    setCentralWidget(central);
}

QWidget *MainWindow::createMeasureWorkspace()
{
    auto *workspaceSplitter = new QSplitter(Qt::Horizontal);
    workspaceSplitter->setChildrenCollapsible(false);
    workspaceSplitter->setHandleWidth(1);
    workspaceSplitter->setStyleSheet(QStringLiteral("QSplitter::handle{background:transparent;}"));

    auto *centerSplitter = new QSplitter(Qt::Vertical);
    centerSplitter->setChildrenCollapsible(false);
    centerSplitter->setHandleWidth(1);
    centerSplitter->setStyleSheet(QStringLiteral("QSplitter::handle{background:transparent;}"));

    auto *topSplitter = new QSplitter(Qt::Horizontal);
    topSplitter->setChildrenCollapsible(false);
    topSplitter->setHandleWidth(1);
    topSplitter->setStyleSheet(QStringLiteral("QSplitter::handle{background:transparent;}"));
    topSplitter->addWidget(createCameraPanel());

    auto *rightTop = new QWidget;
    auto *rightTopLayout = new QVBoxLayout(rightTop);
    rightTopLayout->setContentsMargins(0, 0, 0, 0);
    rightTopLayout->setSpacing(4);
    rightTopLayout->addWidget(createStatsPanel());
    rightTopLayout->addWidget(createProfilePanel(), 1);
    topSplitter->addWidget(rightTop);
    topSplitter->setStretchFactor(0, 2);
    topSplitter->setStretchFactor(1, 2);

    centerSplitter->addWidget(topSplitter);
    m_historyPanel = new HistoryPanel;
    centerSplitter->addWidget(makeBodyPanel(m_historyPanel));
    centerSplitter->setStretchFactor(0, 3);
    centerSplitter->setStretchFactor(1, 2);
    QTimer::singleShot(0, this, [centerSplitter] {
        centerSplitter->setSizes({460, 280});
    });

    workspaceSplitter->addWidget(centerSplitter);

    auto *rightPanel = new QWidget;
    rightPanel->setObjectName(QStringLiteral("rightControlColumn"));
    rightPanel->setStyleSheet(QStringLiteral("background:%1;border-left:1px solid %2;")
        .arg(Theme::palette().bgRail.name(), Theme::palette().border.name()));
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    m_sensorPanel = new SensorPanel;
    m_sensorPanel->setObjectName(QStringLiteral("sensorPanel"));
    m_measurePanel = new MeasureControlPanel;
    m_measurePanel->setObjectName(QStringLiteral("measureControlPanel"));
    m_servoPanel = new ServoControlPanel;
    m_servoPanel->setObjectName(QStringLiteral("servoPanel"));
    m_servoPanel->setStyleSheet(QStringLiteral(
        "background:%1;border:none;border-bottom:1px solid %2;")
        .arg(Theme::palette().bgRail.name(), Theme::palette().border.name()));

    m_sensorPanel->setStyleSheet(QStringLiteral(
        "background:%1;border:none;border-bottom:1px solid %2;")
        .arg(Theme::palette().bgRail.name(), Theme::palette().border.name()));
    m_sensorPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_measurePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_servoPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    rightLayout->addWidget(m_sensorPanel);
    rightLayout->addWidget(m_measurePanel);
    rightLayout->addWidget(m_servoPanel);
    rightLayout->addStretch();

    auto *rightScrollArea = new QScrollArea;
    rightScrollArea->setObjectName(QStringLiteral("rightControlScrollArea"));
    rightScrollArea->setWidgetResizable(true);
    rightScrollArea->setFrameShape(QFrame::NoFrame);
    rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rightScrollArea->setWidget(rightPanel);

    workspaceSplitter->addWidget(rightScrollArea);
    workspaceSplitter->setStretchFactor(0, 1);
    workspaceSplitter->setStretchFactor(1, 0);
    workspaceSplitter->setSizes({1080, 320});

    return workspaceSplitter;
}

// --- createPlaceholderWorkspace ---

QWidget *MainWindow::createPlaceholderWorkspace(const QString &title, const QString &description)
{
    auto *page = new QFrame;
    page->setObjectName(QStringLiteral("placeholderWorkspacePage"));
    page->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);

    auto *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet(QStringLiteral(
        "font-size:22px;font-weight:700;color:%1;").arg(Theme::palette().text.name()));
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *descLabel = new QLabel(description);
    descLabel->setStyleSheet(QStringLiteral(
        "font-size:13px;color:%1;").arg(Theme::palette().textMuted.name()));
    descLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(titleLabel);
    layout->addWidget(descLabel);
    return page;
}

// --- wireUi ---

void MainWindow::wireUi()
{
    m_historyPanel->setModel(m_controller->tableModel());

    connect(m_topTitleBar, &TopTitleBar::sectionRequested, this, [this](const QString &key) {
        static const QStringList keys = {
            QStringLiteral("measure"), QStringLiteral("data"),
            QStringLiteral("recipe"), QStringLiteral("maintenance"),
            QStringLiteral("audit")};
        int idx = keys.indexOf(key);
        if (idx >= 0)
            m_workspaceStack->setCurrentIndex(idx);
        m_topTitleBar->setActiveSection(key);
    });

    connect(m_measurePanel, &MeasureControlPanel::startRequested,
            m_controller, &AppController::startMeasurement);
    connect(m_measurePanel, &MeasureControlPanel::stopRequested,
            m_controller, &AppController::stopMeasurement);
    connect(m_measurePanel, &MeasureControlPanel::manualRequested,
            m_controller, &AppController::manualSample);
    connect(m_measurePanel, &MeasureControlPanel::sampleCountChanged,
            m_controller, &AppController::setSampleCount);

    connect(m_deviceStatusBar, &DeviceStatusBar::startRequested,
            m_controller, &AppController::startMeasurement);
    connect(m_deviceStatusBar, &DeviceStatusBar::stopRequested,
            m_controller, &AppController::stopMeasurement);
    connect(m_deviceStatusBar, &DeviceStatusBar::manualRequested, this, [this] {
        auto *dlg = new ManualSampleDialog(this);
        const auto &st = m_controller->state();
        dlg->setPosition(QString::number(double(st.pos.x), 'f', 3), QString::number(double(st.pos.y), 'f', 3));
        connect(dlg, &ManualSampleDialog::sampleConfirmed,
                m_controller, &AppController::manualSample);
        dlg->open();
    });

    connect(m_servoPanel, &ServoControlPanel::jogRequested,
            this, [this](int dx, int dy) {
        const auto &state = m_controller->state();
        m_controller->jog(dx * state.step, dy * state.step);
    });
    connect(m_servoPanel, &ServoControlPanel::homeRequested,
            m_controller, &AppController::goHome);
    connect(m_servoPanel, &ServoControlPanel::gotoRequested,
            m_controller, &AppController::jog);
    connect(m_servoPanel, &ServoControlPanel::stepChanged,
            m_controller, &AppController::setStep);

    connect(m_controller, &AppController::stateChanged,
            this, &MainWindow::updateFromController);
    connect(m_controller, &AppController::latestRecordChanged,
            this, &MainWindow::updateFromController);
    connect(m_controller, &AppController::profileChanged,
            this, &MainWindow::updateFromController);

    connect(m_historyPanel, &HistoryPanel::currentRowChanged, this, [this](int row) {
        m_selectedHistoryRow = row;
        updateFromController();
    });

    auto *alarmButton = m_topTitleBar->findChild<QPushButton*>(QStringLiteral("alarmButton"));
    if (alarmButton) {
        connect(alarmButton, &QPushButton::clicked, this, [this, alarmButton] {
            auto *dlg = new AlarmCenterDialog(this);
            QPoint pos = alarmButton->mapToGlobal(QPoint(0, alarmButton->height()));
            dlg->showPopup(pos);
        });
    }
}

// --- updateFromController ---

void MainWindow::updateFromController()
{
    const auto &st = m_controller->state();

    m_deviceStatusBar->setProgress(st.progress);
    m_deviceStatusBar->setMeasuring(st.measuring);
    m_measurePanel->setProgress(st.progress);
    m_measurePanel->setMeasuring(st.measuring);
    m_measurePanel->setSampleCount(st.sampleCount);
    m_servoPanel->setPosition(st.pos);
    m_servoPanel->setStep(st.step);

    m_bottomStatusBar->setPosition(st.pos);
    m_bottomStatusBar->setConnected(true);
    m_bottomStatusBar->setMeasuring(st.measuring);

    m_cameraView->setPaused(st.paused);
    m_cameraView->setMeasuring(st.measuring);
    m_cameraView->setStation(1);
    m_cameraView->setFrameRate(22.0);
    m_cameraView->setExposureMs(3.2);
    m_cameraView->setGain(1.4);

    m_profileChart->setMeasuring(st.measuring);

    auto *pauseBtn = findChild<SmallIconButton*>(QStringLiteral("cameraPauseButton"));
    if (pauseBtn) {
        pauseBtn->setIconType(st.paused ? SmallIconButton::Icon::Play
                                        : SmallIconButton::Icon::Pause);
    }

    auto *model = m_controller->tableModel();
    MeasurementRecord record;
    if (model->rowCount() > 0 && m_selectedHistoryRow < model->rowCount())
        record = model->recordAt(m_selectedHistoryRow);
    else
        record = m_controller->latestRecord();

    m_bottomStatusBar->setProbeValue(record.left);
    m_profileChart->setProfile(m_controller->currentProfile());
    applyRecordToSummary(record);
}

// --- createLeftRail ---

QWidget *MainWindow::createLeftRail()
{
    auto *rail = new QWidget;
    rail->setFixedWidth(48);
    rail->setStyleSheet(QStringLiteral("background:%1;border-right:1px solid %2;")
        .arg(Theme::palette().bgRail.name(), Theme::palette().border.name()));

    auto *layout = new QVBoxLayout(rail);
    layout->setContentsMargins(4, 8, 4, 8);
    layout->setSpacing(4);

    using IK = RailIconButton::IconKind;
    auto *btnCamera   = new RailIconButton(IK::Camera);
    auto *btnCurve    = new RailIconButton(IK::Curve);
    auto *btnServo    = new RailIconButton(IK::Servo);
    auto *btnHistory  = new RailIconButton(IK::History);
    auto *btnStats    = new RailIconButton(IK::Stats);
    auto *btnGauge    = new RailIconButton(IK::Gauge);
    auto *btnSettings = new RailIconButton(IK::Settings);
    auto *btnUser     = new RailIconButton(IK::User);

    btnCamera->setObjectName(QStringLiteral("railCameraButton"));
    btnCurve->setObjectName(QStringLiteral("railCurveButton"));
    btnServo->setObjectName(QStringLiteral("railServoButton"));
    btnHistory->setObjectName(QStringLiteral("railHistoryButton"));
    btnStats->setObjectName(QStringLiteral("railStatsButton"));
    btnGauge->setObjectName(QStringLiteral("railGaugeButton"));
    btnSettings->setObjectName(QStringLiteral("railSettingsButton"));
    btnUser->setObjectName(QStringLiteral("railUserButton"));

    btnCamera->setToolTip(QStringLiteral("相机"));
    btnCurve->setToolTip(QStringLiteral("曲线"));
    btnServo->setToolTip(QStringLiteral("伺服"));
    btnHistory->setToolTip(QStringLiteral("记录"));
    btnStats->setToolTip(QStringLiteral("统计"));
    btnGauge->setToolTip(QStringLiteral("仪表"));
    btnSettings->setToolTip(QStringLiteral("设置"));
    btnUser->setToolTip(QStringLiteral("用户"));

    QList<RailIconButton*> topButtons = {
        btnCamera, btnCurve, btnServo, btnHistory, btnStats, btnGauge};
    QList<RailIconButton*> bottomButtons = {btnSettings, btnUser};
    QList<RailIconButton*> allButtons = topButtons + bottomButtons;

    for (auto *btn : allButtons) {
        btn->setFixedSize(40, 40);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(Theme::railButtonStyle(false));
    }

    for (auto *btn : topButtons)
        layout->addWidget(btn);
    layout->addStretch();
    for (auto *btn : bottomButtons)
        layout->addWidget(btn);

    btnCamera->setProperty("active", true);
    btnCamera->setStyleSheet(Theme::railButtonStyle(true));
    for (auto *btn : allButtons) {
        connect(btn, &QPushButton::clicked, this, [allButtons, btn] {
            for (auto *other : allButtons) {
                bool active = (other == btn);
                other->setProperty("active", active);
                other->setStyleSheet(Theme::railButtonStyle(active));
            }
        });
    }

    return rail;
}

// --- createCameraPanel ---

QWidget *MainWindow::createCameraPanel()
{
    auto *frame = new QFrame;
    frame->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *header = new PanelHeaderWidget(QStringLiteral("实时图像 · CAM-01"));
    header->titleLabel()->setObjectName(QStringLiteral("cameraPanelTitleLabel"));

    auto *programInput = new QLineEdit;
    programInput->setObjectName(QStringLiteral("cameraProgramInput"));
    programInput->setPlaceholderText(QStringLiteral("程序"));
    programInput->setStyleSheet(Theme::fieldStyle());
    programInput->setFixedSize(80, 24);

    auto *batchInput = new QLineEdit;
    batchInput->setObjectName(QStringLiteral("cameraBatchInput"));
    batchInput->setPlaceholderText(QStringLiteral("批次"));
    batchInput->setStyleSheet(Theme::fieldStyle());
    batchInput->setFixedSize(30, 24);

    auto *pauseBtn = new SmallIconButton(SmallIconButton::Icon::Pause,
        QStringLiteral("暂停"));
    pauseBtn->setObjectName(QStringLiteral("cameraPauseButton"));
    auto *expandBtn = new SmallIconButton(SmallIconButton::Icon::Expand,
        QStringLiteral("展开"));
    expandBtn->setObjectName(QStringLiteral("cameraExpandButton"));

    header->rightLayout()->addWidget(programInput);
    header->rightLayout()->addWidget(batchInput);
    header->rightLayout()->addWidget(pauseBtn);
    header->rightLayout()->addWidget(expandBtn);

    m_cameraView = new CameraViewWidget;
    m_cameraView->setObjectName(QStringLiteral("cameraView"));

    connect(pauseBtn, &QPushButton::clicked, this, [this, pauseBtn] {
        bool paused = !m_cameraView->paused();
        m_cameraView->setPaused(paused);
        m_controller->setPaused(paused);
        pauseBtn->setIconType(paused ? SmallIconButton::Icon::Play
                                     : SmallIconButton::Icon::Pause);
    });

    layout->addWidget(header);
    layout->addWidget(m_cameraView, 1);
    return frame;
}

// --- createProfilePanel ---

QWidget *MainWindow::createProfilePanel()
{
    auto *frame = new QFrame;
    frame->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *header = new PanelHeaderWidget(QString());
    header->titleLabel()->setObjectName(QStringLiteral("profilePanelTitleLabel"));
    header->titleLabel()->hide();

    auto *scaleCombo = new QComboBox;
    scaleCombo->setObjectName(QStringLiteral("profileScaleCombo"));
    scaleCombo->addItems({QStringLiteral("1x"), QStringLiteral("2x"),
                          QStringLiteral("4x"), QStringLiteral("Auto")});
    scaleCombo->setStyleSheet(Theme::comboBoxStyle());
    scaleCombo->setFixedSize(60, 24);

    auto *hintLabel = new QLabel(QStringLiteral("拖拽区域以标注参考带"));
    hintLabel->setObjectName(QStringLiteral("profileAxisHintLabel"));
    hintLabel->setStyleSheet(QStringLiteral(
        "font-size:11px;color:%1;").arg(Theme::palette().textMuted.name()));

    header->rightLayout()->addWidget(hintLabel);
    header->rightLayout()->addWidget(scaleCombo);

    m_profileChart = new ProfileChartWidget;
    m_profileChart->setObjectName(QStringLiteral("profileChart"));

    layout->addWidget(header);
    layout->addWidget(m_profileChart, 1);
    return frame;
}

// --- createStatsPanel ---

QWidget *MainWindow::createStatsPanel()
{
    auto *frame = new QFrame;
    frame->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    auto *panelHeader = new PanelHeaderWidget(QStringLiteral("当前组 · GROUP-01"));

    auto *titleLabel = panelHeader->titleLabel();
    titleLabel->setObjectName(QStringLiteral("currentGroupTitleLabel"));

    m_groupButtons.clear();
    for (int i = 0; i < MeasurementConstants::GroupCount; ++i) {
        auto *btn = new QPushButton(QString::number(i + 1));
        btn->setObjectName(QStringLiteral("currentGroupButton%1").arg(i + 1));
        btn->setFixedSize(24, 20);
        btn->setCursor(Qt::PointingHandCursor);
        m_groupButtons.append(btn);
        panelHeader->rightLayout()->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [this, i] {
            setActiveGroup(i);
        });
    }

    auto *verdict = new VerdictPillWidget;
    verdict->setObjectName(QStringLiteral("currentGroupVerdictLabel"));
    panelHeader->rightLayout()->addWidget(verdict);

    layout->addWidget(panelHeader);

    // Status dots row
    auto *dotsContainer = new QWidget;
    dotsContainer->setObjectName(QStringLiteral("currentGroupStatusRow"));
    auto *dotsRow = new QHBoxLayout(dotsContainer);
    dotsRow->setContentsMargins(0, 0, 0, 0);
    dotsRow->setSpacing(4);

    auto *dot1 = new QWidget;
    dot1->setObjectName(QStringLiteral("currentGroupStatusOk1"));
    dot1->setFixedSize(10, 10);
    dot1->setStyleSheet(QStringLiteral(
        "background:%1;border-radius:5px;").arg(Theme::palette().ok.name()));

    auto *dot2 = new QWidget;
    dot2->setObjectName(QStringLiteral("currentGroupStatusOk2"));
    dot2->setFixedSize(10, 10);
    dot2->setStyleSheet(QStringLiteral(
        "background:%1;border-radius:5px;").arg(Theme::palette().ok.name()));

    auto *dot3 = new QWidget;
    dot3->setObjectName(QStringLiteral("currentGroupStatusOk3"));
    dot3->setFixedSize(10, 10);
    dot3->setStyleSheet(QStringLiteral(
        "background:%1;border-radius:5px;").arg(Theme::palette().ok.name()));

    auto *dotWarn = new QWidget;
    dotWarn->setObjectName(QStringLiteral("currentGroupStatusWarn"));
    dotWarn->setFixedSize(10, 10);
    dotWarn->setStyleSheet(QStringLiteral(
        "background:%1;border-radius:5px;").arg(Theme::palette().warn.name()));

    dotsRow->addWidget(dot1);
    dotsRow->addWidget(dot2);
    dotsRow->addWidget(dot3);
    dotsRow->addWidget(dotWarn);
    dotsRow->addStretch();
    layout->addWidget(dotsContainer);

    // Stat cards grid
    auto *grid = new QGridLayout;
    grid->setSpacing(6);

    m_thicknessCard = new StatCardWidget;
    m_thicknessCard->setObjectName(QStringLiteral("thicknessCard"));
    m_thicknessCard->setLabel(QStringLiteral("CURRENT"));
    m_thicknessCard->setUnit(QStringLiteral("μm"));
    m_thicknessCard->setTarget(QString::number(MeasurementConstants::TargetThickness, 'f', 3));
    m_thicknessCard->setShowProgress(true);
    m_thicknessCard->setAccentColor(Theme::palette().brand);

    m_maxCard = new StatCardWidget;
    m_maxCard->setObjectName(QStringLiteral("maxCard"));
    m_maxCard->setLabel(QStringLiteral("MAX"));
    m_maxCard->setUnit(QStringLiteral("μm"));
    m_maxCard->setSmall(true);
    m_maxCard->setAccentColor(Theme::palette().warn);

    m_minCard = new StatCardWidget;
    m_minCard->setObjectName(QStringLiteral("minCard"));
    m_minCard->setLabel(QStringLiteral("MIN"));
    m_minCard->setUnit(QStringLiteral("μm"));
    m_minCard->setSmall(true);
    m_minCard->setAccentColor(Theme::palette().ok);

    m_deltaCard = new StatCardWidget;
    m_deltaCard->setObjectName(QStringLiteral("leftHeightCard"));
    m_deltaCard->setLabel(QStringLiteral("左高度"));
    m_deltaCard->setUnit(QStringLiteral("μm"));
    m_deltaCard->setSmall(true);
    m_deltaCard->setAccentColor(Theme::palette().accentTrace);

    m_rightCard = new StatCardWidget;
    m_rightCard->setObjectName(QStringLiteral("rightHeightCard"));
    m_rightCard->setLabel(QStringLiteral("右高度"));
    m_rightCard->setUnit(QStringLiteral("μm"));
    m_rightCard->setSmall(true);
    m_rightCard->setAccentColor(Theme::palette().accentRef);

    grid->addWidget(m_thicknessCard, 0, 0, 2, 1);
    grid->addWidget(m_maxCard, 0, 1);
    grid->addWidget(m_minCard, 1, 1);
    grid->addWidget(m_deltaCard, 0, 2);
    grid->addWidget(m_rightCard, 1, 2);

    layout->addLayout(grid);
    refreshGroupButtons();
    return frame;
}

// --- applyRecordToSummary ---

void MainWindow::applyRecordToSummary(const MeasurementRecord &record)
{
    m_groupSummaries.resize(MeasurementConstants::GroupCount);

    for (int g = 0; g < MeasurementConstants::GroupCount; ++g) {
        double offset = (g - 1) * 0.05;
        GroupSummary &s = m_groupSummaries[g];
        s.thick = record.thick + offset;
        s.hmax  = record.hmax + offset;
        s.hmin  = record.hmin + offset;
        s.left  = record.left + offset;
        s.right = record.right + offset;

        double dev = qAbs(s.thick - MeasurementConstants::TargetThickness);
        if (dev <= MeasurementConstants::ToleranceOk)
            s.verdict = QStringLiteral("ok");
        else if (dev <= MeasurementConstants::ToleranceWarn)
            s.verdict = QStringLiteral("warn");
        else
            s.verdict = QStringLiteral("err");
    }

    applyGroupSummary(m_activeGroupIndex);
}

// --- applyGroupSummary ---

void MainWindow::applyGroupSummary(int groupIndex)
{
    if (groupIndex < 0 || groupIndex >= m_groupSummaries.size())
        return;

    const GroupSummary &s = m_groupSummaries[groupIndex];

    m_thicknessCard->setValue(QString::number(s.thick, 'f', 2));
    double dev = qAbs(s.thick - MeasurementConstants::TargetThickness);
    double progress = qBound(0.0, 1.0 - dev * MeasurementConstants::ProgressScale, 1.0);
    m_thicknessCard->setProgress(progress);
    m_thicknessCard->setTrend(s.thick - MeasurementConstants::TargetThickness);

    QColor accentColor = Theme::palette().ok;
    if (s.verdict == QStringLiteral("warn"))
        accentColor = Theme::palette().warn;
    else if (s.verdict == QStringLiteral("err"))
        accentColor = Theme::palette().err;
    m_thicknessCard->setAccentColor(accentColor);

    m_maxCard->setValue(QString::number(s.hmax, 'f', 2));
    m_minCard->setValue(QString::number(s.hmin, 'f', 2));
    m_deltaCard->setValue(QString::number(s.left, 'f', 2));
    m_rightCard->setValue(QString::number(s.right, 'f', 2));

    m_sensorPanel->setSensorValue(s.left);
}

// --- setActiveGroup ---

void MainWindow::setActiveGroup(int groupIndex)
{
    if (groupIndex < 0 || groupIndex >= MeasurementConstants::GroupCount)
        return;
    m_activeGroupIndex = groupIndex;
    refreshGroupButtons();
    applyGroupSummary(groupIndex);
}

// --- refreshGroupButtons ---

void MainWindow::refreshGroupButtons()
{
    for (int i = 0; i < m_groupButtons.size(); ++i) {
        bool active = (i == m_activeGroupIndex);
        m_groupButtons[i]->setStyleSheet(active
            ? QStringLiteral("QPushButton{background:%1;color:%2;border:none;"
                "border-radius:4px;font-size:11px;font-weight:700;}")
                .arg(Theme::palette().brand.name(), Theme::palette().bgPanel.name())
            : QStringLiteral("QPushButton{background:%1;color:%2;border:1px solid %3;"
                "border-radius:4px;font-size:11px;font-weight:500;}")
                .arg(Theme::palette().bgSunken.name(),
                     Theme::palette().textMuted.name(),
                     Theme::palette().border.name()));
    }
}
