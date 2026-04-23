#pragma once

#include <QWidget>

class QTableView;
class MeasurementTableModel;
class QModelIndex;
class QLabel;

class HistoryPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryPanel(QWidget *parent = nullptr);

    void setModel(MeasurementTableModel *model);
    [[nodiscard]] MeasurementTableModel *model() const;
    [[nodiscard]] QTableView *tableView() const;

signals:
    void currentRowChanged(int row);

private:
    void handleCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void handleRowsInserted(const QModelIndex &parent, int first, int last);
    void handleModelReset();
    void updateTitle();

    QLabel *m_titleLabel = nullptr;
    QTableView *m_table = nullptr;
};
