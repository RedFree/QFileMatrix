#include "app/MainWindow.h"

#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
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
#include "widgets/PanelHeaderWidget.h"
#include "widgets/ProfileChartWidget.h"
#include "widgets/StatCardWidget.h"
#include "dialogs/ManualSampleDialog.h"
#include "dialogs/AlarmCenterDialog.h"

namespace {
class RailIconButton : public QPushButton
{
public:
    enum class IconKind {
        Camera,
        Curve,
        Servo,
        History,
        Stats,
        Gauge,
        Settings,
        User,
    };

    explicit RailIconButton(IconKind kind, QWidget *parent = nullptr)
        : QPushButton(parent)
        , m_kind(kind)
    {
        setText(QString());
        setProperty("railIcon", iconName(kind));
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPushButton::paintEvent(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const bool active = property("active").toBool();
        const QColor color = active ? Theme::palette().brand : Theme::palette().textMuted;
        QPen pen(color);
        pen.setWidthF(1.6);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        const QRectF r = rect().adjusted(11, 11, -11, -11);
        switch (m_kind) {
        case IconKind::Camera:
            painter.drawRoundedRect(r.adjusted(0, 2, 0, 0), 2, 2);
            painter.drawEllipse(r.center(), 3.2, 3.2);
            painter.drawLine(QPointF(r.left() + 2, r.top() + 2), QPointF(r.left() + 5, r.top() + 2));
            break;
        case IconKind::Curve:
            painter.drawPolyline(QPolygonF({QPointF(r.left(), r.center().y() + 2), QPointF(r.left() + 3, r.center().y() + 1), QPointF(r.left() + 6, r.center().y() - 4), QPointF(r.left() + 9, r.center().y() + 4), QPointF(r.right(), r.center().y() - 1)}));
            break;
        case IconKind::Servo:
            painter.drawEllipse(r.center(), 6.0, 6.0);
            painter.drawEllipse(r.center(), 1.6, 1.6);
            painter.drawLine(QPointF(r.center().x(), r.top()), QPointF(r.center().x(), r.top() + 3));
            painter.drawLine(QPointF(r.center().x(), r.bottom()), QPointF(r.center().x(), r.bottom() - 3));
            painter.drawLine(QPointF(r.left(), r.center().y()), QPointF(r.left() + 3, r.center().y()));
            painter.drawLine(QPointF(r.right(), r.center().y()), QPointF(r.right() - 3, r.center().y()));
            break;
        case IconKind::History:
            painter.drawLine(QPointF(r.left() + 2, r.top() + 2), QPointF(r.right(), r.top() + 2));
            painter.drawLine(QPointF(r.left() + 2, r.center().y()), QPointF(r.right(), r.center().y()));
            painter.drawLine(QPointF(r.left() + 2, r.bottom() - 2), QPointF(r.right(), r.bottom() - 2));
            painter.drawEllipse(QPointF(r.left(), r.top() + 2), 0.8, 0.8);
            painter.drawEllipse(QPointF(r.left(), r.center().y()), 0.8, 0.8);
            painter.drawEllipse(QPointF(r.left(), r.bottom() - 2), 0.8, 0.8);
            break;
        case IconKind::Stats:
            painter.drawLine(QPointF(r.left(), r.bottom()), QPointF(r.right(), r.bottom()));
            painter.drawLine(QPointF(r.left() + 1, r.bottom()), QPointF(r.left() + 1, r.center().y() + 2));
            painter.drawLine(QPointF(r.center().x(), r.bottom()), QPointF(r.center().x(), r.top() + 2));
            painter.drawLine(QPointF(r.right() - 1, r.bottom()), QPointF(r.right() - 1, r.center().y() - 1));
            break;
        case IconKind::Gauge:
            painter.drawArc(r.adjusted(0, 2, 0, 6), 0, 180 * 16);
            painter.drawLine(QPointF(r.center().x(), r.center().y() + 2), QPointF(r.right() - 1, r.top() + 4));
            break;
        case IconKind::Settings:
            painter.drawEllipse(r.center(), 4.4, 4.4);
            painter.drawEllipse(r.center(), 1.5, 1.5);
            painter.drawLine(QPointF(r.center().x(), r.top()), QPointF(r.center().x(), r.top() + 2));
            painter.drawLine(QPointF(r.center().x(), r.bottom()), QPointF(r.center().x(), r.bottom() - 2));
            painter.drawLine(QPointF(r.left(), r.center().y()), QPointF(r.left() + 2, r.center().y()));
            painter.drawLine(QPointF(r.right(), r.center().y()), QPointF(r.right() - 2, r.center().y()));
            break;
        case IconKind::User:
            painter.drawEllipse(QPointF(r.center().x(), r.top() + 4), 3.0, 3.0);
            painter.drawArc(QRectF(r.left() + 1, r.center().y() - 1, r.width() - 2, r.height() - 3), 30 * 16, 120 * 16);
            break;
        }
    }

private:
    static QString iconName(IconKind kind)
    {
        switch (kind) {
        case IconKind::Camera: return QStringLiteral("camera");
        case IconKind::Curve: return QStringLiteral("curve");
        case IconKind::Servo: return QStringLiteral("servo");
        case IconKind::History: return QStringLiteral("history");
        case IconKind::Stats: return QStringLiteral("stats");
        case IconKind::Gauge: return QStringLiteral("gauge");
        case IconKind::Settings: return QStringLiteral("settings");
        case IconKind::User: return QStringLiteral("user");
        }
        return QString();
    }

    IconKind m_kind;
};

class SmallIconButton : public QPushButton
{
public:
  enum class Icon { Play, Pause, Expand };

