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

MeasureControlPanel::MeasureControlPanel(QWidget *parent)
: QWidget(parent)
{
    setObjectName(QStringLiteral("measureControlPanel"));
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    auto *headRow = new QHBoxLayout;
    headRow->setContentsMargins(0, 0, 0, 0);
    headRow->setSpacing(6);
    auto *title = new QLabel(QStringLiteral("测量控制"));
    title->setStyleSheet(QStringLiteral("font-size:12px;font-weight:600;color:%1;").arg(Theme::palette().text.name()));
    headRow->addWidget(title);
    headRow->addStretch();

    auto *modeRow = new QHBoxLayout;
    modeRow->setContentsMargins(0, 0, 0, 0);
    modeRow->setSpacing(4);
    auto *autoButton = new QPushButton(QStringLiteral("全自动"));
    autoButton->setObjectName(QStringLiteral("autoModeButton"));
    autoButton->setFixedHeight(22);
    auto *singleButton = new QPushButton(QStringLiteral("单点"));
    singleButton->setObjectName(QStringLiteral("singleModeButton"));
    singleButton->setFixedHeight(22);
    const auto modeStyle = [](bool active) {
        if (active) {
            return QStringLiteral("QPushButton{background:transparent;border:none;border-bottom:2px solid #5070D7;color:#5070D7;font-size:11px;font-weight:600;padding:0 8px;min-height:22px;max-height:22px;}");
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
    modeRow->addWidget(autoButton);
    modeRow->addWidget(singleButton);
    headRow->addLayout(modeRow, 1);
    layout->addLayout(headRow);

    m_stateLabel = new QLabel(QStringLiteral("待机"));
    m_stateLabel->setStyleSheet(QStringLiteral("QLabel{background:#F3F5F8;border:1px solid #E2E6EC;border-radius:10px;padding:2px 8px;color:#596579;font-size:11px;font-weight:600;}"));
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedHeight(8);
    layout->addWidget(m_stateLabel);
    layout->addWidget(m_progressBar);

    m_autoSampleCheck = new QCheckBox(QStringLiteral("自动采样"));
    m_autoSampleCheck->setObjectName(QStringLiteral("autoSampleCheck"));
    m_autoSampleCheck->setChecked(true);
    m_cycleTestCheck = new QCheckBox(QStringLiteral("循环测试"));
    m_cycleTestCheck->setObjectName(QStringLiteral("cycleTestCheck"));
    layout->addWidget(m_autoSampleCheck);

    auto *sampleRow = new QHBoxLayout;
    sampleRow->addWidget(m_cycleTestCheck);
    m_sampleCount = new QSpinBox;
    m_sampleCount->setRange(1, 999);
    m_sampleCount->setValue(5);
    sampleRow->addWidget(m_sampleCount);
    sampleRow->addWidget(new QLabel(QStringLiteral("次")));
    sampleRow->addStretch();
    layout->addLayout(sampleRow);
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
    auto *stopButton = new QPushButton(QStringLiteral("停止"));
    stopButton->setObjectName(QStringLiteral("stopButton"));
    stopButton->setProperty("role", QStringLiteral("danger"));
    stopButton->setFixedHeight(28);
    buttons->addWidget(startButton, 0, 0);
    buttons->addWidget(manualButton, 0, 1);
    buttons->addWidget(stopButton, 1, 0, 1, 2);
    layout->addLayout(buttons);

    connect(startButton, &QPushButton::clicked, this, &MeasureControlPanel::startRequested);
    connect(manualButton, &QPushButton::clicked, this, &MeasureControlPanel::manualRequested);
    connect(stopButton, &QPushButton::clicked, this, &MeasureControlPanel::stopRequested);

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
                                    ? QStringLiteral("QLabel{background:#EEF2FF;border:1px solid #D9E3FF;border-radius:10px;padding:2px 8px;color:#3550A8;font-size:11px;font-weight:600;}")
                                    : QStringLiteral("QLabel{background:#F3F5F8;border:1px solid #E2E6EC;border-radius:10px;padding:2px 8px;color:#596579;font-size:11px;font-weight:600;}"));
}

void MeasureControlPanel::setSampleCount(int sampleCount)
{
    if (m_sampleCount->value() == sampleCount) {
        return;
    }

    const QSignalBlocker blocker(m_sampleCount);
    m_sampleCount->setValue(sampleCount);
}
