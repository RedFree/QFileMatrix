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

void StatCardWidget::setShowProgress(bool show)
{
  if (m_showProgress == show) return;
  m_showProgress = show;
  update();
}

void StatCardWidget::setProgress(double progress)
{
  const double clamped = qBound(0.0, progress, 1.0);
  if (qFuzzyCompare(m_progress, clamped)) return;
  m_progress = clamped;
  if (m_showProgress) update();
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

bool StatCardWidget::showProgress() const
{
  return m_showProgress;
}

double StatCardWidget::progress() const
{
  return m_progress;
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
if (!m_showProgress) {
  painter.fillRect(QRect(0, 0, 3, height()), m_accentColor);
}
painter.setPen(Theme::palette().border);
painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 6, 6);

int vPad = m_small ? 10 : 14;
int hPad = m_small ? 12 : 16;
int x0 = m_showProgress ? hPad : hPad + 3;

painter.setPen(Theme::palette().textMuted);
painter.setFont(QFont(QStringLiteral("Segoe UI"), m_showProgress ? 11 : 10, QFont::DemiBold));
painter.drawText(QRect(x0, vPad, width() - x0 - hPad, 16), Qt::AlignLeft | Qt::AlignVCenter, m_label);

if (m_showProgress) {
  int valueFontSize = 40;
  int valueY = vPad + 24;

  painter.setPen(Theme::palette().bgHeader);
  QFont valueFont(QStringLiteral("JetBrains Mono"), valueFontSize, QFont::DemiBold);
  valueFont.setLetterSpacing(QFont::AbsoluteSpacing, -1);
  painter.setFont(valueFont);
  QRect valueRect(x0, valueY, width() - x0 - hPad, valueFontSize + 4);
  painter.drawText(valueRect, Qt::AlignLeft | Qt::AlignVCenter, m_value);
  int valueWidth = painter.fontMetrics().horizontalAdvance(m_value);

  if (!m_unit.isEmpty()) {
    painter.setPen(Theme::palette().textMuted);
    QFont unitFont(QStringLiteral("Segoe UI"), 14);
    painter.setFont(unitFont);
    int unitX = x0 + valueWidth + 6;
    int unitY = valueY + valueFontSize - 16;
    painter.drawText(QRect(unitX, unitY, width() - unitX - hPad, 18), Qt::AlignLeft | Qt::AlignVCenter, m_unit);
  }

  int barMaxW = qMin(120, width() - x0 - hPad);
  int barH = 6;
  int barX = x0;
  int barY = height() - 20;
  painter.setPen(Qt::NoPen);
  painter.setBrush(Theme::palette().divider);
  painter.drawRoundedRect(QRect(barX, barY, barMaxW, barH), 3, 3);
  painter.setBrush(m_accentColor);
  painter.drawRoundedRect(QRect(barX, barY, static_cast<int>(barMaxW * m_progress), barH), 3, 3);
} else {
  int valueFontSize = m_small ? 20 : 32;
  int valueY = vPad + 18;

  const QColor valueColor = m_small ? m_accentColor : Theme::palette().bgHeader;
  painter.setPen(valueColor);
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
}

QSize StatCardWidget::sizeHint() const
{
return m_showProgress ? QSize(220, 140) : QSize(156, 80);
}
