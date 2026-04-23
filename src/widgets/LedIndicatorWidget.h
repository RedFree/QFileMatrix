#pragma once

#include <QColor>
#include <QPaintEvent>
#include <QSize>
#include <QString>
#include <QTimer>
#include <QWidget>

class LedIndicatorWidget : public QWidget
{
Q_OBJECT

public:
    enum class State {
        Ok,
        Warn,
        Err,
        Off
    };
    Q_ENUM(State)

    explicit LedIndicatorWidget(QWidget *parent = nullptr);

    void setState(State state);
    void setLabel(const QString &label);
    void setSubLabel(const QString &subLabel);
    void setCompact(bool compact);

    [[nodiscard]] State state() const;
    [[nodiscard]] QString label() const;
    [[nodiscard]] QString subLabel() const;
    [[nodiscard]] bool compact() const;

    Q_PROPERTY(bool blinking READ isBlinking NOTIFY blinkingChanged)

    [[nodiscard]] bool isBlinking() const;

signals:
    void blinkingChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QColor bulbColor() const;
    QColor glowColor() const;
    void onBlinkTick();

    State m_state = State::Off;
    QString m_label;
    QString m_subLabel;
    bool m_compact = false;
    bool m_blinkOn = true;
    QTimer m_blinkTimer;
};
