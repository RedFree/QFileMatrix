#include "panels/TopTitleBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QSizePolicy>

#include "theme/Theme.h"

namespace {
QPushButton *makeNavButton(const QString &name, const QString &text, bool active = false)
{
    const auto &p = Theme::palette();
    auto *button = new QPushButton(text);
    button->setObjectName(name);
    button->setFlat(true);
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedHeight(32);
    button->setStyleSheet(active
        ? QStringLiteral("QPushButton{background:transparent;border:none;padding:0 16px;font-size:12px;font-weight:600;color:%1;border-bottom:2px solid %2;}"
                         "QPushButton:hover{color:%1;}").arg(p.headerText.name(), p.brand.name())
        : QStringLiteral("QPushButton{background:transparent;border:none;padding:0 16px;font-size:12px;font-weight:500;color:%1;border-bottom:2px solid transparent;}"
                         "QPushButton:hover{color:%2;}").arg(p.headerTextMuted.name(), p.headerTextSubtle.name()));
    return button;
}

class SearchButton : public QPushButton
{
public:
    explicit SearchButton(QWidget *parent = nullptr)
        : QPushButton(parent)
    {
        setObjectName(QStringLiteral("searchButton"));
        setCursor(Qt::PointingHandCursor);
        setFixedHeight(26);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    }

    QSize sizeHint() const override { return QSize(78, 26); }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPushButton::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        const auto &p = Theme::palette();
        const QColor color = underMouse() ? p.headerText : p.headerTextMuted;
        QPen pen(color);
        pen.setWidthF(1.6);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        const qreal cx = 13, cy = height() / 2.0;
        painter.drawEllipse(QPointF(cx - 1.5, cy), 5.0, 5.0);
        painter.drawLine(QPointF(cx + 3, cy + 3), QPointF(cx + 6.5, cy + 6.5));

        painter.setFont(QFont(QStringLiteral("Segoe UI"), 9));
        painter.drawText(QRectF(24, 0, width() - 24, height()), Qt::AlignVCenter | Qt::AlignLeft, QStringLiteral("搜索 ⌘K"));
    }
};

class AlarmButton : public QPushButton
{
public:
    explicit AlarmButton(QWidget *parent = nullptr)
        : QPushButton(parent)
    {
        setObjectName(QStringLiteral("alarmButton"));
        setCursor(Qt::PointingHandCursor);
        setFixedSize(30, 26);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPushButton::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        const auto &p = Theme::palette();
        const QColor color = underMouse() ? p.headerText : p.headerTextMuted;
        QPen pen(color);
        pen.setWidthF(1.6);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        const qreal cx = width() / 2.0, cy = height() / 2.0 - 1;
        const qreal w = 14, h = 10;
        const QRectF bellRect(cx - w / 2, cy - h / 2, w, h);
        QPainterPath bell;
        bell.moveTo(bellRect.left(), bellRect.bottom());
        bell.lineTo(bellRect.left(), bellRect.top() + h * 0.35);
        bell.quadTo(bellRect.left(), bellRect.top(), bellRect.center().x(), bellRect.top());
        bell.quadTo(bellRect.right(), bellRect.top(), bellRect.right(), bellRect.top() + h * 0.35);
        bell.lineTo(bellRect.right(), bellRect.bottom());
        bell.closeSubpath();
        painter.drawPath(bell);
        painter.drawLine(QPointF(bellRect.left() - 2, bellRect.bottom()), QPointF(bellRect.right() + 2, bellRect.bottom()));
        painter.drawEllipse(QPointF(cx, bellRect.bottom() + 2.5), 2.0, 2.0);

        painter.setPen(Qt::NoPen);
        painter.setBrush(Theme::palette().err);
        painter.drawEllipse(QPointF(width() - 5, 5), 3.5, 3.5);
    }
};
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
    brandTile->setStyleSheet(QStringLiteral("background:qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 %1,stop:1 %2);color:%3;border-radius:4px;font-size:11px;font-weight:700;")
        .arg(Theme::palette().brand.name(), Theme::palette().brandAccent.name(), Theme::palette().bgPanel.name()));

    auto *title = new QLabel(QStringLiteral("FilmMetrix"), brandWrap);
    title->setObjectName(QStringLiteral("brandTitleLabel"));
    title->setAttribute(Qt::WA_TranslucentBackground);
    title->setStyleSheet(QStringLiteral("background:transparent;color:%1;font-size:13px;font-weight:600;letter-spacing:0.01em;").arg(Theme::palette().headerText.name()));

    auto *version = new QLabel(QStringLiteral("v1.0.1"), brandWrap);
    version->setAttribute(Qt::WA_TranslucentBackground);
    version->setStyleSheet(QStringLiteral("background:transparent;color:%1;font-size:11px;font-family:Consolas,'JetBrains Mono',monospace;").arg(Theme::palette().text3.name()));

    auto *slash = new QLabel(QStringLiteral("/"), brandWrap);
    slash->setAttribute(Qt::WA_TranslucentBackground);
    slash->setStyleSheet(QStringLiteral("background:transparent;color:%1;font-size:11px;").arg(Theme::palette().headerTextMuted.name()));

    auto *subtitle = new QLabel(QStringLiteral("全自动膜厚测试系统"), brandWrap);
    subtitle->setObjectName(QStringLiteral("brandSubtitleLabel"));
    subtitle->setAttribute(Qt::WA_TranslucentBackground);
    subtitle->setStyleSheet(QStringLiteral("background:transparent;color:%1;font-size:12px;").arg(Theme::palette().headerTextSubtle.name()));

    m_userLabel = new QLabel(QStringLiteral("HXS · 操作员"), this);
    m_userLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_userLabel->setStyleSheet(QStringLiteral("background:transparent;color:%1;font-size:12px;").arg(Theme::palette().headerTextMuted.name()));

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

        auto *searchButton = new SearchButton(actionWrap);
        searchButton->setStyleSheet(QStringLiteral("QPushButton{background:transparent;border:none;border-radius:6px;}"
            "QPushButton:hover{background:%1;}" ).arg(Theme::palette().headerHover.name(QColor::HexArgb)));

        auto *alarmButton = new AlarmButton(actionWrap);
        alarmButton->setStyleSheet(QStringLiteral("QPushButton{background:transparent;border:none;border-radius:6px;}"
            "QPushButton:hover{background:%1;}" ).arg(Theme::palette().headerHover.name(QColor::HexArgb)));

    auto *divider = new QLabel(actionWrap);
    divider->setAttribute(Qt::WA_TranslucentBackground);
    divider->setFixedSize(1, 18);
    divider->setStyleSheet(QStringLiteral("background:%1;").arg(Theme::palette().headerDivider.name(QColor::HexArgb)));

    auto *userChip = new QLabel(QStringLiteral("HX"), actionWrap);
    userChip->setObjectName(QStringLiteral("userChipLabel"));
    userChip->setAttribute(Qt::WA_TranslucentBackground);
    userChip->setAlignment(Qt::AlignCenter);
    userChip->setFixedSize(22, 22);
    userChip->setStyleSheet(QStringLiteral("background:%1;color:%2;border-radius:11px;font-size:10px;font-weight:700;").arg(Theme::palette().brand.name(), Theme::palette().bgPanel.name()));

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
    painter.fillRect(QRect(0, height() - 1, width(), 1), Theme::palette().headerHover);
}
