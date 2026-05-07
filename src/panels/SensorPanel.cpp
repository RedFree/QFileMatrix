#include "panels/SensorPanel.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <cmath>

#include "theme/Theme.h"
#include "widgets/PanelHeaderWidget.h"
#include "widgets/SensorGaugeWidget.h"

SensorPanel::SensorPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("sensorPanel"));
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *header = new PanelHeaderWidget(QStringLiteral("传感器"));

    auto *heightTab = new QPushButton(QStringLiteral("高度"));
    heightTab->setObjectName(QStringLiteral("heightTabButton"));
    heightTab->setFixedHeight(22);
    auto *lightTab = new QPushButton(QStringLiteral("光强"));
    lightTab->setObjectName(QStringLiteral("lightTabButton"));
    lightTab->setFixedHeight(22);
    const auto tabStyle = [](bool active) {
        if (active) {
      return QStringLiteral("QPushButton{background:transparent;border:1px solid transparent;border-radius:6px;padding:0 8px;min-height:22px;max-height:22px;color:%1;font-size:11px;font-weight:600;}"
        "QPushButton:hover{background:%2;}").arg(Theme::palette().brand.name(), Theme::withAlpha(Theme::palette().brand, 20).name(QColor::HexArgb));
        }
        return QStringLiteral("QPushButton{background:transparent;border:1px solid transparent;border-radius:6px;padding:0 8px;min-height:22px;max-height:22px;color:%1;font-size:11px;}"
            "QPushButton:hover{background:%2;color:%3;}")
            .arg(Theme::palette().textMuted.name(), Theme::palette().bgSunken.name(), Theme::palette().text.name());
    };
    heightTab->setProperty("active", true);
    lightTab->setProperty("active", false);
    heightTab->setStyleSheet(tabStyle(true));
    lightTab->setStyleSheet(tabStyle(false));
    const auto applyTabState = [heightTab, lightTab, tabStyle](QPushButton *activeButton) {
        const bool heightActive = activeButton == heightTab;
        heightTab->setProperty("active", heightActive);
        lightTab->setProperty("active", !heightActive);
        heightTab->setStyleSheet(tabStyle(heightActive));
        lightTab->setStyleSheet(tabStyle(!heightActive));
    };
    connect(heightTab, &QPushButton::clicked, this, [this, applyTabState, heightTab] {
        applyTabState(heightTab);
        setDisplayMode(DisplayMode::Height);
    });
    connect(lightTab, &QPushButton::clicked, this, [this, applyTabState, lightTab] {
        applyTabState(lightTab);
        setDisplayMode(DisplayMode::Light);
    });
    header->rightLayout()->addWidget(heightTab);
    header->rightLayout()->addWidget(lightTab);
    layout->addWidget(header);

    auto *body = new QVBoxLayout;
    body->setContentsMargins(8, 8, 8, 8);
    body->setSpacing(6);

    m_gauge = new SensorGaugeWidget;
    m_gauge->setLabel(QStringLiteral("高度 H"));
    m_gauge->setUnit(QStringLiteral("μm"));
    m_gauge->setRange(-1000.0, 1000.0);
    m_gauge->setValue(-58.79);
    body->addWidget(m_gauge);

    auto *detailGrid = new QGridLayout;
    detailGrid->setHorizontalSpacing(6);
    detailGrid->setVerticalSpacing(6);

    auto *absLabel = new QLabel(QStringLiteral("ABS"));
    auto *offsetLabel = new QLabel(QStringLiteral("OFFSET"));
    auto *pkLabel = new QLabel(QStringLiteral("峰谷值"));
    absLabel->setObjectName(QStringLiteral("sensorPrimaryKeyLabel"));
    offsetLabel->setObjectName(QStringLiteral("sensorOffsetKeyLabel"));
    pkLabel->setObjectName(QStringLiteral("sensorThirdKeyLabel"));
    auto *absValue = new QLabel;
    auto *offsetValue = new QLabel(QStringLiteral("0.000"));
    auto *pkValue = new QLabel;
    absValue->setObjectName(QStringLiteral("sensorAbsValueLabel"));
    pkValue->setObjectName(QStringLiteral("sensorPkPkValueLabel"));
    offsetValue->setObjectName(QStringLiteral("sensorOffsetValueLabel"));

    const auto keyStyle = QStringLiteral("font-size:10px;color:%1;letter-spacing:0.5px;font-weight:600;").arg(Theme::palette().textMuted.name());
    const auto valStyle = QStringLiteral("font-size:11px;color:%1;font-family:Consolas;font-weight:600;").arg(Theme::palette().text.name());
    absLabel->setStyleSheet(keyStyle);
    offsetLabel->setStyleSheet(keyStyle);
    pkLabel->setStyleSheet(keyStyle);
    absValue->setStyleSheet(valStyle);
    offsetValue->setStyleSheet(valStyle);
    pkValue->setStyleSheet(valStyle);

    detailGrid->addWidget(absLabel, 0, 0);
    detailGrid->addWidget(absValue, 0, 1);
    detailGrid->addWidget(offsetLabel, 1, 0);
    detailGrid->addWidget(offsetValue, 1, 1);
    detailGrid->addWidget(pkLabel, 2, 0);
    detailGrid->addWidget(pkValue, 2, 1);

    for (auto *label : {absLabel, offsetLabel, pkLabel, absValue, offsetValue, pkValue}) {
        label->setMinimumHeight(22);
        label->setAlignment(label == absValue || label == offsetValue || label == pkValue ? Qt::AlignRight | Qt::AlignVCenter : Qt::AlignLeft | Qt::AlignVCenter);
    }

    m_detailWrap = new QWidget;
    m_detailWrap->setLayout(detailGrid);
    body->addWidget(m_detailWrap);

    layout->addLayout(body);
    refreshDisplay();
}

