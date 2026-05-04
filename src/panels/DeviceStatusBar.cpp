#include "panels/DeviceStatusBar.h"

#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPainter>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>

#include "theme/Theme.h"
#include "widgets/LedIndicatorWidget.h"

namespace {
LedIndicatorWidget *makeStatus(const QString &name, const QString &label, const QString &sub, LedIndicatorWidget::State state)
{
    auto *indicator = new LedIndicatorWidget;
    indicator->setObjectName(name);
    indicator->setLabel(label);
    indicator->setSubLabel(sub);
    indicator->setState(state);
    indicator->setCompact(true);
    indicator->setMaximumWidth(96);
    return indicator;
}

class StatusLedWidget : public QLabel
{
public:
    explicit StatusLedWidget(QWidget *parent = nullptr)
        : QLabel(parent)
        , m_blinkTimer(new QTimer(this))
    {
        setFixedSize(8, 8);
        setScaledContents(false);
        m_blinkTimer->setInterval(500);
        connect(m_blinkTimer, &QTimer::timeout, this, [this] {
            m_visible = !m_visible;
            updateColor();
        });
    }

    void setMeasuring(bool measuring)
    {
        m_measuring = measuring;
        m_visible = true;
        if (m_measuring) {
            m_blinkTimer->start();
        } else {
            m_blinkTimer->stop();
        }
        updateColor();
    }

private:
    void updateColor()
    {
        const auto &p = Theme::palette();
        if (!m_measuring) {
            setStyleSheet(QStringLiteral("background:%1;border-radius:4px;").arg(p.err.name()));
        } else if (m_visible) {
            setStyleSheet(QStringLiteral("background:%1;border-radius:4px;").arg(p.ok.name()));
        } else {
            setStyleSheet(QStringLiteral("background:%1;border-radius:4px;").arg(p.border.name()));
        }
    }

    bool m_measuring = false;
    bool m_visible = true;
    QTimer *m_blinkTimer;
};
}

