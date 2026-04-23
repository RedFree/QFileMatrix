#pragma once

#include <QDialog>
#include <QVector>

class QLabel;
class QPushButton;
class QScrollArea;

struct AlarmEntry {
    QString type;
    QString message;
    QString time;
};

class AlarmCenterDialog : public QDialog
{
Q_OBJECT

public:
    explicit AlarmCenterDialog(QWidget *parent = nullptr);

    void setAlarms(const QVector<AlarmEntry> &alarms);
    [[nodiscard]] QVector<AlarmEntry> alarms() const;

private:
    void rebuildList();

    QVector<AlarmEntry> m_alarms;
    QScrollArea *m_scrollArea = nullptr;
    QLabel *m_countLabel = nullptr;
};