  explicit SmallIconButton(Icon icon, const QString &text, QWidget *parent = nullptr)
    : QPushButton(text, parent), m_icon(icon)
  {
    const auto &p = Theme::palette();
    setFixedHeight(24);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(QStringLiteral(
      "QPushButton{background:transparent;border:none;border-radius:6px;padding:0 8px 0 22px;color:%1;font-size:11px;}"
      "QPushButton:hover{background:%2;color:%3;}"
    ).arg(p.textMuted.name(), p.bgSunken.name(), p.text1.name()));
  }

  void setIconType(Icon icon) { m_icon = icon; update(); }

protected:
  void paintEvent(QPaintEvent *event) override
  {
    QPushButton::paintEvent(event);
    const auto &p = Theme::palette();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QColor iconColor = underMouse() ? p.text1 : p.textMuted;
    QPen pen(iconColor);
    pen.setWidthF(1.4);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    const qreal cx = 10, cy = height() / 2.0;
    switch (m_icon) {
    case Icon::Play:
      painter.setBrush(iconColor);
      painter.drawPolygon(QPolygonF({QPointF(cx - 3, cy - 4), QPointF(cx - 3, cy + 4), QPointF(cx + 4, cy)}));
      break;
    case Icon::Pause:
      painter.setBrush(iconColor);
      painter.drawRect(QRectF(cx - 4, cy - 3.5, 2.8, 7));
      painter.drawRect(QRectF(cx + 1.2, cy - 3.5, 2.8, 7));
      break;
    case Icon::Expand:
      painter.drawLine(QPointF(cx - 4, cy - 4), QPointF(cx - 1, cy - 4));
      painter.drawLine(QPointF(cx - 4, cy - 4), QPointF(cx - 4, cy - 1));
      painter.drawLine(QPointF(cx + 4, cy - 4), QPointF(cx + 1, cy - 4));
      painter.drawLine(QPointF(cx + 4, cy - 4), QPointF(cx + 4, cy - 1));
      painter.drawLine(QPointF(cx - 4, cy + 4), QPointF(cx - 1, cy + 4));
      painter.drawLine(QPointF(cx - 4, cy + 4), QPointF(cx - 4, cy + 1));
      painter.drawLine(QPointF(cx + 4, cy + 4), QPointF(cx + 1, cy + 4));
      painter.drawLine(QPointF(cx + 4, cy + 4), QPointF(cx + 4, cy + 1));
      break;
    }
  }

private:
  Icon m_icon;
};

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

class VerdictPillWidget : public QWidget
{
public:
    explicit VerdictPillWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setFixedHeight(20);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        setProperty("verdict", QStringLiteral("ok"));
    }

