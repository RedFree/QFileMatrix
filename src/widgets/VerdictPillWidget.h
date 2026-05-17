#pragma once

#include <QWidget>

class VerdictPillWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VerdictPillWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
};
