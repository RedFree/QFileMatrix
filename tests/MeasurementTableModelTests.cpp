#include <QtTest/QTest>

#include <QBrush>

#include "models/MeasurementTableModel.h"
#include "theme/Theme.h"

class MeasurementTableModelTests : public QObject
{
    Q_OBJECT

private slots:
    void setRecordsReflectsRowCountAndDisplayData();
    void prependRecordAddsRowAtTop();
    void providesForegroundAndAlignmentRoles();
};

void MeasurementTableModelTests::setRecordsReflectsRowCountAndDisplayData()
{
    MeasurementTableModel model;
    QVector<MeasurementRecord> rows;
    rows.push_back(MeasurementRecord{1010, 3, 24, 1, QStringLiteral("合格"), 11.5, 10.8, 12.2, -0.1, 0.2, 1, QStringLiteral("2026-04-22"), QStringLiteral("12:34:56"), QStringLiteral("ok")});

    model.setRecords(rows);

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.columnCount(), 14);
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole).toInt(), 1010);
    QCOMPARE(model.data(model.index(0, 5), Qt::DisplayRole).toString(), QStringLiteral("ok"));
}

void MeasurementTableModelTests::prependRecordAddsRowAtTop()
{
    MeasurementTableModel model;
    model.setRecords({MeasurementRecord{1009, 3, 23, 1, QStringLiteral("合格"), 11.4, 10.7, 12.0, 0.1, -0.1, 1, QStringLiteral("2026-04-22"), QStringLiteral("12:33:33"), QStringLiteral("warn")}});

    model.prependRecord(MeasurementRecord{1010, 3, 24, 1, QStringLiteral("自动"), 11.6, 10.9, 12.3, 0.0, 0.3, 1, QStringLiteral("2026-04-22"), QStringLiteral("12:34:56"), QStringLiteral("ok")});

    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole).toInt(), 1010);
    QCOMPARE(model.data(model.index(1, 0), Qt::DisplayRole).toInt(), 1009);
}

void MeasurementTableModelTests::providesForegroundAndAlignmentRoles()
{
    MeasurementTableModel model;
    model.setRecords({MeasurementRecord{1010, 3, 24, 1, QStringLiteral("自动"), 11.6, 10.9, 12.3, 0.0, 0.3, 1, QStringLiteral("2026-04-22"), QStringLiteral("12:34:56"), QStringLiteral("err")}});

    const auto alignment = model.data(model.index(0, 6), Qt::TextAlignmentRole);
    const auto foreground = model.data(model.index(0, 5), Qt::ForegroundRole);

    QVERIFY(alignment.isValid());
    QVERIFY(foreground.isValid());
    QCOMPARE(alignment.toInt(), static_cast<int>(Qt::AlignRight | Qt::AlignVCenter));
    QCOMPARE(foreground.value<QBrush>().color(), Theme::palette().err);
}

QTEST_APPLESS_MAIN(MeasurementTableModelTests)

#include "MeasurementTableModelTests.moc"
