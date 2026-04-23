#include "widgets/StatCardWidget.h"

#include <QChar>
#include <QFont>
#include <QPainter>
#include <cmath>

#include "theme/Theme.h"

StatCardWidget::StatCardWidget(QWidget *parent)
: QWidget(parent)
, m_accentColor(Theme::palette().brand)
{
setMinimumHeight(80);
}

void StatCardWidget::setLabel(const QString &label)
{
m_label = label;
update();
}

void StatCardWidget::setValue(const QString &value)
{
m_value = value;
update();
}

void StatCardWidget::setUnit(const QString &unit)
{
m_unit = unit;
update();
}

void StatCardWidget::setMetaText(const QString &metaText)
{
m_metaText = metaText;
update();
}

void StatCardWidget::setAccentColor(const QColor &accentColor)
{
m_accentColor = accentColor;
update();
}

void StatCardWidget::setTarget(const QString &target)
{
m_target = target;
update();
}

void StatCardWidget::setTrend(double trend)
{
m_trend = trend;
update();
}

void StatCardWidget::setSmall(bool small)
{
if (m_small == small) return;
m_small = small;
update();
emit smallChanged();
}

QString StatCardWidget::label() const
{
return m_label;
}

QString StatCardWidget::value() const
{
return m_value;
}

QString StatCardWidget::unit() const
{
return m_unit;
}

QString StatCardWidget::metaText() const
{
return m_metaText;
}

QColor StatCardWidget::accentColor() const
{
return m_accentColor;
}

QString StatCardWidget::target() const
{
return m_target;
}

double StatCardWidget::trend() const
{
return m_trend;
}

bool StatCardWidget::isSmall() const
{
return m_small;
}

int StatCardWidget::accentWidth() const
{
return 3;
}

int StatCardWidget::borderRadius() const
{
return 6;
}

void StatCardWidget::paintEvent(QPaintEvent *event)
{
Q_UNUSED(event)

QPainter painter(this);
painter.setRenderHint(QPainter::Antialiasing);
painter.fillRect(rect(), Theme::palette().bgPanel);
painter.fillRect(QRect(0, 0, 3, height()), m_accentColor);
painter.setPen(Theme::palette().border);
painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 6, 6);

int vPad = m_small ? 10 : 14;
int hPad = m_small ? 12 : 16;
int x0 = hPad + 3;

painter.setPen(Theme::palette().textMuted);
painter.setFont(QFont(QStringLiteral("Segoe UI"), 10, QFont::DemiBold));
painter.drawText(QRect(x0, vPad, width() - x0 - hPad, 16), Qt::AlignLeft | Qt::AlignVCenter, m_label);

int valueFontSize = m_small ? 20 : 28;
int valueY = vPad + 18;

painter.setPen(m_accentColor);
QFont valueFont(QStringLiteral("Consolas"), valueFontSize, QFont::DemiBold);
valueFont.setLetterSpacing(QFont::AbsoluteSpacing, -1);
painter.setFont(valueFont);
QRect valueRect(x0, valueY, width() - x0 - hPad, valueFontSize + 4);
painter.drawText(valueRect, Qt::AlignLeft | Qt::AlignVCenter, m_value);
int valueWidth = painter.fontMetrics().horizontalAdvance(m_value);

if (!m_unit.isEmpty()) {
painter.setPen(Theme::palette().textMuted);
QFont unitFont(QStringLiteral("Consolas"), 11);
painter.setFont(unitFont);
int unitX = x0 + valueWidth + 4;
int unitY = valueY + valueFontSize - 11;
painter.drawText(QRect(unitX, unitY, width() - unitX - hPad, 14), Qt::AlignLeft | Qt::AlignVCenter, m_unit);
}

int bottomY = height() - 16;
if (!m_target.isEmpty()) {
painter.setFont(QFont(QStringLiteral("Consolas"), 9));
const auto targetStr = QStringLiteral("TARGET %1").arg(m_target);
painter.setPen(Theme::palette().textMuted);
painter.drawText(QRect(x0, bottomY, width() - x0 - hPad, 14), Qt::AlignLeft | Qt::AlignVCenter, targetStr);
const auto targetWidth = painter.fontMetrics().horizontalAdvance(targetStr);
const bool significant = m_trend > 0.0005 || m_trend < -0.0005;
QString trendStr;
QColor trendColor;
if (significant && m_trend > 0) {
trendStr = QStringLiteral("%1 %2").arg(QChar(0x25B2), QString::number(m_trend, 'f', 3));
trendColor = Theme::palette().ok;
} else if (significant && m_trend < 0) {
trendStr = QStringLiteral("%1 %2").arg(QChar(0x25BC), QString::number(qAbs(m_trend), 'f', 3));
trendColor = Theme::palette().err;
} else {
trendStr = QStringLiteral("— 0.000");
trendColor = Theme::palette().textMuted;
}
painter.setPen(trendColor);
painter.drawText(QRect(x0 + targetWidth + 10, bottomY, width() - x0 - hPad - targetWidth - 10, 14), Qt::AlignLeft | Qt::AlignVCenter, trendStr);
} else if (!m_metaText.isEmpty()) {
painter.setPen(Theme::palette().text);
painter.setFont(QFont(QStringLiteral("Segoe UI"), 9));
painter.drawText(QRect(x0, bottomY, width() - x0 - hPad, 14), Qt::AlignLeft | Qt::AlignVCenter, m_metaText);
}
}

QSize StatCardWidget::sizeHint() const
{
return QSize(156, 80);
}
