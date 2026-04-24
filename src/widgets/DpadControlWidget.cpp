#include "widgets/DpadControlWidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QVBoxLayout>

#include "theme/Theme.h"

namespace {
QPushButton *makeButton(const QString &name, const QString &text)
{
auto *button = new QPushButton(text);
button->setObjectName(name);
button->setFixedSize(32, 32);
return button;
}
}

DpadControlWidget::DpadControlWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(12);

    auto *grid = new QGridLayout;
    grid->setSpacing(4);

    auto *up = makeButton(QStringLiteral("upButton"), QStringLiteral("↑"));
    auto *down = makeButton(QStringLiteral("downButton"), QStringLiteral("↓"));
    auto *left = makeButton(QStringLiteral("leftButton"), QStringLiteral("←"));
    auto *right = makeButton(QStringLiteral("rightButton"), QStringLiteral("→"));
    auto *home = makeButton(QStringLiteral("homeButton"), QStringLiteral("HOME"));

    connect(up, &QPushButton::clicked, this, [this] { emit jogRequested(0, 1); });
    connect(down, &QPushButton::clicked, this, [this] { emit jogRequested(0, -1); });
    connect(left, &QPushButton::clicked, this, [this] { emit jogRequested(-1, 0); });
    connect(right, &QPushButton::clicked, this, [this] { emit jogRequested(1, 0); });
    connect(home, &QPushButton::clicked, this, &DpadControlWidget::homeRequested);

    grid->addWidget(up, 0, 1);
    grid->addWidget(left, 1, 0);
    grid->addWidget(home, 1, 1);
    grid->addWidget(right, 1, 2);
    grid->addWidget(down, 2, 1);

    auto *readout = new QVBoxLayout;
    readout->setContentsMargins(0, 0, 0, 0);
    readout->setSpacing(3);

    auto *xRow = new QHBoxLayout;
    auto *xLabel = new QLabel(QStringLiteral("X"));
    xLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;font-family:Consolas;").arg(Theme::palette().textMuted.name()));
    m_xValue = new QLabel;
    m_xValue->setStyleSheet(QStringLiteral("font-size:11px;color:%1;font-weight:600;font-family:Consolas;").arg(Theme::palette().text.name()));
    m_xValue->setAlignment(Qt::AlignRight);
    xRow->addWidget(xLabel);
    xRow->addStretch();
    xRow->addWidget(m_xValue);

    auto *yRow = new QHBoxLayout;
    auto *yLabel = new QLabel(QStringLiteral("Y"));
    yLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;font-family:Consolas;").arg(Theme::palette().textMuted.name()));
    m_yValue = new QLabel;
    m_yValue->setStyleSheet(QStringLiteral("font-size:11px;color:%1;font-weight:600;font-family:Consolas;").arg(Theme::palette().text.name()));
    m_yValue->setAlignment(Qt::AlignRight);
    yRow->addWidget(yLabel);
    yRow->addStretch();
    yRow->addWidget(m_yValue);

    auto *stepRow = new QHBoxLayout;
    auto *stepLabel = new QLabel(QStringLiteral("STEP"));
    stepLabel->setStyleSheet(QStringLiteral("font-size:10px;color:%1;font-family:Consolas;").arg(Theme::palette().textMuted.name()));
    m_stepValue = new QLabel;
    m_stepValue->setStyleSheet(QStringLiteral("font-size:10px;color:%1;font-family:Consolas;").arg(Theme::palette().textMuted.name()));
    m_stepValue->setAlignment(Qt::AlignRight);
    stepRow->addWidget(stepLabel);
    stepRow->addStretch();
    stepRow->addWidget(m_stepValue);

    readout->addLayout(xRow);
    readout->addLayout(yRow);
    readout->addLayout(stepRow);
    readout->addStretch();

    root->addLayout(grid);
    root->addLayout(readout, 1);

    setStyleSheet(Theme::buttonStyle());
    updateReadout();
}

void DpadControlWidget::setPosition(const MachinePosition &position)
{
    m_position = position;
    updateReadout();
}

void DpadControlWidget::setStep(int step)
{
    m_step = step;
    updateReadout();
}

MachinePosition DpadControlWidget::position() const
{
    return m_position;
}

int DpadControlWidget::step() const
{
    return m_step;
}

void DpadControlWidget::updateReadout()
{
    m_xValue->setText(QLocale().toString(m_position.x));
    m_yValue->setText(QLocale().toString(m_position.y));
    m_stepValue->setText(QStringLiteral("%1 pls").arg(QLocale().toString(m_step)));
}
