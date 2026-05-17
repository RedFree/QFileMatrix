#pragma once

#include <QPushButton>

class SmallIconButton : public QPushButton
{
    Q_OBJECT

public:
    enum class Icon { Play, Pause, Expand };

    explicit SmallIconButton(Icon icon, const QString &text, QWidget *parent = nullptr);

    void setIconType(Icon icon);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Icon m_icon;
};
