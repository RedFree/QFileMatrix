#include "panels/DeviceStatusBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

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
    return indicator;
}
}

DeviceStatusBar::DeviceStatusBar(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(18);

    auto *leftWrap = new QWidget;
    auto *leftLayout = new QHBoxLayout(leftWrap);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(10);
    leftLayout->addWidget(makeStatus(QStringLiteral("commStatus"), QStringLiteral("通讯"), QStringLiteral("TCP 192.168.1.10"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("xAxisStatus"), QStringLiteral("X 轴"), QStringLiteral("Ready"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("yAxisStatus"), QStringLiteral("Y 轴"), QStringLiteral("Ready"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("zAxisStatus"), QStringLiteral("Z 轴"), QStringLiteral("Ready"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("airStatus"), QStringLiteral("气压"), QStringLiteral("0.62 MPa"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("lightStatus"), QStringLiteral("光源"), QStringLiteral("Aging 78%"), LedIndicatorWidget::State::Warn));
    leftLayout->addWidget(makeStatus(QStringLiteral("emergencyStatus"), QStringLiteral("急停"), QStringLiteral("Released"), LedIndicatorWidget::State::Off));
    layout->addWidget(leftWrap);

    layout->addStretch();

    auto *progressWrap = new QWidget;
    auto *progressLayout = new QHBoxLayout(progressWrap);
    progressLayout->setContentsMargins(0, 0, 0, 0);
    progressLayout->setSpacing(10);
    m_progressTitleLabel = new QLabel(QStringLiteral("测量进度"));
    m_progressTitleLabel->setStyleSheet(QStringLiteral("font-size:10px;font-weight:600;letter-spacing:1px;color:%1;").arg(Theme::palette().textMuted.name()));
    m_stateLabel = new QLabel(QStringLiteral("待机"));
    m_stateLabel->setObjectName(QStringLiteral("statusStateLabel"));
    m_stateLabel->setStyleSheet(QStringLiteral("QLabel{background:#EEF2FF;border:1px solid #D9E3FF;border-radius:10px;padding:2px 10px;color:#3550A8;font-size:11px;font-weight:600;}"));
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedWidth(280);
    m_percentLabel = new QLabel(QStringLiteral("0%"));
    m_percentLabel->setObjectName(QStringLiteral("statusPercentLabel"));
    m_percentLabel->setMinimumWidth(40);
    m_percentLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;").arg(Theme::palette().text.name()));

    progressLayout->addWidget(m_progressTitleLabel);
    progressLayout->addWidget(m_progressBar);
    progressLayout->addWidget(m_percentLabel);
    progressLayout->addWidget(m_stateLabel);
    layout->addWidget(progressWrap);

    auto *actionWrap = new QWidget;
    auto *actionLayout = new QHBoxLayout(actionWrap);
    actionLayout->setContentsMargins(0, 0, 0, 0);
    actionLayout->setSpacing(6);

    auto *startButton = new QPushButton(QStringLiteral("开始测量"));
    startButton->setObjectName(QStringLiteral("statusStartButton"));
    startButton->setProperty("role", QStringLiteral("primary"));
    auto *manualButton = new QPushButton(QStringLiteral("手动采样"));
    manualButton->setObjectName(QStringLiteral("statusManualButton"));
    auto *stopButton = new QPushButton(QStringLiteral("停止"));
    stopButton->setObjectName(QStringLiteral("statusStopButton"));

    connect(startButton, &QPushButton::clicked, this, &DeviceStatusBar::startRequested);
    connect(manualButton, &QPushButton::clicked, this, &DeviceStatusBar::manualRequested);
    connect(stopButton, &QPushButton::clicked, this, &DeviceStatusBar::stopRequested);

    actionLayout->addWidget(startButton);
    actionLayout->addWidget(manualButton);
    actionLayout->addWidget(stopButton);
    layout->addWidget(actionWrap);

    setStyleSheet(QStringLiteral(
                      "QProgressBar{background:%1;border:1px solid %2;border-radius:4px;height:6px;}"
                      "QProgressBar::chunk{background:%3;border-radius:4px;}"
                      "QPushButton{background:%4;border:1px solid %2;border-radius:6px;padding:6px 10px;color:%5;font-size:11px;}"
                      "QPushButton:hover{background:%1;}"
                      "QPushButton[role='primary']{background:%3;border-color:%3;color:white;}"
                      "QPushButton#statusStopButton{background:#FFF4F1;border-color:#F1D0C5;color:#A54E2F;}"
                  )
                      .arg(Theme::palette().bgSunken.name(), Theme::palette().border.name(), Theme::palette().brand.name(), Theme::palette().bgPanel.name(), Theme::palette().text.name()));
}

void DeviceStatusBar::setProgress(double progress)
{
    m_progressBar->setValue(static_cast<int>(progress));
    m_percentLabel->setText(QStringLiteral("%1%").arg(static_cast<int>(progress)));
}

void DeviceStatusBar::setMeasuring(bool measuring)
{
    m_stateLabel->setText(measuring ? QStringLiteral("测量中") : QStringLiteral("待机"));
    m_stateLabel->setStyleSheet(measuring
                                    ? QStringLiteral("QLabel{background:#EEF2FF;border:1px solid #D9E3FF;border-radius:10px;padding:2px 10px;color:#3550A8;font-size:11px;font-weight:600;}")
                                    : QStringLiteral("QLabel{background:#F3F5F8;border:1px solid #E2E6EC;border-radius:10px;padding:2px 10px;color:#596579;font-size:11px;font-weight:600;}"));
}
