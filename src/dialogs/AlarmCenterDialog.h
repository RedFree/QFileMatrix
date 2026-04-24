#pragma once

#include <QVector>
#include <QWidget>

class QLabel;
class QPushButton;
class QScrollArea;

struct AlarmEntry {
    QString type;
    QString message;
    QString time;
};

class AlarmCenterDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmCenterDialog(QWidget *parent = nullptr);

    void setAlarms(const QVector<AlarmEntry> &alarms);
    [[nodiscard]] QVector<AlarmEntry> alarms() const;

    void showPopup(const QPoint &globalPos);

private:
    void rebuildList();

    QVector<AlarmEntry> m_alarms;
    QScrollArea *m_scrollArea = nullptr;
    QLabel *m_countLabel = nullptr;
};
