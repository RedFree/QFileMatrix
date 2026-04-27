#include "panels/DeviceStatusBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>

#include "theme/Theme.h"
#include "widgets/LedIndicatorWidget.h"

namespace {
LedIndicatorWidget *makeStatus(const QString &name, const QString &label, const QString &sub, LedIndicatorWidget::State state)
{
    auto *indicator = new LedIndicatorWidget;
    indicator->setObjectName(name);
    indicator->setLabel(label);
    indicator->setSubLabel(sub);
    indicator->setState(state);
    indicator->setCompact(true);
    indicator->setMaximumWidth(120);
    return indicator;
}

class StatusPillWidget : public QWidget
{
public:
    explicit StatusPillWidget(QWidget *parent = nullptr)
        : QWidget(parent)
        , m_blinkTimer(new QTimer(this))
    {
        setFixedHeight(20);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        setProperty("measuring", false);
        setProperty("dotVisible", true);
        m_blinkTimer->setObjectName(QStringLiteral("statusBlinkTimer"));
        m_blinkTimer->setInterval(500);
        connect(m_blinkTimer, &QTimer::timeout, this, [this] {
            setProperty("dotVisible", !property("dotVisible").toBool());
            update();
        });
    }

    QSize sizeHint() const override { return QSize(60, 20); }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        const bool measuring = property("measuring").toBool();
        QColor bg = Theme::palette().bgSunken;
        QColor border = Theme::palette().border;
        QColor fg = Theme::palette().textMuted;
        QString text = QStringLiteral("待机");

        if (measuring) {
            bg = Theme::palette().brandWeak;
            border = Theme::palette().brandWeak.darker(115);
            fg = Theme::palette().brandStrong;
            text = QStringLiteral("测量中");
        }

        const QRect pillRect = rect().adjusted(0, 1, 0, -1);
        painter.setPen(border);
        painter.setBrush(bg);
        painter.drawRoundedRect(pillRect, pillRect.height() / 2.0, pillRect.height() / 2.0);

        const qreal dotR = 3.0;
        const qreal dotX = pillRect.left() + pillRect.height() / 2.0;
        const qreal dotY = pillRect.center().y();

        if (!measuring || property("dotVisible").toBool()) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(fg);
            painter.drawEllipse(QPointF(dotX, dotY), dotR, dotR);
        }

        painter.setPen(fg);
        painter.setBrush(Qt::NoBrush);
        QFont font(QStringLiteral("Consolas"), 9);
        font.setWeight(QFont::Bold);
        painter.setFont(font);
        const QRect textRect = pillRect.adjusted(qRound(pillRect.height() / 2.0) + 2, 0, -4, 0);
        painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
    }

private:
    QTimer *m_blinkTimer;
};
}

