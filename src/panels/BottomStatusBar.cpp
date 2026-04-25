#include "panels/BottomStatusBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QPainter>

#include "theme/Theme.h"

namespace {
class LedBulb : public QWidget
{
public:
explicit LedBulb(const QColor &color, QWidget *parent = nullptr)
: QWidget(parent), m_color(color)
{
setFixedSize(9, 9);
setAttribute(Qt::WA_TranslucentBackground);
}
void setColor(const QColor &c) { m_color = c; update(); }
protected:
void paintEvent(QPaintEvent *) override
{
QPainter p(this);
p.setRenderHint(QPainter::Antialiasing);
p.setPen(Qt::NoPen);
p.setBrush(m_color);
p.drawEllipse(rect().adjusted(1, 1, -1, -1));
}
private:
QColor m_color;
};
} // namespace

static void setLedBulbColor(QWidget *led, const QColor &color)
{
if (auto *bulb = dynamic_cast<LedBulb*>(led))
bulb->setColor(color);
}

BottomStatusBar::BottomStatusBar(QWidget *parent)
: QWidget(parent)
{
    setFixedHeight(28);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 12, 0);
    layout->setSpacing(0);

    m_probeLabel = new QLabel;
    m_xLabel = new QLabel;
    m_yLabel = new QLabel;
    m_targetPointsLabel = new QLabel;
    m_collectedPointsLabel = new QLabel;
    m_triggerFramesLabel = new QLabel;
    m_sampleLabel = new QLabel;
    m_fpsLabel = new QLabel;
    m_connectionLabel = new QLabel;
    m_measureStateLabel = new QLabel;

    m_probeLabel->setObjectName(QStringLiteral("bottomProbeLabel"));
    m_xLabel->setObjectName(QStringLiteral("bottomXLabel"));
    m_yLabel->setObjectName(QStringLiteral("bottomYLabel"));
    m_targetPointsLabel->setObjectName(QStringLiteral("bottomTargetPointsLabel"));
    m_collectedPointsLabel->setObjectName(QStringLiteral("bottomCollectedPointsLabel"));
    m_triggerFramesLabel->setObjectName(QStringLiteral("bottomTriggerFramesLabel"));
    m_sampleLabel->setObjectName(QStringLiteral("bottomSampleLabel"));
    m_fpsLabel->setObjectName(QStringLiteral("bottomFpsLabel"));
    m_connectionLabel->setObjectName(QStringLiteral("bottomConnectionLabel"));
    m_measureStateLabel->setObjectName(QStringLiteral("bottomMeasureStateLabel"));

    const auto monoStyle = QStringLiteral("font-size:11px;font-family:Consolas;color:%1;").arg(Theme::palette().textMuted.name());
    const auto monoMutedStyle = QStringLiteral("font-size:11px;font-family:Consolas;color:%1;").arg(Theme::palette().textMuted.name());
    m_probeLabel->setStyleSheet(monoStyle);
    m_xLabel->setStyleSheet(monoStyle);
    m_yLabel->setStyleSheet(monoStyle);
    m_targetPointsLabel->setStyleSheet(monoStyle);
    m_collectedPointsLabel->setStyleSheet(monoStyle);
    m_triggerFramesLabel->setStyleSheet(monoStyle);
    m_sampleLabel->setStyleSheet(monoStyle);
    m_fpsLabel->setStyleSheet(monoStyle);
  m_connectionLabel->setStyleSheet(QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
    .arg(Theme::palette().okWeak.name(), Theme::palette().okWeak.darker(115).name(), Theme::palette().ok.name()));
  m_measureStateLabel->setStyleSheet(QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
    .arg(Theme::palette().bgSunken.name(), Theme::palette().border.name(), Theme::palette().textMuted.name()));

    m_probeLabel->setTextFormat(Qt::RichText);
    m_xLabel->setTextFormat(Qt::RichText);
    m_yLabel->setTextFormat(Qt::RichText);
    m_targetPointsLabel->setTextFormat(Qt::RichText);
    m_collectedPointsLabel->setTextFormat(Qt::RichText);
    m_triggerFramesLabel->setTextFormat(Qt::RichText);
    m_sampleLabel->setTextFormat(Qt::RichText);
    m_fpsLabel->setTextFormat(Qt::RichText);

    layout->addWidget(m_probeLabel);
    layout->addSpacing(16);
    layout->addWidget(m_xLabel);
    layout->addSpacing(16);
    layout->addWidget(m_yLabel);
    layout->addSpacing(16);
    layout->addWidget(m_targetPointsLabel);
    layout->addSpacing(16);
    layout->addWidget(m_collectedPointsLabel);
    layout->addSpacing(16);
    layout->addWidget(m_triggerFramesLabel);
    layout->addSpacing(16);
    layout->addWidget(m_sampleLabel);
    layout->addSpacing(16);
    layout->addWidget(m_fpsLabel);
layout->addSpacing(16);
  auto *led = new LedBulb(Theme::palette().ok);
