#include "panels/ServoControlPanel.h"

#include <cmath>

#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QVBoxLayout>

#include "theme/Theme.h"
#include "widgets/DpadControlWidget.h"
#include "widgets/PanelHeaderWidget.h"

namespace {
class ServoIconButton : public QPushButton
{
public:
    explicit ServoIconButton(const QString &text, QWidget *parent = nullptr)
        : QPushButton(text, parent)
    {
        setFixedHeight(22);
        setCursor(Qt::PointingHandCursor);
        setMouseTracking(true);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPushButton::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QColor iconColor = isDown() ? Theme::palette().brandStrong
            : (underMouse() ? Theme::palette().brand : Theme::palette().textMuted);
        QPen pen(iconColor);
        pen.setWidthF(1.4);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        const qreal cx = 11, cy = height() / 2.0;
        const qreal r = 4.5;
        painter.drawArc(QRectF(cx - r, cy - r, r * 2, r * 2), -30 * 16, -280 * 16);
        const qreal angle = -30.0 * M_PI / 180.0;
        const qreal tipX = cx + r * std::cos(angle);
        const qreal tipY = cy - r * std::sin(angle);
        const qreal arrLen = 3.0;
        painter.drawLine(QPointF(tipX, tipY), QPointF(tipX - arrLen, tipY - arrLen));
        painter.drawLine(QPointF(tipX, tipY), QPointF(tipX + arrLen * 0.3, tipY - arrLen));
    }
};
}

ServoControlPanel::ServoControlPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("servoPanel"));
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *header = new PanelHeaderWidget(QStringLiteral("伺服 / 点动"));
 auto *homeButton = new ServoIconButton(QStringLiteral("回原点"));
 homeButton->setObjectName(QStringLiteral("servoHomeButton"));
    connect(homeButton, &QPushButton::clicked, this, &ServoControlPanel::homeRequested);
    header->rightLayout()->addWidget(homeButton);
    layout->addWidget(header);

    auto *body = new QVBoxLayout;
    body->setContentsMargins(8, 8, 8, 8);
    body->setSpacing(6);

    m_dpad = new DpadControlWidget;
    body->addWidget(m_dpad);
    connect(m_dpad, &DpadControlWidget::jogRequested, this, &ServoControlPanel::jogRequested);
    connect(m_dpad, &DpadControlWidget::homeRequested, this, &ServoControlPanel::homeRequested);

    auto *stepRow = new QHBoxLayout;
    stepRow->setContentsMargins(0, 0, 0, 0);
    stepRow->setSpacing(6);
    auto *stepLabel = new QLabel(QStringLiteral("步长"));
    stepLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;letter-spacing:1px;font-weight:600;").arg(Theme::palette().textMuted.name()));
    stepRow->addWidget(stepLabel);
    m_stepSpin = new QSpinBox;
    m_stepSpin->setRange(1, 1000000);
    m_stepSpin->setValue(200);
    m_stepSpin->setFixedHeight(24);
    stepRow->addWidget(m_stepSpin, 1);
    body->addLayout(stepRow);
    connect(m_stepSpin, &QSpinBox::valueChanged, this, &ServoControlPanel::stepChanged);

m_jogHintLabel = nullptr;
auto *hintRow = new QHBoxLayout;
hintRow->setContentsMargins(0, 0, 0, 0);
hintRow->setSpacing(4);
  const auto kbdStyle = QStringLiteral("QLabel{min-width:18px;height:18px;border:1px solid %1;border-bottom:2px solid %2;border-radius:4px;background:%3;color:%2;font:700 11px Consolas;padding:0 4px;}")
    .arg(Theme::withAlpha(Theme::palette().brand, 120).name(QColor::HexArgb), Theme::palette().brand.name(), Theme::withAlpha(Theme::palette().brand, 24).name(QColor::HexArgb));
