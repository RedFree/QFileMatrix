#include "panels/MeasureControlPanel.h"

#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QVBoxLayout>

#include "theme/Theme.h"
#include "widgets/PanelHeaderWidget.h"

MeasureControlPanel::MeasureControlPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("measureControlPanel"));
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *header = new PanelHeaderWidget(QStringLiteral("测量控制"));
    auto *autoButton = new QPushButton(QStringLiteral("全自动"));
    autoButton->setObjectName(QStringLiteral("autoModeButton"));
    autoButton->setFixedHeight(22);
    auto *singleButton = new QPushButton(QStringLiteral("单点"));
    singleButton->setObjectName(QStringLiteral("singleModeButton"));
    singleButton->setFixedHeight(22);
    const auto modeStyle = [](bool active) {
if (active) {
return QStringLiteral("QPushButton{background:transparent;border:none;border-bottom:2px solid #5070D7;color:#5070D7;font-size:11px;font-weight:600;padding:0 8px;min-height:22px;max-height:22px;border-radius:4px 4px 0 0;}");
        }
        return QStringLiteral("QPushButton{background:transparent;border:none;color:%1;font-size:11px;padding:0 8px;min-height:22px;max-height:22px;}"
            "QPushButton:hover{color:%2;}")
            .arg(Theme::palette().textMuted.name(), Theme::palette().text.name());
    };
    autoButton->setProperty("active", true);
    singleButton->setProperty("active", false);
    autoButton->setStyleSheet(modeStyle(true));
    singleButton->setStyleSheet(modeStyle(false));
    const auto applyModeState = [autoButton, singleButton, modeStyle](QPushButton *activeButton) {
        const bool autoActive = activeButton == autoButton;
        autoButton->setProperty("active", autoActive);
        singleButton->setProperty("active", !autoActive);
        autoButton->setStyleSheet(modeStyle(autoActive));
        singleButton->setStyleSheet(modeStyle(!autoActive));
    };
    connect(autoButton, &QPushButton::clicked, this, [applyModeState, autoButton] { applyModeState(autoButton); });
    connect(singleButton, &QPushButton::clicked, this, [applyModeState, singleButton] { applyModeState(singleButton); });
    header->rightLayout()->addWidget(autoButton);
    header->rightLayout()->addWidget(singleButton);
    layout->addWidget(header);

    auto *body = new QVBoxLayout;
    body->setContentsMargins(10, 10, 10, 10);
    body->setSpacing(8);

    m_stateLabel = new QLabel(QStringLiteral("待机"));
    m_stateLabel->setStyleSheet(QStringLiteral("QLabel{background:#F3F5F8;border:1px solid #E2E6EC;border-radius:10px;padding:2px 8px;color:#596579;font-size:11px;font-weight:600;font-family:Consolas;}"));
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedHeight(8);
    body->addWidget(m_stateLabel);
    body->addWidget(m_progressBar);

    m_autoSampleCheck = new QCheckBox(QStringLiteral("自动采样"));
    m_autoSampleCheck->setObjectName(QStringLiteral("autoSampleCheck"));
    m_autoSampleCheck->setChecked(true);
    m_cycleTestCheck = new QCheckBox(QStringLiteral("循环测试"));
    m_cycleTestCheck->setObjectName(QStringLiteral("cycleTestCheck"));
    body->addWidget(m_autoSampleCheck);

    auto *sampleRow = new QHBoxLayout;
    sampleRow->addWidget(m_cycleTestCheck);
    m_sampleCount = new QSpinBox;
    m_sampleCount->setRange(1, 999);
    m_sampleCount->setValue(5);
    m_sampleCount->setStyleSheet(QStringLiteral("QSpinBox{font-family:Consolas;font-size:11px;border:1px solid %1;border-radius:4px;padding:0 4px;background:%2;color:%3;text-align:right;min-width:50px;max-height:22px;}"
        "QSpinBox::up-button,QSpinBox::down-button{width:16px;border:none;background:transparent;}")
        .arg(Theme::palette().border.name(), Theme::palette().bgPanel.name(), Theme::palette().text.name()));
    sampleRow->addWidget(m_sampleCount);
    auto *unitLabel = new QLabel(QStringLiteral("次"));
    unitLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;").arg(Theme::palette().textMuted.name()));
    sampleRow->addWidget(unitLabel);
    sampleRow->addStretch();
    body->addLayout(sampleRow);
    connect(m_sampleCount, &QSpinBox::valueChanged, this, &MeasureControlPanel::sampleCountChanged);

    auto *buttons = new QGridLayout;
    buttons->setHorizontalSpacing(6);
    buttons->setVerticalSpacing(6);
    auto *startButton = new QPushButton(QStringLiteral("开始测量"));
    startButton->setObjectName(QStringLiteral("startButton"));
    startButton->setProperty("role", QStringLiteral("primary"));
    startButton->setFixedHeight(28);
    auto *manualButton = new QPushButton(QStringLiteral("手动采样"));
    manualButton->setObjectName(QStringLiteral("manualButton"));
    manualButton->setFixedHeight(28);
    auto *stopButton = new QPushButton(QStringLiteral("停止测量"));
    stopButton->setObjectName(QStringLiteral("stopButton"));
    stopButton->setProperty("role", QStringLiteral("danger"));
    stopButton->setFixedHeight(28);
    buttons->addWidget(startButton, 0, 0);
    buttons->addWidget(manualButton, 0, 1);
    buttons->addWidget(stopButton, 1, 0, 1, 2);
    body->addLayout(buttons);

    connect(startButton, &QPushButton::clicked, this, &MeasureControlPanel::startRequested);
    connect(manualButton, &QPushButton::clicked, this, &MeasureControlPanel::manualRequested);
    connect(stopButton, &QPushButton::clicked, this, &MeasureControlPanel::stopRequested);

    layout->addLayout(body);

    setStyleSheet(Theme::fieldStyle() + QStringLiteral(
        "QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:0 10px;color:%3;font-size:11px;}"
        "QPushButton[role='primary'] {background:%4;border-color:%4;color:white;}"
        "QPushButton[role='danger'] {background:#FFF4F1;border-color:#F1D0C5;color:#A54E2F;}"
        "QProgressBar{background:%5;border:1px solid %2;border-radius:4px;height:6px;}"
        "QProgressBar::chunk{background:%4;border-radius:4px;}"
        )
        .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text.name(), Theme::palette().brand.name(), Theme::palette().bgSunken.name()));
}

void MeasureControlPanel::setProgress(double progress)
{
    m_progressBar->setValue(static_cast<int>(progress));
}

void MeasureControlPanel::setMeasuring(bool measuring)
{
    m_stateLabel->setText(measuring ? QStringLiteral("测量中") : QStringLiteral("待机"));
    m_stateLabel->setStyleSheet(measuring
        ? QStringLiteral("QLabel{background:#EEF2FF;border:1px solid #D9E3FF;border-radius:10px;padding:2px 8px;color:#3550A8;font-size:11px;font-weight:600;font-family:Consolas;}")
        : QStringLiteral("QLabel{background:#F3F5F8;border:1px solid #E2E6EC;border-radius:10px;padding:2px 8px;color:#596579;font-size:11px;font-weight:600;font-family:Consolas;}"));
}

void MeasureControlPanel::setSampleCount(int sampleCount)
{
    if (m_sampleCount->value() == sampleCount) {
        return;
    }

    const QSignalBlocker blocker(m_sampleCount);
    m_sampleCount->setValue(sampleCount);
}