    QSize sizeHint() const override { return QSize(52, 20); }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        const QString v = property("verdict").toString();
        QColor bg = Theme::palette().bgSunken;
        QColor border = Theme::palette().border;
        QColor fg = Theme::palette().textMuted;
        QString text = QStringLiteral("待定");

        if (v == QStringLiteral("ok")) {
            bg = Theme::palette().okWeak;
            border = Theme::palette().okWeak.darker(115);
            fg = Theme::palette().ok;
            text = QStringLiteral("合格");
        } else if (v == QStringLiteral("warn")) {
            bg = Theme::palette().warnWeak;
            border = Theme::palette().warnWeak.darker(115);
            fg = Theme::palette().warn.darker(140);
            text = QStringLiteral("临界");
        } else if (v == QStringLiteral("err")) {
            bg = Theme::palette().errWeak;
            border = Theme::palette().errWeak.darker(115);
            fg = Theme::palette().err;
            text = QStringLiteral("超差");
        }

        const QRect pillRect = rect().adjusted(0, 1, 0, -1);
        painter.setPen(border);
        painter.setBrush(bg);
        painter.drawRoundedRect(pillRect, pillRect.height() / 2.0, pillRect.height() / 2.0);

        painter.setPen(Qt::NoPen);
        painter.setBrush(fg);
        const qreal dotR = 3.0;
        const qreal dotX = pillRect.left() + pillRect.height() / 2.0;
        const qreal dotY = pillRect.center().y();
        painter.drawEllipse(QPointF(dotX, dotY), dotR, dotR);

