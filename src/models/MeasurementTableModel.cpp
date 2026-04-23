#include "models/MeasurementTableModel.h"

#include <QBrush>
#include <QStringList>

namespace {
constexpr int kColumnCount = 14;

QVariant displayValue(const MeasurementRecord &record, int column)
{
    switch (column) {
    case 0:
        return record.seq;
    case 1:
        return record.batch;
    case 2:
        return record.point;
    case 3:
        return record.group;
    case 4:
        return record.label;
    case 5:
        return record.verdict;
    case 6:
        return QString::number(record.thick, 'f', 3);
    case 7:
        return QString::number(record.hmin, 'f', 3);
    case 8:
        return QString::number(record.hmax, 'f', 3);
    case 9:
        return QString::number(record.left, 'f', 3);
    case 10:
        return QString::number(record.right, 'f', 3);
    case 11:
        return record.station;
    case 12:
        return record.date;
    case 13:
        return record.time;
    default:
        return {};
    }
}

QString headerLabel(int section)
{
    static const QStringList labels = {
        QStringLiteral("序号"),
        QStringLiteral("批号"),
        QStringLiteral("测量点号"),
        QStringLiteral("数据组"),
        QStringLiteral("标签"),
        QStringLiteral("判定"),
        QStringLiteral("厚度"),
        QStringLiteral("最小高度"),
        QStringLiteral("最大高度"),
        QStringLiteral("左高度"),
        QStringLiteral("右高度"),
        QStringLiteral("机台"),
        QStringLiteral("日期"),
        QStringLiteral("时间")
    };

    return labels.value(section);
}

bool isNumericColumn(int column)
{
    switch (column) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return true;
    default:
        return false;
    }
}

QVariant foregroundValue(const MeasurementRecord &record, int column)
{
    if (column != 5 && column != 6) {
        return {};
    }

    if (record.verdict == QStringLiteral("ok")) {
        return QBrush(QColor(0x14, 0x8A, 0x52));
    }
    if (record.verdict == QStringLiteral("warn")) {
        return QBrush(QColor(0xB0, 0x73, 0x00));
    }
    if (record.verdict == QStringLiteral("err")) {
        return QBrush(QColor(0xC2, 0x41, 0x0C));
    }

    return {};
}

}

MeasurementTableModel::MeasurementTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int MeasurementTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_records.size();
}

int MeasurementTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : kColumnCount;
}

QVariant MeasurementTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_records.size()) {
        return {};
    }

    const auto &record = m_records.at(index.row());

    if (role == Qt::DisplayRole) {
        return displayValue(record, index.column());
    }

    if (role == Qt::ForegroundRole) {
        return foregroundValue(record, index.column());
    }

    if (role == Qt::TextAlignmentRole) {
        if (isNumericColumn(index.column())) {
            return static_cast<int>(Qt::AlignRight | Qt::AlignVCenter);
        }
        if (index.column() == 5 || index.column() == 11) {
            return static_cast<int>(Qt::AlignCenter);
        }
    }

    return {};
}

QVariant MeasurementTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    return headerLabel(section);
}

void MeasurementTableModel::setRecords(const QVector<MeasurementRecord> &records)
{
    beginResetModel();
    m_records = records;
    endResetModel();
}

void MeasurementTableModel::prependRecord(const MeasurementRecord &record)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_records.prepend(record);
    endInsertRows();
}

MeasurementRecord MeasurementTableModel::recordAt(int row) const
{
    return m_records.value(row);
}
