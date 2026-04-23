#pragma once

#include <QColor>
#include <QPaintEvent>
#include <QSize>
#include <QString>
#include <QWidget>

class StatCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatCardWidget(QWidget *parent = nullptr);

    void setLabel(const QString &label);
    void setValue(const QString &value);
    void setUnit(const QString &unit);
    void setMetaText(const QString &metaText);
    void setAccentColor(const QColor &accentColor);

    [[nodiscard]] QString label() const;
    [[nodiscard]] QString value() const;
    [[nodiscard]] QString unit() const;
    [[nodiscard]] QString metaText() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QString m_label;
    QString m_value;
    QString m_unit;
    QString m_metaText;
    QColor m_accentColor;
};
