#pragma once

#include <QAbstractTableModel>
#include <QVector>

#include "domain/MeasurementRecord.h"

class MeasurementTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MeasurementTableModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setRecords(const QVector<MeasurementRecord> &records);
    void prependRecord(const MeasurementRecord &record);
    [[nodiscard]] MeasurementRecord recordAt(int row) const;

private:
    QVector<MeasurementRecord> m_records;
};