DeviceStatusBar::DeviceStatusBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(44);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(14, 0, 14, 0);
    layout->setSpacing(6);

    auto *leftWrap = new QWidget;
    auto *leftLayout = new QHBoxLayout(leftWrap);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(8);
    leftLayout->addWidget(makeStatus(QStringLiteral("commStatus"), QStringLiteral("通讯"), QStringLiteral("TCP"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("xAxisStatus"), QStringLiteral("X 轴"), QStringLiteral("Ready"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("yAxisStatus"), QStringLiteral("Y 轴"), QStringLiteral("Ready"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("lightStatus"), QStringLiteral("光源"), QStringLiteral("78%"), LedIndicatorWidget::State::Warn));
    layout->addWidget(leftWrap);

    auto *divider = new QWidget;
    divider->setObjectName(QStringLiteral("statusDivider"));
    divider->setFixedSize(1, 22);
    divider->setStyleSheet(QStringLiteral("background:%1;").arg(Theme::palette().border.name()));
    layout->addWidget(divider);

    auto *progressWrap = new QWidget;
    auto *progressLayout = new QHBoxLayout(progressWrap);
    progressLayout->setContentsMargins(0, 0, 0, 0);
    progressLayout->setSpacing(4);
    progressWrap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_progressTitleLabel = new QLabel(QStringLiteral("测量进度"));
    m_progressTitleLabel->setStyleSheet(QStringLiteral("font-size:10.5px;font-weight:600;letter-spacing:0.5px;color:%1;").arg(Theme::palette().textMuted.name()));
    {
        QFont tightFont(m_progressTitleLabel->font());
        tightFont.setPointSizeF(10.5);
        tightFont.setWeight(QFont::DemiBold);
        QFontMetrics fm(tightFont);
        m_progressTitleLabel->setFixedWidth(fm.horizontalAdvance(QStringLiteral("测量进度")) + 2);
    }

    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(false);
    m_progressBar->setMinimumWidth(160);
    m_progressBar->setMaximumWidth(220);
    m_progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_percentLabel = new QLabel(QStringLiteral("0%"));
    m_percentLabel->setObjectName(QStringLiteral("statusPercentLabel"));
    m_percentLabel->setMinimumWidth(34);
    m_percentLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;font-family:Consolas;").arg(Theme::palette().text1.name()));

    m_stateLed = new StatusLedWidget(progressWrap);
    m_stateLed->setObjectName(QStringLiteral("statusStateLed"));

    m_stateLabel = new QLabel(QStringLiteral("待机"));
    m_stateLabel->setObjectName(QStringLiteral("statusStateLabel"));
    {
        QFont stateFont(QStringLiteral("Consolas"), 9, QFont::Bold);
        QFontMetrics fm(stateFont);
        const int maxW = qMax(fm.horizontalAdvance(QStringLiteral("待机")),
                              fm.horizontalAdvance(QStringLiteral("测量中")));
        m_stateLabel->setFixedWidth(maxW + 2);
        m_stateLabel->setStyleSheet(QStringLiteral("font-family:Consolas;font-size:9px;font-weight:bold;color:%1;").arg(Theme::palette().textMuted.name()));
    }

    progressLayout->addWidget(m_progressTitleLabel);
    progressLayout->addWidget(m_progressBar);
    progressLayout->addWidget(m_percentLabel);
    progressLayout->addWidget(m_stateLed);
    progressLayout->addWidget(m_stateLabel);
    layout->addWidget(progressWrap, 1);

    auto *actionWrap = new QWidget;
    actionWrap->setFixedWidth(200);
    auto *actionLayout = new QHBoxLayout(actionWrap);
    actionLayout->setContentsMargins(0, 0, 0, 0);
    actionLayout->setSpacing(6);

    auto *startButton = new QPushButton(QStringLiteral("开始测量"));
    startButton->setObjectName(QStringLiteral("statusStartButton"));
    startButton->setProperty("role", QStringLiteral("primary"));
    startButton->setFixedHeight(22);
    startButton->setFixedWidth(80);
    m_startButton = startButton;

    auto *manualButton = new QPushButton(QStringLiteral("手动采样"));
    manualButton->setObjectName(QStringLiteral("statusManualButton"));
    manualButton->setFixedHeight(22);
    manualButton->setFixedWidth(80);
    m_manualButton = manualButton;

    auto *stopButton = new QPushButton(QStringLiteral("停止"));
    stopButton->setObjectName(QStringLiteral("statusStopButton"));
    stopButton->setFixedHeight(22);
    stopButton->setFixedWidth(80);
    m_stopButton = stopButton;

    connect(startButton, &QPushButton::clicked, this, &DeviceStatusBar::startRequested);
    connect(manualButton, &QPushButton::clicked, this, &DeviceStatusBar::manualRequested);
    connect(stopButton, &QPushButton::clicked, this, &DeviceStatusBar::stopRequested);

    actionLayout->addWidget(startButton);
    actionLayout->addWidget(manualButton);
    actionLayout->addWidget(stopButton);
    stopButton->hide();
    layout->addWidget(actionWrap);

 setStyleSheet(QStringLiteral(
 "QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:0 8px;min-height:22px;max-height:22px;color:%3;font-size:11px;}"
 "QPushButton:hover{background:%4;}"
  "QPushButton[role='primary']{background:%5;border-color:%5;color:%9;}"
  "QPushButton#statusStopButton{background:%6;border-color:%7;color:%8;}"
  )
  .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text1.name(), Theme::palette().bgSunken.name(), Theme::palette().brand.name(),
  Theme::palette().errWeak.name(), Theme::palette().errWeak.darker(115).name(), Theme::palette().err.name(), Theme::palette().bgPanel.name()));
}

void DeviceStatusBar::setProgress(double progress)
{
    m_progressBar->setValue(static_cast<int>(progress));
    m_percentLabel->setText(QStringLiteral("%1%").arg(static_cast<int>(progress)));
}

void DeviceStatusBar::setMeasuring(bool measuring)
{
    if (auto *led = dynamic_cast<StatusLedWidget*>(m_stateLed)) {
        led->setMeasuring(measuring);
    }

    if (m_stateLabel) {
        m_stateLabel->setText(measuring ? QStringLiteral("测量中") : QStringLiteral("待机"));
        m_stateLabel->setStyleSheet(QStringLiteral("font-family:Consolas;font-size:9px;font-weight:bold;color:%1;")
            .arg(measuring ? Theme::palette().ok.name() : Theme::palette().err.name()));
    }

    if (m_startButton) m_startButton->setVisible(!measuring);
    if (m_manualButton) m_manualButton->setVisible(!measuring);
    if (m_stopButton) m_stopButton->setVisible(measuring);

    m_progressBar->setStyleSheet(measuring
        ? QStringLiteral("QProgressBar{background:%1;border:none;border-radius:3px;height:6px;}QProgressBar::chunk{background:%2;border-radius:3px;}")
            .arg(Theme::palette().bgSunken.name(), Theme::palette().brand.name())
        : QStringLiteral("QProgressBar{background:%1;border:none;border-radius:3px;height:6px;}QProgressBar::chunk{background:%2;border-radius:3px;}")
            .arg(Theme::palette().bgSunken.name(), Theme::palette().ok.name()));
}
