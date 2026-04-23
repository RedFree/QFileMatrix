#include "widgets/DpadControlWidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "theme/Theme.h"

namespace {
QPushButton *makeButton(const QString &name, const QString &text)
{
    auto *button = new QPushButton(text);
    button->setObjectName(name);
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
    readout->setSpacing(6);

    auto *xLabel = new QLabel(QStringLiteral("X"));
    m_xValue = new QLabel;
    auto *yLabel = new QLabel(QStringLiteral("Y"));
    m_yValue = new QLabel;
    auto *stepLabel = new QLabel(QStringLiteral("STEP"));
    m_stepValue = new QLabel;

    xLabel->setStyleSheet(Theme::titleStyle());
    yLabel->setStyleSheet(Theme::titleStyle());
    stepLabel->setStyleSheet(Theme::titleStyle());

    readout->addWidget(xLabel);
    readout->addWidget(m_xValue);
    readout->addWidget(yLabel);
    readout->addWidget(m_yValue);
    readout->addWidget(stepLabel);
    readout->addWidget(m_stepValue);
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
    m_xValue->setText(QString::number(m_position.x));
    m_yValue->setText(QString::number(m_position.y));
    m_stepValue->setText(QString::number(m_step));
}