        painter.setPen(fg);
        painter.setBrush(Qt::NoBrush);
        const QFont font(QStringLiteral("Consolas"), 9);
        painter.setFont(font);
        const QRect textRect = pillRect.adjusted(qRound(pillRect.height() / 2.0) + 2, 0, -4, 0);
        painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
    }
};
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
    rightPanel->setObjectName(QStringLiteral("rightControlColumn"));
    rightPanel->setStyleSheet(QStringLiteral("background:%1;border-left:1px solid %2;")
        .arg(Theme::palette().bgRail.name(), Theme::palette().border.name()));
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    m_sensorPanel = new SensorPanel;
    m_measurePanel = new MeasureControlPanel;
    m_servoPanel = new ServoControlPanel;

    const auto flushPanelStyle = QStringLiteral("background:%1;border:none;border-bottom:1px solid %2;border-radius:0;")
        .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name());
    m_sensorPanel->setStyleSheet(flushPanelStyle);
    m_measurePanel->setStyleSheet(flushPanelStyle);
    m_servoPanel->setStyleSheet(flushPanelStyle);

    m_sensorPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_measurePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_servoPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    rightLayout->addWidget(m_sensorPanel);
    rightLayout->addWidget(m_measurePanel);
    rightLayout->addWidget(m_servoPanel);
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
    connect(m_deviceStatusBar, &DeviceStatusBar::stopRequested, m_controller, &AppController::stopMeasurement);
    connect(m_deviceStatusBar, &DeviceStatusBar::manualRequested, this, [this] {
        auto *dialog = new ManualSampleDialog(this);
        const auto &pos = m_controller->state().pos;
        dialog->setPosition(QString::number(pos.x, 'f', 3), QString::number(pos.y, 'f', 3));
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(dialog, &ManualSampleDialog::sampleConfirmed, m_controller, &AppController::manualSample);
        dialog->exec();
    });

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

 auto *alarmButton = m_topTitleBar->findChild<QPushButton*>(QStringLiteral("alarmButton"));
 if (alarmButton) {
     connect(alarmButton, &QPushButton::clicked, this, [this, alarmButton] {
         auto *popup = new AlarmCenterDialog(this);
         const QPoint btnTopRight = alarmButton->mapToGlobal(QPoint(alarmButton->width(), 0));
         popup->showPopup(btnTopRight);
     });
 }
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
    m_cameraView->setStation(1);
    m_cameraView->setFrameRate(22.0);
    m_cameraView->setExposureMs(3.2);
    m_cameraView->setGain(1.4);
    m_profileChart->setMeasuring(state.measuring);
    m_profileChart->setProfile(m_controller->currentProfile());
 if (auto *pauseButton = findChild<QPushButton*>(QStringLiteral("cameraPauseButton"))) {
 pauseButton->setText(state.paused ? QStringLiteral("继续") : QStringLiteral("暂停"));
 if (auto *sb = dynamic_cast<SmallIconButton*>(pauseButton)) {
 sb->setIconType(state.paused ? SmallIconButton::Icon::Play : SmallIconButton::Icon::Pause);
 }
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
        RailIconButton::IconKind iconKind;
        const char *tooltip;
        bool active;
    } navButtons[] = {
        {"railCameraButton", RailIconButton::IconKind::Camera, "相机", true},
        {"railCurveButton", RailIconButton::IconKind::Curve, "曲线", false},
        {"railServoButton", RailIconButton::IconKind::Servo, "伺服", false},
        {"railHistoryButton", RailIconButton::IconKind::History, "记录", false},
        {"railStatsButton", RailIconButton::IconKind::Stats, "统计", false},
        {"railGaugeButton", RailIconButton::IconKind::Gauge, "仪表", false},
    };

    QList<QPushButton*> navButtonWidgets;
    for (const auto &spec : navButtons) {
        auto *button = new RailIconButton(spec.iconKind);
        button->setObjectName(QString::fromLatin1(spec.name));
        button->setToolTip(QString::fromUtf8(spec.tooltip));
        button->setFixedSize(40, 40);
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

    auto *settingsButton = new RailIconButton(RailIconButton::IconKind::Settings);
    settingsButton->setObjectName(QStringLiteral("railSettingsButton"));
    settingsButton->setToolTip(QStringLiteral("设置"));
    settingsButton->setFixedSize(40, 40);
    settingsButton->setCursor(Qt::PointingHandCursor);
    settingsButton->setStyleSheet(Theme::railButtonStyle(false));
    layout->addWidget(settingsButton);

    auto *userButton = new RailIconButton(RailIconButton::IconKind::User);
    userButton->setObjectName(QStringLiteral("railUserButton"));
    userButton->setToolTip(QStringLiteral("用户"));
    userButton->setFixedSize(40, 40);
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
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *header = new PanelHeaderWidget(QStringLiteral("实时图像 · CAM-01"));
    header->titleLabel()->setObjectName(QStringLiteral("cameraPanelTitleLabel"));

 auto *programLabel = new QLabel(QStringLiteral("程序号"));
 programLabel->setStyleSheet(QStringLiteral("color:%1;font-size:11px;letter-spacing:1px;font-weight:500;").arg(Theme::palette().text3.name()));
 auto *programInput = new QLineEdit(QStringLiteral("1010"));
 programInput->setObjectName(QStringLiteral("cameraProgramInput"));
 programInput->setFixedWidth(50);
 programInput->setFixedHeight(24);
 auto *batchLabel = new QLabel(QStringLiteral("批号"));
 batchLabel->setStyleSheet(QStringLiteral("color:%1;font-size:11px;letter-spacing:1px;font-weight:500;").arg(Theme::palette().text3.name()));
 auto *batchInput = new QLineEdit(QStringLiteral("3"));
 batchInput->setObjectName(QStringLiteral("cameraBatchInput"));
 batchInput->setFixedWidth(30);
 batchInput->setFixedHeight(24);
 auto *pauseButton = new SmallIconButton(SmallIconButton::Icon::Pause, QStringLiteral("暂停"));
 pauseButton->setObjectName(QStringLiteral("cameraPauseButton"));
 auto *expandButton = new SmallIconButton(SmallIconButton::Icon::Expand, QStringLiteral("展开"));
 expandButton->setObjectName(QStringLiteral("cameraExpandButton"));
  programInput->setStyleSheet(Theme::fieldStyle());
  batchInput->setStyleSheet(Theme::fieldStyle());
 connect(pauseButton, &QPushButton::clicked, this, [this, pauseButton] {
 m_controller->setPaused(!m_controller->state().paused);
 pauseButton->setText(m_controller->state().paused ? QStringLiteral("继续") : QStringLiteral("暂停"));
 pauseButton->setIconType(m_controller->state().paused ? SmallIconButton::Icon::Play : SmallIconButton::Icon::Pause);
 });
 header->rightLayout()->addWidget(programLabel);
 header->rightLayout()->addWidget(programInput);
 header->rightLayout()->addWidget(batchLabel);
 header->rightLayout()->addWidget(batchInput);
    header->rightLayout()->addWidget(pauseButton);
header->rightLayout()->addWidget(expandButton);
layout->addWidget(header);

auto *body = new QVBoxLayout;
body->setContentsMargins(0, 0, 0, 0);
m_cameraView = new CameraViewWidget;
    m_cameraView->setObjectName(QStringLiteral("cameraView"));
    body->addWidget(m_cameraView, 1);
    layout->addLayout(body);
    return frame;
}