void SensorPanel::setSensorValue(double value)
{
    m_heightValue = value;
    m_lightValue = qBound(0.0, 78.0 + value * 0.012, 100.0);
    refreshDisplay();
}

double SensorPanel::sensorValue() const
{
    return m_heightValue;
}

void SensorPanel::setDisplayMode(DisplayMode mode)
{
    if (m_displayMode == mode) {
        return;
    }
    m_displayMode = mode;
    refreshDisplay();
}

void SensorPanel::refreshDisplay()
{
    if (m_displayMode == DisplayMode::Height) {
        m_gauge->setLabel(QStringLiteral("高度 H"));
        m_gauge->setUnit(QStringLiteral("μm"));
        m_gauge->setRange(-1000.0, 1000.0);
        m_gauge->setValue(m_heightValue);
        refreshDerivedLabels(m_heightValue);
        return;
    }

    m_gauge->setLabel(QStringLiteral("光强 I"));
    m_gauge->setUnit(QStringLiteral("%"));
    m_gauge->setRange(0.0, 100.0);
    m_gauge->setValue(m_lightValue);
    refreshDerivedLabels(m_lightValue);
}

void SensorPanel::refreshDerivedLabels(double value)
{
    const QColor accent = m_displayMode == DisplayMode::Height ? Theme::palette().brand : Theme::palette().brandAccent;
    const QColor accentStrong = m_displayMode == DisplayMode::Height ? Theme::palette().gaugeHighlight : Theme::palette().brandStrong;
    if (m_detailWrap) {
        m_detailWrap->setStyleSheet(QStringLiteral("background:transparent;border:1px solid %1;border-radius:6px;padding:4px;")
            .arg(Theme::withAlpha(accent, 145).name(QColor::HexArgb)));
    }
    if (auto *primaryKey = findChild<QLabel*>(QStringLiteral("sensorPrimaryKeyLabel"))) {
        primaryKey->setText(m_displayMode == DisplayMode::Height ? QStringLiteral("ABS") : QStringLiteral("INTENSITY"));
        primaryKey->setStyleSheet(QStringLiteral("font-size:10px;color:%1;letter-spacing:0.5px;font-weight:700;").arg(accentStrong.name()));
    }
    if (auto *thirdKey = findChild<QLabel*>(QStringLiteral("sensorThirdKeyLabel"))) {
        thirdKey->setText(m_displayMode == DisplayMode::Height ? QStringLiteral("峰谷值") : QStringLiteral("稳定度"));
        thirdKey->setStyleSheet(QStringLiteral("font-size:10px;color:%1;letter-spacing:0.5px;font-weight:700;").arg(accentStrong.name()));
    }
    if (auto *offsetKey = findChild<QLabel*>(QStringLiteral("sensorOffsetKeyLabel"))) {
        offsetKey->setStyleSheet(QStringLiteral("font-size:10px;color:%1;letter-spacing:0.5px;font-weight:700;").arg(accentStrong.name()));
    }
    if (auto *absValue = findChild<QLabel*>(QStringLiteral("sensorAbsValueLabel"))) {
        absValue->setText(QString::number(value, 'f', m_displayMode == DisplayMode::Height ? 3 : 1));
        absValue->setStyleSheet(QStringLiteral("font-size:11px;color:%1;font-family:Consolas;font-weight:700;").arg(Theme::palette().text.name()));
    }
    if (auto *offsetValue = findChild<QLabel*>(QStringLiteral("sensorOffsetValueLabel"))) {
        offsetValue->setText(m_displayMode == DisplayMode::Height ? QStringLiteral("0.000") : QStringLiteral("+0.2"));
        offsetValue->setStyleSheet(QStringLiteral("font-size:11px;color:%1;font-family:Consolas;font-weight:700;").arg(Theme::palette().text.name()));
    }
    if (auto *pkValue = findChild<QLabel*>(QStringLiteral("sensorPkPkValueLabel"))) {
        pkValue->setStyleSheet(QStringLiteral("font-size:11px;color:%1;font-family:Consolas;font-weight:700;").arg(Theme::palette().text.name()));
        if (m_displayMode == DisplayMode::Light) {
            const double stability = qBound(0.0, 100.0 - qAbs(value - 78.0) * 2.0, 100.0);
            pkValue->setText(QStringLiteral("%1%").arg(QString::number(stability, 'f', 1)));
            return;
        }
        const double peak = std::abs(value);
        pkValue->setText(QStringLiteral("%1 / %2")
                             .arg(QString::number(peak, 'f', 2), QString::number(-peak * 0.8, 'f', 2)));
    }
}
