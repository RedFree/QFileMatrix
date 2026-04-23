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
            return QStringLiteral("QPushButton{background:transparent;border:1px solid transparent;border-radius:6px;padding:0 8px;min-height:22px;max-height:22px;color:#5070D7;font-size:11px;font-weight:600;}"
                "QPushButton:hover{background:rgba(80,112,215,0.08);}");
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
    connect(heightTab, &QPushButton::clicked, this, [applyTabState, heightTab] { applyTabState(heightTab); });
    connect(lightTab, &QPushButton::clicked, this, [applyTabState, lightTab] { applyTabState(lightTab); });
    header->rightLayout()->addWidget(heightTab);
    header->rightLayout()->addWidget(lightTab);
    layout->addWidget(header);

    auto *body = new QVBoxLayout;
    body->setContentsMargins(10, 10, 10, 10);
    body->setSpacing(8);

    m_gauge = new SensorGaugeWidget;
    m_gauge->setLabel(QStringLiteral("高度 H"));
    m_gauge->setUnit(QStringLiteral("μm"));
    m_gauge->setRange(-1000.0, 1000.0);
    m_gauge->setValue(-58.79);
    body->addWidget(m_gauge);

    auto *detailGrid = new QGridLayout;
    detailGrid->setHorizontalSpacing(10);
    detailGrid->setVerticalSpacing(6);

    auto *absLabel = new QLabel(QStringLiteral("ABS"));
    auto *offsetLabel = new QLabel(QStringLiteral("OFFSET"));
    auto *pkLabel = new QLabel(QStringLiteral("峰谷值"));
    auto *absValue = new QLabel;
    auto *offsetValue = new QLabel(QStringLiteral("0.000"));
    auto *pkValue = new QLabel;
    absValue->setObjectName(QStringLiteral("sensorAbsValueLabel"));
    pkValue->setObjectName(QStringLiteral("sensorPkPkValueLabel"));
    offsetValue->setObjectName(QStringLiteral("sensorOffsetValueLabel"));

    const auto keyStyle = QStringLiteral("font-size:11px;color:%1;").arg(Theme::palette().textMuted.name());
    const auto valStyle = QStringLiteral("font-size:11px;color:%1;font-family:Consolas;").arg(Theme::palette().text.name());
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

    auto *detailWrap = new QWidget;
    detailWrap->setStyleSheet(QStringLiteral("border-top:1px dashed %1;padding-top:8px;").arg(Theme::palette().divider.name()));
    detailWrap->setLayout(detailGrid);
    body->addWidget(detailWrap);

    layout->addLayout(body);
    refreshDerivedLabels(-58.79);
}

void SensorPanel::setSensorValue(double value)
{
    m_gauge->setValue(value);
    refreshDerivedLabels(value);
}

double SensorPanel::sensorValue() const
{
    return m_gauge->value();
}

void SensorPanel::refreshDerivedLabels(double value)
{
    if (auto *absValue = findChild<QLabel*>(QStringLiteral("sensorAbsValueLabel"))) {
        absValue->setText(QString::number(value, 'f', 3));
    }
    if (auto *pkValue = findChild<QLabel*>(QStringLiteral("sensorPkPkValueLabel"))) {
        const double peak = std::abs(value);
        pkValue->setText(QStringLiteral("%1 / %2")
                             .arg(QString::number(peak, 'f', 2), QString::number(-peak * 0.8, 'f', 2)));
    }
}
