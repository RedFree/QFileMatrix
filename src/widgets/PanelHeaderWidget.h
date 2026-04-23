#pragma once

#include <QWidget>

class QHBoxLayout;
class QLabel;

class PanelHeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PanelHeaderWidget(const QString &title, QWidget *parent = nullptr);

    QHBoxLayout *rightLayout() const;
    QLabel *titleLabel() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *m_rightLayout = nullptr;
    QLabel *m_titleLabel = nullptr;
};