QWidget *MainWindow::createProfilePanel()
{
    auto *frame = new QFrame;
    frame->setStyleSheet(Theme::frameStyle());
    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *header = new PanelHeaderWidget(QStringLiteral("膜厚轮廓曲线"));
    header->titleLabel()->setObjectName(QStringLiteral("profilePanelTitleLabel"));

 auto *scaleLabel = new QLabel(QStringLiteral("量程"));
 scaleLabel->setStyleSheet(QStringLiteral("color:%1;font-size:11px;letter-spacing:1px;font-weight:500;").arg(Theme::palette().text3.name()));
 auto *scaleCombo = new QComboBox;
 scaleCombo->setObjectName(QStringLiteral("profileScaleCombo"));
 scaleCombo->addItems({QStringLiteral("1:1"), QStringLiteral("1:2"), QStringLiteral("2:1")});
 scaleCombo->setFixedWidth(68);
 scaleCombo->setFixedHeight(24);
  scaleCombo->setStyleSheet(QStringLiteral(
      "QComboBox{background:%1;border:1px solid %2;border-radius:6px;padding:4px 8px;color:%3;font-size:11px;}"
      "QComboBox:hover{background:%4;}"
      "QComboBox::drop-down{border:none;width:18px;background:transparent;}"
  ).arg(Theme::palette().bgPanel.name(), Theme::palette().borderStrong.name(), Theme::palette().text1.name(), Theme::palette().bgSunken.name()));
 auto *hint = new QLabel(QStringLiteral("Y μm · X px"));
 hint->setObjectName(QStringLiteral("profileAxisHintLabel"));
 hint->setStyleSheet(QStringLiteral("font-size:10.5px;color:%1;").arg(Theme::palette().text3.name()));
 header->rightLayout()->addWidget(scaleLabel);
 header->rightLayout()->addWidget(scaleCombo);
    header->rightLayout()->addWidget(hint);
    layout->addWidget(header);

 auto *body = new QVBoxLayout;
 body->setContentsMargins(4, 8, 8, 0);
 m_profileChart = new ProfileChartWidget;
    m_profileChart->setObjectName(QStringLiteral("profileChart"));
    body->addWidget(m_profileChart, 1);
    layout->addLayout(body);
    return frame;
}

