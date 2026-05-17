#pragma once

#include <QPushButton>
#include <QString>

class RailIconButton : public QPushButton
{
    Q_OBJECT

public:
    enum class IconKind {
        Camera,
        Curve,
        Servo,
        History,
        Stats,
        Gauge,
        Settings,
        User,
    };

    explicit RailIconButton(IconKind kind, const QString &text = QString(), QWidget *parent = nullptr);

    static QString iconName(IconKind kind);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    IconKind m_kind;
    QString m_text;
};
