#include "panels/TopTitleBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSizePolicy>

#include "theme/Theme.h"

namespace {
QPushButton *makeNavButton(const QString &name, const QString &text, bool active = false)
{
  auto *button = new QPushButton(text);
  button->setObjectName(name);
  button->setFlat(true);
  button->setCursor(Qt::PointingHandCursor);
  button->setFixedHeight(32);
  button->setStyleSheet(active
    ? QStringLiteral("QPushButton{background:transparent;border:none;padding:0 16px;font-size:12px;font-weight:600;color:#FFFFFF;border-bottom:2px solid %1;}"
      "QPushButton:hover{color:#FFFFFF;}").arg(Theme::palette().brand.name())
    : QStringLiteral("QPushButton{background:transparent;border:none;padding:0 16px;font-size:12px;font-weight:500;color:#98A2B3;border-bottom:2px solid transparent;}"
      "QPushButton:hover{color:#D3DAE6;}"));
  return button;
}
}

TopTitleBar::TopTitleBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("TopTitleBar"));
    setFixedHeight(44);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 12, 0);
    layout->setSpacing(14);

    auto *brandWrap = new QWidget(this);
    brandWrap->setObjectName(QStringLiteral("brandWrap"));
    brandWrap->setAttribute(Qt::WA_TranslucentBackground);

    auto *brandLayout = new QHBoxLayout(brandWrap);
    brandLayout->setContentsMargins(0, 0, 0, 0);
    brandLayout->setSpacing(6);

    auto *brandTile = new QLabel(QStringLiteral("μT"), brandWrap);
    brandTile->setObjectName(QStringLiteral("brandTileLabel"));
    brandTile->setAlignment(Qt::AlignCenter);
    brandTile->setFixedSize(22, 22);
    brandTile->setStyleSheet(QStringLiteral("background:%1;color:#FFFFFF;border-radius:4px;font-size:11px;font-weight:700;").arg(Theme::palette().brand.name()));

    auto *title = new QLabel(QStringLiteral("工业软件测量系统"), brandWrap);
    title->setObjectName(QStringLiteral("brandTitleLabel"));
    title->setAttribute(Qt::WA_TranslucentBackground);
    title->setStyleSheet(QStringLiteral("background:transparent;color:#EDF1F7;font-size:13px;font-weight:600;letter-spacing:0.01em;"));

    auto *version = new QLabel(QStringLiteral("v1.0.1"), brandWrap);
    version->setAttribute(Qt::WA_TranslucentBackground);
    version->setStyleSheet(QStringLiteral("background:transparent;color:#8A93A1;font-size:11px;font-family:Consolas,'JetBrains Mono',monospace;"));

    auto *slash = new QLabel(QStringLiteral("/"), brandWrap);
    slash->setAttribute(Qt::WA_TranslucentBackground);
    slash->setStyleSheet(QStringLiteral("background:transparent;color:%1;font-size:11px;").arg(Theme::palette().textMuted.name()));

    auto *subtitle = new QLabel(QStringLiteral("高精度测量与过程监控"), brandWrap);
    subtitle->setObjectName(QStringLiteral("brandSubtitleLabel"));
    subtitle->setAttribute(Qt::WA_TranslucentBackground);
    subtitle->setStyleSheet(QStringLiteral("background:transparent;color:#D3DAE6;font-size:12px;"));

    m_userLabel = new QLabel(QStringLiteral("HXS · 操作员"), this);
    m_userLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_userLabel->setStyleSheet(QStringLiteral("background:transparent;color:#B8C1D0;font-size:12px;"));

    brandLayout->addWidget(brandTile);
    brandLayout->addWidget(title);
    brandLayout->addWidget(version);
    brandLayout->addWidget(slash);
    brandLayout->addWidget(subtitle);

    auto *navWrap = new QWidget(this);
    navWrap->setObjectName(QStringLiteral("topNavWrap"));
    navWrap->setAttribute(Qt::WA_TranslucentBackground);
    navWrap->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    auto *navLayout = new QHBoxLayout(navWrap);
    navLayout->setContentsMargins(18, 0, 0, 0);
    navLayout->setSpacing(2);

    m_measureButton = makeNavButton(QStringLiteral("navMeasureButton"), QStringLiteral("测量"), true);
    navLayout->addWidget(m_measureButton);
    navLayout->addWidget(makeNavButton(QStringLiteral("navDataButton"), QStringLiteral("数据")));
    navLayout->addWidget(makeNavButton(QStringLiteral("navRecipeButton"), QStringLiteral("配方")));
    navLayout->addWidget(makeNavButton(QStringLiteral("navMaintenanceButton"), QStringLiteral("维护")));
    navLayout->addWidget(makeNavButton(QStringLiteral("navAuditButton"), QStringLiteral("审计")));

    auto *actionWrap = new QWidget(this);
    actionWrap->setObjectName(QStringLiteral("actionWrap"));
    actionWrap->setAttribute(Qt::WA_TranslucentBackground);

    auto *actionLayout = new QHBoxLayout(actionWrap);
    actionLayout->setContentsMargins(0, 0, 0, 0);
    actionLayout->setSpacing(12);

    auto *searchButton = new QPushButton(QStringLiteral("🔍 搜索  ⌘K"), actionWrap);
    searchButton->setObjectName(QStringLiteral("searchButton"));
    searchButton->setCursor(Qt::PointingHandCursor);
    searchButton->setFlat(true);
    searchButton->setFixedHeight(26);
    searchButton->setStyleSheet(QStringLiteral("QPushButton{background:transparent;border:none;border-radius:6px;padding:0 8px;color:#B8C1D0;font-size:11px;}"
                                               "QPushButton:hover{background:rgba(255,255,255,0.06);color:#EDF1F7;}"));

    auto *alarmButton = new QPushButton(QStringLiteral("🔔 告警"), actionWrap);
    alarmButton->setObjectName(QStringLiteral("alarmButton"));
    alarmButton->setCursor(Qt::PointingHandCursor);
    alarmButton->setFlat(true);
    alarmButton->setFixedHeight(26);
    alarmButton->setStyleSheet(QStringLiteral("QPushButton{background:transparent;border:none;border-radius:6px;padding:0 8px;color:#B8C1D0;font-size:11px;}"
                                              "QPushButton:hover{background:rgba(255,255,255,0.06);color:#EDF1F7;}"));

    auto *divider = new QLabel(actionWrap);
    divider->setAttribute(Qt::WA_TranslucentBackground);
    divider->setFixedSize(1, 18);
    divider->setStyleSheet(QStringLiteral("background:rgba(255,255,255,0.12);"));

    auto *userChip = new QLabel(QStringLiteral("HX"), actionWrap);
    userChip->setObjectName(QStringLiteral("userChipLabel"));
    userChip->setAttribute(Qt::WA_TranslucentBackground);
    userChip->setAlignment(Qt::AlignCenter);
    userChip->setFixedSize(22, 22);
    userChip->setStyleSheet(QStringLiteral("background:%1;color:#FFFFFF;border-radius:11px;font-size:10px;font-weight:700;").arg(Theme::palette().brand.name()));

    actionLayout->addWidget(searchButton);
    actionLayout->addWidget(alarmButton);
    actionLayout->addWidget(divider);
    actionLayout->addWidget(userChip);
    actionLayout->addWidget(m_userLabel);

    layout->addWidget(brandWrap);
    layout->addWidget(navWrap, 0, Qt::AlignLeft);
    layout->addStretch();
    layout->addWidget(actionWrap);

    setStyleSheet(QStringLiteral("QWidget#brandWrap,QWidget#topNavWrap,QWidget#actionWrap{background:transparent;}"));
}

void TopTitleBar::setUserName(const QString &userName)
{
    m_userLabel->setText(userName);
}

QString TopTitleBar::userName() const
{
    return m_userLabel->text();
}

void TopTitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), Theme::palette().bgHeader);
    painter.fillRect(QRect(0, height() - 1, width(), 1), QColor(255, 255, 255, 15));
}