m_connectionLed = led;
layout->addWidget(m_connectionLed);
layout->addSpacing(4);
layout->addWidget(m_connectionLabel);
layout->addSpacing(8);
    layout->addWidget(m_measureStateLabel);
    layout->addSpacing(8);

  const auto okPillStyle = QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 6px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
    .arg(Theme::palette().okWeak.name(), Theme::palette().okWeak.darker(115).name(), Theme::palette().ok.name());
  auto *xAxisPill = new QLabel(QStringLiteral("X 轴正常"));
  xAxisPill->setObjectName(QStringLiteral("bottomXAxisPill"));
  xAxisPill->setStyleSheet(okPillStyle);
  auto *yAxisPill = new QLabel(QStringLiteral("Y 轴正常"));
  yAxisPill->setObjectName(QStringLiteral("bottomYAxisPill"));
  yAxisPill->setStyleSheet(okPillStyle);
    layout->addWidget(xAxisPill);
    layout->addWidget(yAxisPill);

    layout->addStretch();

    auto *buildLabel = new QLabel(QStringLiteral("构建 #2026.04.20"));
    buildLabel->setObjectName(QStringLiteral("bottomBuildLabel"));
    auto *sep1 = new QLabel(QStringLiteral("|"));
    auto *userLabel = new QLabel(QStringLiteral("用户: HXS"));
    userLabel->setObjectName(QStringLiteral("bottomUserLabel"));
    auto *sep2 = new QLabel(QStringLiteral("|"));
    auto *versionLabel = new QLabel(QStringLiteral("v1.0.1"));
    versionLabel->setObjectName(QStringLiteral("bottomVersionLabel"));
    buildLabel->setStyleSheet(monoMutedStyle);
    sep1->setStyleSheet(monoMutedStyle);
    userLabel->setStyleSheet(monoMutedStyle);
    sep2->setStyleSheet(monoMutedStyle);
    versionLabel->setStyleSheet(monoMutedStyle);
    layout->addWidget(buildLabel);
    layout->addWidget(sep1);
    layout->addWidget(userLabel);
    layout->addWidget(sep2);
    layout->addWidget(versionLabel);

    setStyleSheet(QStringLiteral("background:%1;border-top:1px solid %2;").arg(Theme::palette().bgPanel.name(), Theme::palette().border.name()));

    setPosition(MachinePosition{});
    setProbeValue(-58.79);
    setTargetPoints(0);
    setCollectedPoints(1365);
    setTriggerFrames(0);
    setSampleProgress(5, 24);
    setFrameRate(22.0);
    setConnected(true);
    setMeasuring(false);
}

void BottomStatusBar::setPosition(const MachinePosition &position)
{
 const auto tc = Theme::palette().text1.name();
 m_xLabel->setText(QString("X: <span style=\"color:%1\"><b>%2</b></span>").arg(tc, QLocale().toString(position.x)));
 m_yLabel->setText(QString("Y: <span style=\"color:%1\"><b>%2</b></span>").arg(tc, QLocale().toString(position.y)));
}

void BottomStatusBar::setProbeValue(double probeValue)
{
 const auto tc = Theme::palette().text1.name();
 m_probeLabel->setText(QString("探针: <span style=\"color:%1\"><b>%2</b></span>").arg(tc, QString::number(probeValue, 'f', 3)));
}

void BottomStatusBar::setSampleProgress(int current, int total)
{
 const auto tc = Theme::palette().text1.name();
 m_sampleLabel->setText(QString("采样: <span style=\"color:%1\"><b>%2</b></span> / %3").arg(tc).arg(current).arg(total));
}

void BottomStatusBar::setFrameRate(double fps)
{
    const auto tc = Theme::palette().text1.name();
    m_fpsLabel->setText(QString("FPS: <span style=\"color:%1\"><b>%2</b></span>").arg(tc, QString::number(fps, 'f', 2)));
}

void BottomStatusBar::setTargetPoints(int points)
{
    const auto tc = Theme::palette().text1.name();
    m_targetPointsLabel->setText(QString("目标点数: <span style=\"color:%1\"><b>%2</b></span>").arg(tc).arg(points));
}

void BottomStatusBar::setCollectedPoints(int points)
{
    const auto tc = Theme::palette().text1.name();
    m_collectedPointsLabel->setText(QString("采集点数: <span style=\"color:%1\"><b>%2</b></span>").arg(tc).arg(points));
}

void BottomStatusBar::setTriggerFrames(int frames)
{
    const auto tc = Theme::palette().text1.name();
    m_triggerFramesLabel->setText(QString("触发帧: <span style=\"color:%1\"><b>%2</b></span>").arg(tc).arg(frames));
}

void BottomStatusBar::setConnected(bool connected)
{
m_connectionLabel->setText(connected ? QStringLiteral("连接成功") : QStringLiteral("连接断开"));
  m_connectionLabel->setStyleSheet(connected
    ? QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
      .arg(Theme::palette().okWeak.name(), Theme::palette().okWeak.darker(115).name(), Theme::palette().ok.name())
    : QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
      .arg(Theme::palette().errWeak.name(), Theme::palette().errWeak.darker(115).name(), Theme::palette().err.name()));
  setLedBulbColor(m_connectionLed, connected
    ? Theme::palette().ok
    : Theme::palette().err);
}

void BottomStatusBar::setMeasuring(bool measuring)
{
    m_measureStateLabel->setText(measuring ? QStringLiteral("测量中") : QStringLiteral("待机"));
  m_measureStateLabel->setStyleSheet(measuring
    ? QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
      .arg(Theme::palette().brandWeak.name(), Theme::palette().brandWeak.darker(115).name(), Theme::palette().brandStrong.name())
    : QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
      .arg(Theme::palette().bgSunken.name(), Theme::palette().border.name(), Theme::palette().textMuted.name()));
}
