#include "panels/BottomStatusBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>

#include "theme/Theme.h"

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
    m_connectionLabel = new QLabel;
    m_measureStateLabel = new QLabel;

    m_probeLabel->setObjectName(QStringLiteral("bottomProbeLabel"));
    m_xLabel->setObjectName(QStringLiteral("bottomXLabel"));
    m_yLabel->setObjectName(QStringLiteral("bottomYLabel"));
    m_connectionLabel->setObjectName(QStringLiteral("bottomConnectionLabel"));
    m_measureStateLabel->setObjectName(QStringLiteral("bottomMeasureStateLabel"));

    const auto monoStyle = QStringLiteral("font-size:11px;font-family:Consolas;color:%1;").arg(Theme::palette().textMuted.name());
    m_probeLabel->setStyleSheet(monoStyle);
    m_xLabel->setStyleSheet(monoStyle);
    m_yLabel->setStyleSheet(monoStyle);
    m_connectionLabel->setStyleSheet(QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
        .arg(Theme::palette().okWeak.name(), Theme::palette().okWeak.darker(115).name(), Theme::palette().ok.name()));
    m_measureStateLabel->setStyleSheet(QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
        .arg(Theme::palette().bgSunken.name(), Theme::palette().border.name(), Theme::palette().textMuted.name()));

    m_probeLabel->setTextFormat(Qt::RichText);
    m_xLabel->setTextFormat(Qt::RichText);
    m_yLabel->setTextFormat(Qt::RichText);

    layout->addWidget(m_probeLabel);
    layout->addSpacing(16);
    layout->addWidget(m_xLabel);
    layout->addSpacing(16);
    layout->addWidget(m_yLabel);
    layout->addSpacing(16);
    layout->addWidget(m_connectionLabel);
    layout->addSpacing(8);
    layout->addWidget(m_measureStateLabel);
    layout->addStretch();

    auto *versionLabel = new QLabel(QStringLiteral("v1.0.1"));
    versionLabel->setObjectName(QStringLiteral("bottomVersionLabel"));
    versionLabel->setStyleSheet(monoStyle);
    layout->addWidget(versionLabel);

    setStyleSheet(QStringLiteral("background:%1;border-top:1px solid %2;").arg(Theme::palette().bgPanel.name(), Theme::palette().border.name()));

    setPosition(MachinePosition{});
    setProbeValue(-58.79);
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

void BottomStatusBar::setConnected(bool connected)
{
    m_connectionLabel->setText(connected ? QStringLiteral("连接成功") : QStringLiteral("连接断开"));
    m_connectionLabel->setStyleSheet(connected
        ? QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
            .arg(Theme::palette().okWeak.name(), Theme::palette().okWeak.darker(115).name(), Theme::palette().ok.name())
        : QStringLiteral("QLabel{background:%1;border:1px solid %2;border-radius:9px;padding:1px 8px;color:%3;font-size:10px;font-weight:600;font-family:Consolas;}")
            .arg(Theme::palette().errWeak.name(), Theme::palette().errWeak.darker(115).name(), Theme::palette().err.name()));
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
