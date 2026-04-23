#include "panels/HistoryPanel.h"

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

#include "models/MeasurementTableModel.h"
#include "theme/Theme.h"

HistoryPanel::HistoryPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    auto *header = new QHBoxLayout;
    header->setContentsMargins(0, 0, 0, 0);
    header->setSpacing(8);
    m_titleLabel = new QLabel(QStringLiteral("测量记录 · 0 条"));
    m_titleLabel->setObjectName(QStringLiteral("historyTitleLabel"));
    m_titleLabel->setStyleSheet(QStringLiteral("font-size:12px;font-weight:600;color:%1;").arg(Theme::palette().text.name()));
    header->addWidget(m_titleLabel);
    header->addStretch();

    auto *search = new QLineEdit;
    search->setObjectName(QStringLiteral("historySearchInput"));
    search->setPlaceholderText(QStringLiteral("搜索序号 / 日期..."));
    search->setFixedWidth(150);
    search->setFixedHeight(26);
    search->setStyleSheet(QStringLiteral("QLineEdit{background:%1;border:1px solid %2;border-radius:6px;padding:4px 8px;color:%3;font-size:11px;}")
                              .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text.name()));
    header->addWidget(search);

    const auto makeToolButton = [](const QString &name, const QString &text) {
        auto *button = new QPushButton(text);
        button->setObjectName(name);
        button->setFixedHeight(26);
        button->setStyleSheet(QStringLiteral("QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:5px 10px;color:%3;font-size:11px;}"
                                             "QPushButton:hover{background:%4;}")
                                  .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text.name(), Theme::palette().bgSunken.name()));
        return button;
    };
    header->addWidget(makeToolButton(QStringLiteral("historyFilterButton"), QStringLiteral("筛选")));
    header->addWidget(makeToolButton(QStringLiteral("historyStatsButton"), QStringLiteral("统计")));
    header->addWidget(makeToolButton(QStringLiteral("historyExportButton"), QStringLiteral("导出")));
    header->addWidget(makeToolButton(QStringLiteral("historyDeleteButton"), QStringLiteral("删除")));
    layout->addLayout(header);

    m_table = new QTableView;
    m_table->setObjectName(QStringLiteral("historyTable"));
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(28);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
    m_table->setStyleSheet(QStringLiteral("QTableView{border:1px solid %1;border-radius:8px;background:white;alternate-background-color:%2;gridline-color:%3;}"
                                        "QHeaderView::section{background:%4;border:none;border-bottom:1px solid %3;padding:6px;color:%5;font-size:11px;font-weight:600;}"
                                        "QTableView::item:selected{background:#EEF2FF;color:%5;}")
                               .arg(Theme::palette().border.name(), Theme::palette().bgSunken.name(), Theme::palette().divider.name(), Theme::palette().bgRail.name(), Theme::palette().text.name()));

    layout->addWidget(m_table);
}

void HistoryPanel::setModel(MeasurementTableModel *model)
{
    m_table->setModel(model);
    if (model != nullptr) {
        connect(model, &QAbstractItemModel::rowsInserted, this, &HistoryPanel::handleRowsInserted);
        connect(model, &QAbstractItemModel::modelReset, this, &HistoryPanel::handleModelReset);
    }
    if (m_table->selectionModel() != nullptr) {
        connect(m_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
                this, &HistoryPanel::handleCurrentRowChanged);
    }
    if (model != nullptr && model->rowCount() > 0) {
        m_table->selectRow(0);
        emit currentRowChanged(0);
    }
    updateTitle();
}

MeasurementTableModel *HistoryPanel::model() const
{
    return qobject_cast<MeasurementTableModel*>(m_table->model());
}

QTableView *HistoryPanel::tableView() const
{
    return m_table;
}

void HistoryPanel::handleCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (!current.isValid()) {
        return;
    }

    emit currentRowChanged(current.row());
}

void HistoryPanel::handleRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)
    updateTitle();
}

void HistoryPanel::handleModelReset()
{
    updateTitle();
}

void HistoryPanel::updateTitle()
{
    const int rows = m_table->model() == nullptr ? 0 : m_table->model()->rowCount();
    m_titleLabel->setText(QStringLiteral("测量记录 · %1 条").arg(rows));
}