DeviceStatusBar::DeviceStatusBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(44);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(14, 0, 14, 0);
    layout->setSpacing(6);

    auto *leftWrap = new QWidget;
    auto *leftLayout = new QHBoxLayout(leftWrap);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(4);
    leftLayout->addWidget(makeStatus(QStringLiteral("commStatus"), QStringLiteral("通讯"), QStringLiteral("TCP 192.168.1.10"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("xAxisStatus"), QStringLiteral("X 轴"), QStringLiteral("Ready"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("yAxisStatus"), QStringLiteral("Y 轴"), QStringLiteral("Ready"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("zAxisStatus"), QStringLiteral("Z 轴"), QStringLiteral("Ready"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("airStatus"), QStringLiteral("气压"), QStringLiteral("0.62 MPa"), LedIndicatorWidget::State::Ok));
    leftLayout->addWidget(makeStatus(QStringLiteral("lightStatus"), QStringLiteral("光源"), QStringLiteral("Aging 78%"), LedIndicatorWidget::State::Warn));
    leftLayout->addWidget(makeStatus(QStringLiteral("emergencyStatus"), QStringLiteral("急停"), QStringLiteral("Released"), LedIndicatorWidget::State::Off));
    layout->addWidget(leftWrap);

    auto *divider = new QWidget;
    divider->setObjectName(QStringLiteral("statusDivider"));
    divider->setFixedSize(1, 22);
    divider->setStyleSheet(QStringLiteral("background:%1;").arg(Theme::palette().border.name()));
    layout->addWidget(divider);

    auto *progressWrap = new QWidget;
    auto *progressLayout = new QHBoxLayout(progressWrap);
    progressLayout->setContentsMargins(0, 0, 0, 0);
    progressLayout->setSpacing(10);
    progressWrap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_progressTitleLabel = new QLabel(QStringLiteral("测量进度"));
    m_progressTitleLabel->setStyleSheet(QStringLiteral("font-size:10.5px;font-weight:600;letter-spacing:1px;color:%1;").arg(Theme::palette().textMuted.name()));
    m_stateLabel = new StatusPillWidget;
    m_stateLabel->setObjectName(QStringLiteral("statusStateLabel"));
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedWidth(280);
    m_percentLabel = new QLabel(QStringLiteral("0%"));
    m_percentLabel->setObjectName(QStringLiteral("statusPercentLabel"));
    m_percentLabel->setMinimumWidth(40);
    m_percentLabel->setStyleSheet(QStringLiteral("font-size:11px;color:%1;font-family:Consolas;").arg(Theme::palette().text1.name()));

    progressLayout->addWidget(m_progressTitleLabel);
    progressLayout->addWidget(m_progressBar);
    progressLayout->addWidget(m_percentLabel);
    progressLayout->addWidget(m_stateLabel);
    layout->addWidget(progressWrap, 1);

    auto *actionWrap = new QWidget;
    auto *actionLayout = new QHBoxLayout(actionWrap);
    actionLayout->setContentsMargins(0, 0, 0, 0);
    actionLayout->setSpacing(6);

 auto *startButton = new QPushButton(QStringLiteral("开始测量"));
 startButton->setObjectName(QStringLiteral("statusStartButton"));
 startButton->setProperty("role", QStringLiteral("primary"));
 startButton->setFixedHeight(22);
 m_startButton = startButton;
 auto *manualButton = new QPushButton(QStringLiteral("手动采样"));
 manualButton->setObjectName(QStringLiteral("statusManualButton"));
 manualButton->setFixedHeight(22);
 m_manualButton = manualButton;
 auto *stopButton = new QPushButton(QStringLiteral("停止"));
 stopButton->setObjectName(QStringLiteral("statusStopButton"));
 stopButton->setFixedHeight(22);
 m_stopButton = stopButton;

 connect(startButton, &QPushButton::clicked, this, &DeviceStatusBar::startRequested);
 connect(manualButton, &QPushButton::clicked, this, &DeviceStatusBar::manualRequested);
 connect(stopButton, &QPushButton::clicked, this, &DeviceStatusBar::stopRequested);

 actionLayout->addWidget(startButton);
 actionLayout->addWidget(manualButton);
 actionLayout->addWidget(stopButton);
 stopButton->hide();
    layout->addWidget(actionWrap);

 setStyleSheet(QStringLiteral(
 "QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:0 8px;min-height:22px;max-height:22px;color:%3;font-size:11px;}"
 "QPushButton:hover{background:%4;}"
 "QPushButton[role='primary']{background:%5;border-color:%5;color:white;}"
 "QPushButton#statusStopButton{background:%6;border-color:%7;color:%8;}"
 )
 .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text1.name(), Theme::palette().bgSunken.name(), Theme::palette().brand.name(),
 Theme::palette().errWeak.name(), Theme::palette().errWeak.darker(115).name(), Theme::palette().err.name()));
}

void DeviceStatusBar::setProgress(double progress)
{
    m_progressBar->setValue(static_cast<int>(progress));
    m_percentLabel->setText(QStringLiteral("%1%").arg(static_cast<int>(progress)));
}

void DeviceStatusBar::setMeasuring(bool measuring)
{
    m_stateLabel->setProperty("measuring", measuring);
    m_stateLabel->update();

    auto *timer = m_stateLabel->findChild<QTimer*>(QStringLiteral("statusBlinkTimer"));
    if (timer) {
        if (measuring) {
            timer->start();
        } else {
            timer->stop();
            m_stateLabel->setProperty("dotVisible", true);
            m_stateLabel->update();
        }
    }

    if (m_startButton) m_startButton->setVisible(!measuring);
    if (m_manualButton) m_manualButton->setVisible(!measuring);
    if (m_stopButton) m_stopButton->setVisible(measuring);

    m_progressBar->setStyleSheet(measuring
        ? QStringLiteral("QProgressBar{background:%1;border:none;border-radius:3px;height:6px;}QProgressBar::chunk{background:%2;border-radius:3px;}")
            .arg(Theme::palette().bgSunken.name(), Theme::palette().brand.name())
        : QStringLiteral("QProgressBar{background:%1;border:none;border-radius:3px;height:6px;}QProgressBar::chunk{background:%2;border-radius:3px;}")
            .arg(Theme::palette().bgSunken.name(), Theme::palette().ok.name()));
}
