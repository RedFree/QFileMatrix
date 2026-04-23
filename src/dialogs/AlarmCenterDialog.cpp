#include "dialogs/AlarmCenterDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "theme/Theme.h"

AlarmCenterDialog::AlarmCenterDialog(QWidget *parent)
: QDialog(parent)
{
    setWindowTitle(QStringLiteral("告警中心"));
    setFixedWidth(380);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *headerLayout = new QHBoxLayout;
    headerLayout->setContentsMargins(10, 8, 10, 8);
    m_countLabel = new QLabel(QStringLiteral("告警中心 · 0 条未处理"));
    m_countLabel->setStyleSheet(QStringLiteral("font-size:12px;font-weight:600;color:%1;").arg(Theme::palette().text.name()));
    auto *closeButton = new QPushButton(QStringLiteral("✕"));
    closeButton->setObjectName(QStringLiteral("alarmCenterCloseButton"));
    closeButton->setFlat(true);
    closeButton->setFixedSize(24, 24);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(QStringLiteral("QPushButton{border:none;color:%1;font-size:14px;}QPushButton:hover{color:%2;}")
        .arg(Theme::palette().textMuted.name(), Theme::palette().text.name()));
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    headerLayout->addWidget(m_countLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(closeButton);
    mainLayout->addLayout(headerLayout);

    auto *divider = new QWidget;
    divider->setFixedHeight(1);
    divider->setStyleSheet(QStringLiteral("background:%1;").arg(Theme::palette().border.name()));
    mainLayout->addWidget(divider);

    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setMaximumHeight(320);
    m_scrollArea->setStyleSheet(QStringLiteral("QScrollArea{border:none;background:transparent;}"));
    mainLayout->addWidget(m_scrollArea);

    setStyleSheet(QStringLiteral(
        "QDialog{background:%1;border:1px solid %2;border-radius:8px;}"
    ).arg(Theme::palette().bgPanel.name(), Theme::palette().border.name()));

    setAlarms({
        {QStringLiteral("warn"), QStringLiteral("光源老化已达 78%，建议 2 周内更换"), QStringLiteral("14:45:02")},
        {QStringLiteral("err"), QStringLiteral("Station-01 第 1009 号超差 (13.42μm)"), QStringLiteral("14:42:18")},
        {QStringLiteral("warn"), QStringLiteral("气压偏低 0.58 MPa，目标 0.62 MPa"), QStringLiteral("14:38:55")},
    });
}

void AlarmCenterDialog::setAlarms(const QVector<AlarmEntry> &alarms)
{
    m_alarms = alarms;
    m_countLabel->setText(QStringLiteral("告警中心 · %1 条未处理").arg(m_alarms.size()));
    rebuildList();
}

QVector<AlarmEntry> AlarmCenterDialog::alarms() const
{
    return m_alarms;
}

void AlarmCenterDialog::rebuildList()
{
    auto *container = new QWidget;
    container->setStyleSheet(QStringLiteral("background:transparent;"));
    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    for (int i = 0; i < m_alarms.size(); ++i) {
        const auto &alarm = m_alarms[i];
        auto *row = new QWidget;
        row->setStyleSheet(QStringLiteral("background:transparent;"));
        auto *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(10, 10, 14, 10);
        rowLayout->setSpacing(10);

        const bool isErr = alarm.type == QStringLiteral("err");
        auto *pill = new QLabel(isErr ? QStringLiteral("超差") : QStringLiteral("警告"));
        pill->setStyleSheet(isErr
            ? QStringLiteral("QLabel{background:#FFF4F1;border:1px solid #F1D0C5;border-radius:8px;padding:1px 6px;color:#A54E2F;font-size:9.5px;font-weight:600;font-family:Consolas;}")
            : QStringLiteral("QLabel{background:#FFF9EA;border:1px solid #F2E2AC;border-radius:8px;padding:1px 6px;color:#9A6B00;font-size:9.5px;font-weight:600;font-family:Consolas;}"));

        auto *contentWrap = new QWidget;
        contentWrap->setStyleSheet(QStringLiteral("background:transparent;"));
        auto *contentLayout = new QVBoxLayout(contentWrap);
        contentLayout->setContentsMargins(0, 0, 0, 0);
        contentLayout->setSpacing(2);
        auto *msgLabel = new QLabel(alarm.message);
        msgLabel->setStyleSheet(QStringLiteral("font-size:12px;color:%1;").arg(Theme::palette().text.name()));
        msgLabel->setWordWrap(true);
        auto *timeLabel = new QLabel(alarm.time);
        timeLabel->setStyleSheet(QStringLiteral("font-size:10.5px;color:%1;font-family:Consolas;").arg(Theme::palette().textMuted.name()));
        contentLayout->addWidget(msgLabel);
        contentLayout->addWidget(timeLabel);

        rowLayout->addWidget(pill);
        rowLayout->addWidget(contentWrap, 1);
        layout->addWidget(row);

        if (i < m_alarms.size() - 1) {
            auto *rowDivider = new QWidget;
            rowDivider->setFixedHeight(1);
            rowDivider->setStyleSheet(QStringLiteral("background:%1;").arg(Theme::palette().divider.name()));
            layout->addWidget(rowDivider);
        }
    }

    layout->addStretch();
    m_scrollArea->setWidget(container);
}
