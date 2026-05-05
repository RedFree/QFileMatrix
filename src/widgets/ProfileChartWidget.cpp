#include "widgets/ProfileChartWidget.h"

#include "qcustomplot.h"
#include "theme/Theme.h"

ProfileChartWidget::ProfileChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(420, 260);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_plot = new QCustomPlot(this);
    m_plot->setMinimumSize(400, 240);
    layout->addWidget(m_plot, 1);

    setupPlot();
}

void ProfileChartWidget::setupPlot()
{
    const auto &p = Theme::palette();

    m_plot->setBackground(QBrush(p.bgPanel));
    m_plot->axisRect()->setBackground(QBrush(p.bgPanel));
    m_plot->axisRect()->setMargins(QMargins(44, 10, 10, 24));

    m_plot->xAxis->setBasePen(QPen(p.border));
    m_plot->yAxis->setBasePen(QPen(p.border));
    m_plot->xAxis->setTickPen(QPen(p.border));
    m_plot->yAxis->setTickPen(QPen(p.border));
    m_plot->xAxis->setSubTickPen(QPen(Qt::NoPen));
    m_plot->yAxis->setSubTickPen(QPen(Qt::NoPen));
    m_plot->xAxis->setTickLabelColor(p.textMuted);
    m_plot->yAxis->setTickLabelColor(p.textMuted);
    m_plot->xAxis->setTickLabelFont(QFont(QStringLiteral("Consolas"), 8));
    m_plot->yAxis->setTickLabelFont(QFont(QStringLiteral("Consolas"), 8));

    const QColor gridColor = QColor(QStringLiteral("#D0D5DE"));
    m_plot->xAxis->grid()->setVisible(true);
    m_plot->xAxis->grid()->setPen(QPen(gridColor, 1, Qt::DotLine));
    m_plot->xAxis->grid()->setZeroLinePen(QPen(gridColor, 1, Qt::SolidLine));
    m_plot->yAxis->grid()->setVisible(true);
    m_plot->yAxis->grid()->setPen(QPen(gridColor, 1, Qt::DotLine));
    m_plot->yAxis->grid()->setZeroLinePen(QPen(gridColor, 1, Qt::SolidLine));

    m_plot->xAxis2->setVisible(false);
    m_plot->yAxis2->setVisible(false);

    updateAxisRanges();

    updateBands();

    m_profileGraph = m_plot->addGraph();
    m_profileGraph->setPen(QPen(p.accentTrace, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    m_profileGraph->setLineStyle(QCPGraph::lsLine);
    m_profileGraph->setScatterStyle(QCPScatterStyle::ssNone);

    m_targetLine = new QCPItemStraightLine(m_plot);
    m_targetLine->setPen(QPen(p.brandAccent, 1, Qt::DashLine));
    m_targetLine->point1->setCoords(0, 11.5);
    m_targetLine->point2->setCoords(1280, 11.5);

    m_axisLabel = new QCPItemText(m_plot);
    m_axisLabel->setText(QStringLiteral("μm vs px"));
    m_axisLabel->setFont(QFont(QStringLiteral("Consolas"), 9));
    m_axisLabel->setColor(p.textMuted);
    m_axisLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_axisLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    m_axisLabel->position->setCoords(0.02, 0.02);

    m_plot->replot();
}

void ProfileChartWidget::updateAxisRanges()
{
    m_plot->xAxis->setRange(0, 1280);
    m_plot->yAxis->setRange(-25, 100);

    m_plot->xAxis->ticker()->setTickCount(6);
    m_plot->yAxis->ticker()->setTickCount(7);
}

void ProfileChartWidget::updateBands()
{
    for (auto *rect : m_bandRects) {
        m_plot->removeItem(rect);
    }
    m_bandRects.clear();

    const auto &p = Theme::palette();
    const QVector<RefBand> bands {
        {250.0, 80.0, QStringLiteral("起始"), Theme::withAlpha(p.accentRef, 40), Theme::withAlpha(p.accentRef, 180)},
        {600.0, 120.0, QStringLiteral("数据"), Theme::withAlpha(p.ok, 30), Theme::withAlpha(p.ok, 180)},
        {900.0, 90.0, QStringLiteral("结束"), Theme::withAlpha(p.accentRef, 40), Theme::withAlpha(p.accentRef, 180)}
    };

    for (const auto &band : bands) {
        auto *rect = new QCPItemRect(m_plot);
        rect->setPen(QPen(band.stroke, 1.5));
        rect->setBrush(QBrush(band.fill));
        rect->topLeft->setCoords(band.x, 100);
        rect->bottomRight->setCoords(band.x + band.width, -25);
        m_bandRects.append(rect);

        auto *label = new QCPItemText(m_plot);
        label->setText(band.label);
        label->setFont(QFont(QStringLiteral("Consolas"), 7));
        label->setColor(band.stroke);
        label->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        label->position->setCoords(band.x + 4, 96);
    }
}

void ProfileChartWidget::updateTargetLine()
{
    if (m_targetLine) {
        m_targetLine->setPen(QPen(Theme::palette().brandAccent, 1, Qt::DashLine));
    }
}

void ProfileChartWidget::updateGraphData()
{
    if (!m_profileGraph) return;

    QVector<double> xs, ys;
    xs.reserve(m_profile.size());
    ys.reserve(m_profile.size());
    for (const auto &pt : m_profile) {
        xs.append(pt.x);
        ys.append(pt.y);
    }
    m_profileGraph->setData(xs, ys);
    m_plot->replot();
}

void ProfileChartWidget::setProfile(const ProfileData &profile)
{
    m_profile = profile;
    updateGraphData();
}

void ProfileChartWidget::setMeasuring(bool measuring)
{
    m_measuring = measuring;
    if (m_plot) {
        m_plot->replot();
    }
}

ProfileData ProfileChartWidget::profile() const
{
    return m_profile;
}

QVector<RefBand> ProfileChartWidget::bands() const
{
    return {};
}

QSize ProfileChartWidget::sizeHint() const
{
    return QSize(560, 320);
}
