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

private:
    QHBoxLayout *m_rightLayout = nullptr;
    QLabel *m_titleLabel = nullptr;
};