const auto hintSpanStyle = QStringLiteral("font-size:11px;color:%1;font-family:Consolas;font-weight:600;").arg(Theme::palette().gaugeHighlight.name());
for (const auto &key : {QStringLiteral("↑"), QStringLiteral("↓"), QStringLiteral("←"), QStringLiteral("→")}) {
auto *kbd = new QLabel(key);
kbd->setStyleSheet(kbdStyle);
kbd->setFixedHeight(18);
kbd->setAlignment(Qt::AlignCenter);
hintRow->addWidget(kbd);
}
auto *jogSpan = new QLabel(QStringLiteral("JOG"));
jogSpan->setStyleSheet(hintSpanStyle);
hintRow->addWidget(jogSpan);
hintRow->addStretch();
auto *hKbd = new QLabel(QStringLiteral("H"));
hKbd->setStyleSheet(kbdStyle);
hKbd->setFixedHeight(18);
hKbd->setAlignment(Qt::AlignCenter);
hintRow->addWidget(hKbd);
auto *homeSpan = new QLabel(QStringLiteral("HOME"));
homeSpan->setObjectName(QStringLiteral("servoJogHintLabel"));
homeSpan->setStyleSheet(hintSpanStyle);
hintRow->addWidget(homeSpan);
body->addLayout(hintRow);

    auto *gotoWrap = new QWidget;
    auto *gotoLayout = new QGridLayout(gotoWrap);
    gotoLayout->setContentsMargins(0, 6, 0, 0);
    gotoLayout->setHorizontalSpacing(6);
    gotoLayout->setVerticalSpacing(6);
    gotoWrap->setStyleSheet(QStringLiteral("border-top:1px dashed %1;").arg(Theme::palette().divider.name()));

    auto *gotoXLabel = new QLabel(QStringLiteral("GOTO X"));
    auto *gotoYLabel = new QLabel(QStringLiteral("GOTO Y"));
    gotoXLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;letter-spacing:1px;font-weight:600;").arg(Theme::palette().textMuted.name()));
    gotoYLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;letter-spacing:1px;font-weight:600;").arg(Theme::palette().textMuted.name()));

    m_xSpin = new QSpinBox;
    m_ySpin = new QSpinBox;
    m_xSpin->setRange(0, 1000000);
    m_ySpin->setRange(0, 1000000);
    m_xSpin->setValue(10000);
    m_ySpin->setValue(10000);
    m_xSpin->setFixedHeight(24);
    m_ySpin->setFixedHeight(24);
    auto *moveButton = new QPushButton(QStringLiteral("移动"));
    moveButton->setObjectName(QStringLiteral("servoMoveButton"));
    moveButton->setProperty("role", QStringLiteral("primary"));
    moveButton->setFixedHeight(22);
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

    const auto &p = Theme::palette();
    const QString bgPanel       = p.bgPanel.name();
    const QString border        = p.border.name();
    const QString text          = p.text.name();
    const QString brand         = p.brand.name();
    const QString brandDark     = p.brandStrong.name();
    const QString brandAlpha20  = Theme::withAlpha(p.brand, 20).name(QColor::HexArgb);
    const QString brandAlpha26  = Theme::withAlpha(p.brand, 26).name(QColor::HexArgb);
    const QString brandAlpha45  = Theme::withAlpha(p.brand, 45).name(QColor::HexArgb);
    const QString brandAlpha80  = Theme::withAlpha(p.brand, 80).name(QColor::HexArgb);
    const QString brandAlpha135 = Theme::withAlpha(p.brand, 135).name(QColor::HexArgb);
    const QString gaugeHighlight = p.gaugeHighlight.name();
    const QString white         = p.bgPanel.name();

    setStyleSheet(
        QStringLiteral("background:%1;border:none;border-bottom:1px solid %2;border-radius:0;").arg(bgPanel, border) +
        Theme::fieldStyle() +
        QStringLiteral("QCheckBox{color:%1;font-size:11px;}").arg(text) +
        QStringLiteral("QCheckBox::indicator{width:12px;height:12px;border:1px solid %1;border-radius:3px;background:%2;}").arg(border, bgPanel) +
        QStringLiteral("QCheckBox::indicator:checked{background:%1;border-color:%1;}").arg(brand) +
        QStringLiteral("QPushButton{background:%1;border:1px solid %2;border-bottom:2px solid %3;border-radius:4px;padding:0 10px;color:%4;font-size:11px;font-weight:600;}").arg(bgPanel, border, brandAlpha45, text) +
        QStringLiteral("QPushButton:hover{background:%1;border-color:%2;border-bottom-color:%2;color:%2;}").arg(brandAlpha26, brand) +
        QStringLiteral("QPushButton:pressed{background:%1;border-color:%2;border-bottom:1px solid %2;padding-top:1px;color:%2;}").arg(brandAlpha45, brand) +
        QStringLiteral("QPushButton#servoHomeButton{background:%1;border:1px solid %2;border-bottom:1px solid %2;color:%3;padding:0 8px 0 18px;}").arg(brandAlpha20, brandAlpha135, brand) +
        QStringLiteral("QPushButton#servoHomeButton:hover{background:%1;border-bottom:2px solid %2;color:%3;}").arg(brandAlpha26, brand, gaugeHighlight) +
        QStringLiteral("QPushButton#servoHomeButton:pressed{background:%1;border-bottom:1px solid %2;padding-top:1px;color:%3;}").arg(brandAlpha45, brand, gaugeHighlight) +
        QStringLiteral("QPushButton#servoMoveButton{background:qlineargradient(x1:0,y1=0,x2:0,y2=1,stop:0 %1,stop:1 %2);border-color:%2;border-bottom:2px solid %3;color:%4;}").arg(brandAlpha80, brand, brandDark, white) +
        QStringLiteral("QPushButton#servoMoveButton:hover{background:%1;border-color:%1;border-bottom:2px solid %2;color:%3;}").arg(gaugeHighlight, brandDark, white) +
        QStringLiteral("QPushButton#servoMoveButton:pressed{background:%1;border-color:%1;border-bottom:1px solid %1;padding-top:1px;color:%2;}").arg(brandDark, white) +
        QStringLiteral("QSpinBox{font-family:Consolas;font-size:11px;border:1px solid %1;border-radius:4px;padding:0 4px;background:%2;color:%3;}").arg(brandAlpha135, bgPanel, text) +
        QStringLiteral("QSpinBox:focus{border:1px solid %1;background:%2;}").arg(brand, brandAlpha20) +
        QStringLiteral("QSpinBox::up-button,QSpinBox::down-button{width:16px;border:none;background:transparent;}"));
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
