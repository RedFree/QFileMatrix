#include "panels/ServoControlPanel.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QVBoxLayout>

#include "theme/Theme.h"
#include "widgets/DpadControlWidget.h"
#include "widgets/PanelHeaderWidget.h"

ServoControlPanel::ServoControlPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("servoPanel"));
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *header = new PanelHeaderWidget(QStringLiteral("伺服 / 点动"));
    auto *homeButton = new QPushButton(QStringLiteral("回原点"));
    homeButton->setObjectName(QStringLiteral("servoHomeButton"));
    homeButton->setFixedHeight(22);
    connect(homeButton, &QPushButton::clicked, this, &ServoControlPanel::homeRequested);
    header->rightLayout()->addWidget(homeButton);
    layout->addWidget(header);

    auto *body = new QVBoxLayout;
    body->setContentsMargins(10, 10, 10, 10);
    body->setSpacing(8);

    m_dpad = new DpadControlWidget;
    body->addWidget(m_dpad);
    connect(m_dpad, &DpadControlWidget::jogRequested, this, &ServoControlPanel::jogRequested);
    connect(m_dpad, &DpadControlWidget::homeRequested, this, &ServoControlPanel::homeRequested);

    auto *stepRow = new QHBoxLayout;
    stepRow->setContentsMargins(0, 0, 0, 0);
    stepRow->setSpacing(6);
    auto *stepLabel = new QLabel(QStringLiteral("步长"));
    stepLabel->setStyleSheet(QStringLiteral("font-size:10px;color:%1;letter-spacing:1px;font-weight:600;").arg(Theme::palette().textMuted.name()));
    stepRow->addWidget(stepLabel);
    m_stepSpin = new QSpinBox;
    m_stepSpin->setRange(1, 1000000);
    m_stepSpin->setValue(200);
    m_stepSpin->setFixedHeight(26);
    stepRow->addWidget(m_stepSpin, 1);
    body->addLayout(stepRow);
    connect(m_stepSpin, &QSpinBox::valueChanged, this, &ServoControlPanel::stepChanged);

m_jogHintLabel = nullptr;
auto *hintRow = new QHBoxLayout;
hintRow->setContentsMargins(0, 0, 0, 0);
hintRow->setSpacing(4);
const auto kbdStyle = QStringLiteral("QLabel{min-width:18px;height:18px;border:1px solid #C9D0DA;border-bottom:2px solid #C9D0DA;border-radius:4px;background:%1;color:%2;font:500 10px Consolas;qproperty-alignment:AlignCenter;padding:0 4px;}")
.arg(Theme::palette().bgRail.name(), Theme::palette().textMuted.name());
const auto hintSpanStyle = QStringLiteral("font-size:10px;color:%1;font-family:Consolas;").arg(Theme::palette().textMuted.name());
for (const auto &key : {QStringLiteral("↑"), QStringLiteral("↓"), QStringLiteral("←"), QStringLiteral("→")}) {
auto *kbd = new QLabel(key);
kbd->setStyleSheet(kbdStyle);
kbd->setFixedHeight(18);
hintRow->addWidget(kbd);
}
auto *jogSpan = new QLabel(QStringLiteral("JOG"));
jogSpan->setStyleSheet(hintSpanStyle);
hintRow->addWidget(jogSpan);
hintRow->addStretch();
auto *hKbd = new QLabel(QStringLiteral("H"));
hKbd->setStyleSheet(kbdStyle);
hKbd->setFixedHeight(18);
hintRow->addWidget(hKbd);
auto *homeSpan = new QLabel(QStringLiteral("HOME"));
homeSpan->setObjectName(QStringLiteral("servoJogHintLabel"));
homeSpan->setStyleSheet(hintSpanStyle);
hintRow->addWidget(homeSpan);
body->addLayout(hintRow);

    auto *gotoWrap = new QWidget;
    auto *gotoLayout = new QGridLayout(gotoWrap);
    gotoLayout->setContentsMargins(0, 8, 0, 0);
    gotoLayout->setHorizontalSpacing(6);
    gotoLayout->setVerticalSpacing(6);
    gotoWrap->setStyleSheet(QStringLiteral("border-top:1px dashed %1;").arg(Theme::palette().divider.name()));

    auto *gotoXLabel = new QLabel(QStringLiteral("GOTO X"));
    auto *gotoYLabel = new QLabel(QStringLiteral("GOTO Y"));
    gotoXLabel->setStyleSheet(QStringLiteral("font-size:10px;color:%1;letter-spacing:1px;font-weight:600;").arg(Theme::palette().textMuted.name()));
    gotoYLabel->setStyleSheet(QStringLiteral("font-size:10px;color:%1;letter-spacing:1px;font-weight:600;").arg(Theme::palette().textMuted.name()));

    m_xSpin = new QSpinBox;
    m_ySpin = new QSpinBox;
    m_xSpin->setRange(0, 1000000);
    m_ySpin->setRange(0, 1000000);
    m_xSpin->setValue(10000);
    m_ySpin->setValue(10000);
    m_xSpin->setFixedHeight(26);
    m_ySpin->setFixedHeight(26);
    auto *moveButton = new QPushButton(QStringLiteral("移动"));
    moveButton->setObjectName(QStringLiteral("servoMoveButton"));
    moveButton->setProperty("role", QStringLiteral("primary"));
    moveButton->setFixedHeight(24);
    gotoLayout->addWidget(gotoXLabel, 0, 0);
    gotoLayout->addWidget(gotoYLabel, 0, 1);
    gotoLayout->addWidget(m_xSpin, 1, 0);
    gotoLayout->addWidget(m_ySpin, 1, 1);
    gotoLayout->addWidget(moveButton, 2, 0, 1, 2);
    body->addWidget(gotoWrap);
    connect(moveButton, &QPushButton::clicked, this, [this] { emit gotoRequested(m_xSpin->value(), m_ySpin->value()); });

    auto *zHomeCheck = new QCheckBox(QStringLiteral("Z 轴回零"));
    zHomeCheck->setObjectName(QStringLiteral("servoZHomeCheck"));
    body->addWidget(zHomeCheck);

    layout->addLayout(body);

    setStyleSheet(Theme::fieldStyle() + QStringLiteral(
        "QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:0 10px;color:%3;font-size:11px;}"
        "QPushButton#servoHomeButton{background:transparent;border:none;color:%4;padding:0 8px;}"
        "QPushButton#servoHomeButton:hover{background:%5;color:%3;}"
        "QPushButton[role='primary'] {background:%4;border-color:%4;color:white;}"
        )
        .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text.name(), Theme::palette().brand.name(), Theme::palette().bgSunken.name()));
}

void ServoControlPanel::setPosition(const MachinePosition &position)
{
    m_dpad->setPosition(position);
}

void ServoControlPanel::setStep(int step)
{
    if (m_stepSpin->value() != step) {
        const QSignalBlocker blocker(m_stepSpin);
        m_stepSpin->setValue(step);
    }
    m_dpad->setStep(step);
}
