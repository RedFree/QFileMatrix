#pragma once

#include <QColor>
#include <QPaintEvent>
#include <QSize>
#include <QString>
#include <QWidget>

class StatCardWidget : public QWidget
{
Q_OBJECT
Q_PROPERTY(bool small READ isSmall WRITE setSmall NOTIFY smallChanged)
Q_PROPERTY(int accentWidth READ accentWidth CONSTANT)
Q_PROPERTY(int borderRadius READ borderRadius CONSTANT)

public:
explicit StatCardWidget(QWidget *parent = nullptr);

void setLabel(const QString &label);
void setValue(const QString &value);
void setUnit(const QString &unit);
void setMetaText(const QString &metaText);
void setAccentColor(const QColor &accentColor);
void setTarget(const QString &target);
  void setTrend(double trend);
  void setSmall(bool small);
  void setShowProgress(bool show);
  void setProgress(double progress);

  [[nodiscard]] QString label() const;
[[nodiscard]] QString value() const;
[[nodiscard]] QString unit() const;
[[nodiscard]] QString metaText() const;
[[nodiscard]] QColor accentColor() const;
[[nodiscard]] QString target() const;
  [[nodiscard]] double trend() const;
  [[nodiscard]] bool isSmall() const;
  [[nodiscard]] bool showProgress() const;
  [[nodiscard]] double progress() const;
  [[nodiscard]] int accentWidth() const;
  [[nodiscard]] int borderRadius() const;

Q_SIGNAL void smallChanged();

protected:
void paintEvent(QPaintEvent *event) override;
QSize sizeHint() const override;

private:
QString m_label;
QString m_value;
QString m_unit;
QString m_metaText;
QColor m_accentColor;
QString m_target;
double m_trend = 0.0;
  bool m_small = false;
  bool m_showProgress = false;
  double m_progress = 0.0;
};
