#include "widgets/PanelHeaderWidget.h"

#include <QHBoxLayout>
#include <QLabel>

#include "theme/Theme.h"

PanelHeaderWidget::PanelHeaderWidget(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(32);
    setStyleSheet(QStringLiteral("background:%1;border-bottom:1px solid %2;").arg(Theme::palette().bgPanel.name(), Theme::palette().border.name()));

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 12, 0);
    layout->setSpacing(8);

    m_titleLabel = new QLabel(title);
    m_titleLabel->setObjectName(QStringLiteral("panelHeaderTitle"));
    m_titleLabel->setStyleSheet(QStringLiteral("font-size:11px;font-weight:600;letter-spacing:0.08em;text-transform:uppercase;color:%1;").arg(Theme::palette().textMuted.name()));
    layout->addWidget(m_titleLabel);
    layout->addStretch();

    m_rightLayout = new QHBoxLayout;
    m_rightLayout->setContentsMargins(0, 0, 0, 0);
    m_rightLayout->setSpacing(6);
    layout->addLayout(m_rightLayout);
}

QHBoxLayout *PanelHeaderWidget::rightLayout() const
{
    return m_rightLayout;
}

QLabel *PanelHeaderWidget::titleLabel() const
{
    return m_titleLabel;
}
