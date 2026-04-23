#pragma once

#include <QString>
#include <QWidget>

class QLabel;
class QPaintEvent;
class QPushButton;

class TopTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TopTitleBar(QWidget *parent = nullptr);

    void setUserName(const QString &userName);
    [[nodiscard]] QString userName() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *m_measureButton = nullptr;
    QLabel *m_userLabel = nullptr;
};
