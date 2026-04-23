#pragma once

#include <QDialog>

class QLabel;
class QPushButton;

class ManualSampleDialog : public QDialog
{
Q_OBJECT

public:
    explicit ManualSampleDialog(QWidget *parent = nullptr);

    void setPosition(const QString &x, const QString &y);
    void setProgram(const QString &program);
    void setBatch(const QString &batch);

    [[nodiscard]] QString positionX() const;
    [[nodiscard]] QString positionY() const;
    [[nodiscard]] QString program() const;
    [[nodiscard]] QString batch() const;

signals:
    void sampleConfirmed();

private:
    QLabel *m_posLabel = nullptr;
    QLabel *m_programLabel = nullptr;
    QLabel *m_batchLabel = nullptr;
    QPushButton *m_confirmButton = nullptr;
};
