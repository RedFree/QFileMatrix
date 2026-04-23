#include "panels/TopTitleBar.h"

#include <QHBoxLayout>

namespace {
QPushButton *makeNavButton(const QString &name, const QString &text, bool active = false)
{
    auto *button = new QPushButton(text);
    button->setObjectName(name);
    button->setFlat(true);
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedHeight(32);
    button->setStyleSheet(active
                              ? QStringLiteral("QPushButton{color:#FFFFFF;border:none;padding:0 16px;font-size:12px;font-weight:600;border-bottom:2px solid #5070D7;background:transparent;}"
                                               "QPushButton:hover{color:#FFFFFF;}")
                              : QStringLiteral("QPushButton{color:#98A2B3;border:none;padding:0 16px;font-size:12px;font-weight:500;border-bottom:2px solid transparent;background:transparent;}"
                                               "QPushButton:hover{color:#FFFFFF;}")
    );
    return button;
}
}

TopTitleBar::TopTitleBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(44);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 12, 0);
    layout->setSpacing(16);

    auto *brandWrap = new QWidget;
    auto *brandLayout = new QHBoxLayout(brandWrap);
    brandLayout->setContentsMargins(0, 0, 0, 0);
    brandLayout->setSpacing(8);

    auto *brandTile = new QLabel(QStringLiteral("μT"));
    brandTile->setObjectName(QStringLiteral("brandTileLabel"));
    brandTile->setAlignment(Qt::AlignCenter);
    brandTile->setFixedSize(22, 22);
    brandTile->setStyleSheet(QStringLiteral(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 #5B7FE8,stop:1 #4062D9);"
        "color:white;border-radius:4px;font-weight:700;font-size:11px;"));

    auto *title = new QLabel(QStringLiteral("工业软件测量系统"));
    title->setObjectName(QStringLiteral("brandTitleLabel"));
    auto *version = new QLabel(QStringLiteral("v1.0.1"));
    auto *slash = new QLabel(QStringLiteral("/"));
    auto *subtitle = new QLabel(QStringLiteral("高精度测量与过程监控"));
    subtitle->setObjectName(QStringLiteral("brandSubtitleLabel"));
    m_userLabel = new QLabel(QStringLiteral("HXS · 操作员"));

    title->setStyleSheet(QStringLiteral("font-size:13px;font-weight:600;letter-spacing:0.01em;color:#EDF1F7;"));
    version->setStyleSheet(QStringLiteral("font-size:11px;color:#8A93A1;font-family:monospace;"));
    slash->setStyleSheet(QStringLiteral("font-size:11px;color:#5B6573;"));
    subtitle->setStyleSheet(QStringLiteral("font-size:12px;color:#D3DAE6;"));
    m_userLabel->setStyleSheet(QStringLiteral("font-size:12px;color:#B8C1D0;"));

    brandLayout->addWidget(brandTile);
    brandLayout->addWidget(title);
    brandLayout->addWidget(version);
    brandLayout->addWidget(slash);
    brandLayout->addWidget(subtitle);

    auto *navWrap = new QWidget;
    navWrap->setObjectName(QStringLiteral("topNavWrap"));
    navWrap->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    auto *navLayout = new QHBoxLayout(navWrap);
    navLayout->setContentsMargins(20, 0, 0, 0);
    navLayout->setSpacing(2);

    m_measureButton = makeNavButton(QStringLiteral("navMeasureButton"), QStringLiteral("测量"), true);
    navLayout->addWidget(m_measureButton);
    navLayout->addWidget(makeNavButton(QStringLiteral("navDataButton"), QStringLiteral("数据")));
    navLayout->addWidget(makeNavButton(QStringLiteral("navRecipeButton"), QStringLiteral("配方")));
    navLayout->addWidget(makeNavButton(QStringLiteral("navMaintenanceButton"), QStringLiteral("维护")));
    navLayout->addWidget(makeNavButton(QStringLiteral("navAuditButton"), QStringLiteral("审计")));

    auto *actionWrap = new QWidget;
    auto *actionLayout = new QHBoxLayout(actionWrap);
    actionLayout->setContentsMargins(0, 0, 0, 0);
    actionLayout->setSpacing(14);

    auto *searchButton = new QPushButton(QStringLiteral("🔍 搜索  \u2318K"));
    searchButton->setObjectName(QStringLiteral("searchButton"));
    auto *alarmButton = new QPushButton(QStringLiteral("🔔 告警"));
    alarmButton->setObjectName(QStringLiteral("alarmButton"));
    auto *divider = new QLabel;
    divider->setFixedSize(1, 18);
    divider->setStyleSheet(QStringLiteral("background:rgba(255,255,255,0.12);"));
    auto *userChip = new QLabel(QStringLiteral("HX"));
    userChip->setObjectName(QStringLiteral("userChipLabel"));
    userChip->setAlignment(Qt::AlignCenter);
    userChip->setFixedSize(22, 22);
    userChip->setStyleSheet(QStringLiteral(
        "background:#5070D7;color:white;border-radius:11px;font-size:10px;font-weight:700;"));

    searchButton->setCursor(Qt::PointingHandCursor);
    alarmButton->setCursor(Qt::PointingHandCursor);
    searchButton->setFixedHeight(26);
    alarmButton->setFixedHeight(26);
    searchButton->setStyleSheet(QStringLiteral(
        "QPushButton{background:transparent;border:1px solid rgba(255,255,255,0.12);border-radius:6px;padding:4px 10px;color:#B8C1D0;font-size:11px;}"
        "QPushButton:hover{background:rgba(255,255,255,0.06);color:#EDF1F7;}"));
    alarmButton->setStyleSheet(QStringLiteral(
        "QPushButton{background:transparent;border:1px solid rgba(255,255,255,0.12);border-radius:6px;padding:4px 10px;color:#B8C1D0;font-size:11px;position:relative;}"
        "QPushButton:hover{background:rgba(255,255,255,0.06);color:#EDF1F7;}"));

    actionLayout->addWidget(searchButton);
    actionLayout->addWidget(alarmButton);
    actionLayout->addWidget(divider);
    actionLayout->addWidget(userChip);
    actionLayout->addWidget(m_userLabel);

    layout->addWidget(brandWrap);
    layout->addWidget(navWrap, 0, Qt::AlignLeft);
    layout->addStretch();
    layout->addWidget(actionWrap);

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(QStringLiteral("#0F1B2D")));
    setPalette(pal);
    setStyleSheet(QStringLiteral("border-bottom:1px solid rgba(255,255,255,0.06);"));
}

void TopTitleBar::setUserName(const QString &userName)
{
    m_userLabel->setText(userName);
}

QString TopTitleBar::userName() const
{
    return m_userLabel->text();
}
