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
    m_sampleLabel = new QLabel;
    m_fpsLabel = new QLabel;
    m_connectionLabel = new QLabel;
    m_measureStateLabel = new QLabel;

    m_probeLabel->setObjectName(QStringLiteral("bottomProbeLabel"));
    m_xLabel->setObjectName(QStringLiteral("bottomXLabel"));
    m_yLabel->setObjectName(QStringLiteral("bottomYLabel"));
    m_sampleLabel->setObjectName(QStringLiteral("bottomSampleLabel"));
    m_fpsLabel->setObjectName(QStringLiteral("bottomFpsLabel"));
    m_connectionLabel->setObjectName(QStringLiteral("bottomConnectionLabel"));
    m_measureStateLabel->setObjectName(QStringLiteral("bottomMeasureStateLabel"));

    const auto monoStyle = QStringLiteral("font-size:11px;font-family:Consolas;color:%1;").arg(Theme::palette().text.name());
    const auto monoMutedStyle = QStringLiteral("font-size:11px;font-family:Consolas;color:%1;").arg(Theme::palette().textMuted.name());
    m_probeLabel->setStyleSheet(monoStyle);
    m_xLabel->setStyleSheet(monoStyle);
    m_yLabel->setStyleSheet(monoStyle);
    m_sampleLabel->setStyleSheet(monoStyle);
    m_fpsLabel->setStyleSheet(monoStyle);
    m_connectionLabel->setStyleSheet(QStringLiteral("QLabel{background:#EEF7F0;border:1px solid #D7ECDC;border-radius:9px;padding:1px 8px;color:#357A4D;font-size:10px;font-weight:600;font-family:Consolas;}"));
    m_measureStateLabel->setStyleSheet(QStringLiteral("QLabel{background:#F3F5F8;border:1px solid #E2E6EC;border-radius:9px;padding:1px 8px;color:#596579;font-size:10px;font-weight:600;font-family:Consolas;}"));

    m_probeLabel->setTextFormat(Qt::RichText);
    m_xLabel->setTextFormat(Qt::RichText);
    m_yLabel->setTextFormat(Qt::RichText);
    m_sampleLabel->setTextFormat(Qt::RichText);
    m_fpsLabel->setTextFormat(Qt::RichText);

    layout->addWidget(m_probeLabel);
    layout->addSpacing(16);
    layout->addWidget(m_xLabel);
    layout->addSpacing(16);
    layout->addWidget(m_yLabel);
    layout->addSpacing(16);
    layout->addWidget(m_sampleLabel);
    layout->addSpacing(16);
layout->addWidget(m_fpsLabel);
layout->addSpacing(16);
auto *led = new LedBulb(QColor(QStringLiteral("#357A4D")));
m_connectionLed = led;
layout->addWidget(m_connectionLed);
layout->addSpacing(4);
layout->addWidget(m_connectionLabel);
layout->addSpacing(8);
    layout->addWidget(m_measureStateLabel);
    layout->addSpacing(8);

    auto *xAxisPill = new QLabel(QStringLiteral("X 轴正常"));
    xAxisPill->setObjectName(QStringLiteral("bottomXAxisPill"));
    xAxisPill->setStyleSheet(QStringLiteral("QLabel{background:#EEF7F0;border:1px solid #D7ECDC;border-radius:9px;padding:1px 6px;color:#357A4D;font-size:10px;font-weight:600;font-family:Consolas;}"));
    auto *yAxisPill = new QLabel(QStringLiteral("Y 轴正常"));
    yAxisPill->setObjectName(QStringLiteral("bottomYAxisPill"));
    yAxisPill->setStyleSheet(QStringLiteral("QLabel{background:#EEF7F0;border:1px solid #D7ECDC;border-radius:9px;padding:1px 6px;color:#357A4D;font-size:10px;font-weight:600;font-family:Consolas;}"));
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
    setSampleProgress(5, 24);
    setFrameRate(22.0);
    setConnected(true);
    setMeasuring(false);
}

void BottomStatusBar::setPosition(const MachinePosition &position)
{
    m_xLabel->setText(QStringLiteral("X: <b>%1</b>").arg(QLocale().toString(position.x)));
    m_yLabel->setText(QStringLiteral("Y: <b>%1</b>").arg(QLocale().toString(position.y)));
}

void BottomStatusBar::setProbeValue(double probeValue)
{
    m_probeLabel->setText(QStringLiteral("探针: <b>%1</b>").arg(QString::number(probeValue, 'f', 3)));
}

void BottomStatusBar::setSampleProgress(int current, int total)
{
    m_sampleLabel->setText(QStringLiteral("采样: <b>%1</b> / %2").arg(current).arg(total));
}

void BottomStatusBar::setFrameRate(double fps)
{
    m_fpsLabel->setText(QStringLiteral("FPS: <b>%1</b>").arg(QString::number(fps, 'f', 2)));
}

void BottomStatusBar::setConnected(bool connected)
{
m_connectionLabel->setText(connected ? QStringLiteral("连接成功") : QStringLiteral("连接断开"));
m_connectionLabel->setStyleSheet(connected
? QStringLiteral("QLabel{background:#EEF7F0;border:1px solid #D7ECDC;border-radius:9px;padding:1px 8px;color:#357A4D;font-size:10px;font-weight:600;font-family:Consolas;}")
: QStringLiteral("QLabel{background:#FFF4F1;border:1px solid #F1D0C5;border-radius:9px;padding:1px 8px;color:#A54E2F;font-size:10px;font-weight:600;font-family:Consolas;}"));
setLedBulbColor(m_connectionLed, connected
? QColor(QStringLiteral("#357A4D"))
: QColor(QStringLiteral("#A54E2F")));
}

void BottomStatusBar::setMeasuring(bool measuring)
{
    m_measureStateLabel->setText(measuring ? QStringLiteral("测量中") : QStringLiteral("待机"));
    m_measureStateLabel->setStyleSheet(measuring
        ? QStringLiteral("QLabel{background:#EEF2FF;border:1px solid #D9E3FF;border-radius:9px;padding:1px 8px;color:#3550A8;font-size:10px;font-weight:600;font-family:Consolas;}")
        : QStringLiteral("QLabel{background:#F3F5F8;border:1px solid #E2E6EC;border-radius:9px;padding:1px 8px;color:#596579;font-size:10px;font-weight:600;font-family:Consolas;}"));
}