QWidget *MainWindow::createStatsPanel()
{
    auto *container = new QFrame;
    container->setStyleSheet(Theme::frameStyle());
    auto *outer = new QVBoxLayout(container);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    auto *header = new PanelHeaderWidget(QStringLiteral("当前组 · GROUP-01"));
    header->titleLabel()->setObjectName(QStringLiteral("currentGroupTitleLabel"));
 auto *verdict = new VerdictPillWidget;
 verdict->setObjectName(QStringLiteral("currentGroupVerdictLabel"));
 header->rightLayout()->addWidget(verdict);
    outer->addWidget(header);

    auto *body = new QVBoxLayout;
    body->setContentsMargins(10, 10, 10, 10);

    auto *layout = new QGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setHorizontalSpacing(8);
    layout->setVerticalSpacing(8);

    m_thicknessCard = new StatCardWidget;
    m_thicknessCard->setObjectName(QStringLiteral("thicknessCard"));
    m_thicknessCard->setLabel(QStringLiteral("厚度 THK"));
    m_thicknessCard->setUnit(QStringLiteral("μm"));
    m_thicknessCard->setTarget(QStringLiteral("11.500"));
m_maxCard = new StatCardWidget;
m_maxCard->setObjectName(QStringLiteral("maxCard"));
m_maxCard->setSmall(true);
m_maxCard->setLabel(QStringLiteral("MAX"));
m_maxCard->setUnit(QStringLiteral("μm"));
m_minCard = new StatCardWidget;
m_minCard->setObjectName(QStringLiteral("minCard"));
m_minCard->setSmall(true);
m_minCard->setLabel(QStringLiteral("MIN"));
m_minCard->setUnit(QStringLiteral("μm"));
m_deltaCard = new StatCardWidget;
m_deltaCard->setObjectName(QStringLiteral("leftHeightCard"));
m_deltaCard->setSmall(true);
m_deltaCard->setLabel(QStringLiteral("左高度"));
m_deltaCard->setUnit(QStringLiteral("μm"));
m_rightCard = new StatCardWidget;
m_rightCard->setObjectName(QStringLiteral("rightHeightCard"));
m_rightCard->setSmall(true);
m_rightCard->setLabel(QStringLiteral("右高度"));
m_rightCard->setUnit(QStringLiteral("μm"));

 layout->addWidget(m_thicknessCard, 0, 0, 2, 1);
 layout->setColumnStretch(0, 13);
 layout->setColumnStretch(1, 10);
 layout->setColumnStretch(2, 10);
 layout->addWidget(m_maxCard, 0, 1);
    layout->addWidget(m_minCard, 1, 1);
    layout->addWidget(m_deltaCard, 0, 2);
    layout->addWidget(m_rightCard, 1, 2);
    body->addLayout(layout);
    outer->addLayout(body);
    return container;
}

void MainWindow::applyRecordToSummary(const MeasurementRecord &record)
{
    auto *verdict = findChild<QWidget*>(QStringLiteral("currentGroupVerdictLabel"));
    m_thicknessCard->setValue(QString::number(record.thick, 'f', 3));
    m_maxCard->setValue(QString::number(record.hmax, 'f', 3));
    m_minCard->setValue(QString::number(record.hmin, 'f', 3));
    m_deltaCard->setValue(QString::number(record.left, 'f', 3));
    m_rightCard->setValue(QString::number(record.right, 'f', 3));
    m_thicknessCard->setTarget(QStringLiteral("11.500"));
    m_thicknessCard->setTrend(record.thick - 11.5);
    m_thicknessCard->setAccentColor(record.verdict == QStringLiteral("ok")
                                        ? Theme::palette().ok
                                        : (record.verdict == QStringLiteral("warn") ? Theme::palette().warn : Theme::palette().err));
    m_sensorPanel->setSensorValue(record.left);
 if (verdict != nullptr) {
     verdict->setProperty("verdict", record.verdict);
     verdict->update();
 }
}
