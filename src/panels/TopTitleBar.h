#pragma once

#include <QList>
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
    void setActiveSection(const QString &sectionKey);

signals:
    void sectionRequested(const QString &sectionKey);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<QPushButton*> m_navButtons;
    QPushButton *m_measureButton = nullptr;
    QLabel *m_userLabel = nullptr;
};
