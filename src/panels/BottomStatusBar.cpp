#include "panels/BottomStatusBar.h"

#include <QHBoxLayout>
#include <QLabel>

#include "theme/Theme.h"

BottomStatusBar::BottomStatusBar(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 4, 12, 4);
    layout->setSpacing(16);

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

    const auto labelStyle = QStringLiteral("font-size:11px;color:%1;").arg(Theme::palette().text.name());
    m_probeLabel->setStyleSheet(labelStyle);
    m_xLabel->setStyleSheet(labelStyle);
    m_yLabel->setStyleSheet(labelStyle);
    m_sampleLabel->setStyleSheet(labelStyle);
    m_fpsLabel->setStyleSheet(labelStyle);
    m_connectionLabel->setStyleSheet(QStringLiteral("QLabel{background:#EEF7F0;border:1px solid #D7ECDC;border-radius:9px;padding:1px 8px;color:#357A4D;font-size:10px;font-weight:600;}"));
    m_measureStateLabel->setStyleSheet(QStringLiteral("QLabel{background:#F3F5F8;border:1px solid #E2E6EC;border-radius:9px;padding:1px 8px;color:#596579;font-size:10px;font-weight:600;}"));

    layout->addWidget(m_probeLabel);
    layout->addWidget(m_xLabel);
    layout->addWidget(m_yLabel);
    layout->addWidget(m_sampleLabel);
    layout->addWidget(m_fpsLabel);
    layout->addWidget(m_connectionLabel);
    layout->addWidget(m_measureStateLabel);
    layout->addStretch();

    auto *userLabel = new QLabel(QStringLiteral("HXS · 操作员"));
    auto *versionLabel = new QLabel(QStringLiteral("工业软件测量系统 v0.1 / build 20260423"));
    versionLabel->setObjectName(QStringLiteral("bottomVersionLabel"));
    userLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;").arg(Theme::palette().textMuted.name()));
    versionLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;").arg(Theme::palette().textMuted.name()));
    layout->addWidget(userLabel);
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
    m_xLabel->setText(QStringLiteral("X %1").arg(position.x));
    m_yLabel->setText(QStringLiteral("Y %1").arg(position.y));
}

void BottomStatusBar::setProbeValue(double probeValue)
{
    m_probeLabel->setText(QStringLiteral("探针 %1 μm").arg(QString::number(probeValue, 'f', 3)));
}

void BottomStatusBar::setSampleProgress(int current, int total)
{
    m_sampleLabel->setText(QStringLiteral("采样 %1 / %2").arg(current).arg(total));
}

void BottomStatusBar::setFrameRate(double fps)
{
    m_fpsLabel->setText(QStringLiteral("FPS %1").arg(QString::number(fps, 'f', 1)));
}

void BottomStatusBar::setConnected(bool connected)
{
    m_connectionLabel->setText(connected ? QStringLiteral("连接正常") : QStringLiteral("连接断开"));
    m_connectionLabel->setStyleSheet(connected
                                         ? QStringLiteral("QLabel{background:#EEF7F0;border:1px solid #D7ECDC;border-radius:9px;padding:1px 8px;color:#357A4D;font-size:10px;font-weight:600;}")
                                         : QStringLiteral("QLabel{background:#FFF4F1;border:1px solid #F1D0C5;border-radius:9px;padding:1px 8px;color:#A54E2F;font-size:10px;font-weight:600;}"));
}

void BottomStatusBar::setMeasuring(bool measuring)
{
    m_measureStateLabel->setText(measuring ? QStringLiteral("测量中") : QStringLiteral("待机"));
    m_measureStateLabel->setStyleSheet(measuring
                                           ? QStringLiteral("QLabel{background:#EEF2FF;border:1px solid #D9E3FF;border-radius:9px;padding:1px 8px;color:#3550A8;font-size:10px;font-weight:600;}")
                                           : QStringLiteral("QLabel{background:#F3F5F8;border:1px solid #E2E6EC;border-radius:9px;padding:1px 8px;color:#596579;font-size:10px;font-weight:600;}"));
}
