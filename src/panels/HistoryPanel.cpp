#include "panels/HistoryPanel.h"

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPalette>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QVBoxLayout>

#include "models/MeasurementTableModel.h"
#include "theme/Theme.h"
#include "theme/Theme.h"
#include "widgets/PanelHeaderWidget.h"

namespace {
class SearchLineEdit : public QLineEdit
{
public:
    explicit SearchLineEdit(QWidget *parent = nullptr) : QLineEdit(parent)
    {
        setTextMargins(20, 0, 4, 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QLineEdit::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen pen(Theme::palette().textMuted);
        pen.setWidthF(1.4);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        const qreal cx = 10, cy = height() / 2.0;
        painter.drawEllipse(QPointF(cx, cy), 4.0, 4.0);
        painter.drawLine(QPointF(cx + 3, cy + 3), QPointF(cx + 6, cy + 6));
    }
};

class HeaderToolButton : public QPushButton
{
public:
    enum class Icon { Filter, Stats, Export, Trash };

    explicit HeaderToolButton(Icon icon, const QString &text, QWidget *parent = nullptr)
        : QPushButton(text, parent), m_icon(icon)
    {
        setFixedHeight(24);
        setCursor(Qt::PointingHandCursor);
        setStyleSheet(QStringLiteral("QPushButton{background:transparent;border:none;border-radius:6px;padding:0 8px 0 22px;color:%1;font-size:11px;}"
            "QPushButton:hover{background:%2;}")
            .arg(Theme::palette().textMuted.name(), Theme::palette().bgSunken.name()));
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPushButton::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen pen(Theme::palette().textMuted);
        pen.setWidthF(1.3);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        const qreal cx = 10, cy = height() / 2.0;
        switch (m_icon) {
        case Icon::Filter:
            painter.setBrush(Theme::palette().textMuted);
            painter.drawPolygon(QPolygonF({QPointF(cx - 4, cy - 3.5), QPointF(cx + 4, cy - 3.5), QPointF(cx + 1, cy + 0.5), QPointF(cx + 1, cy + 3.5), QPointF(cx - 1, cy + 3.5), QPointF(cx - 1, cy + 0.5)}));
            break;
        case Icon::Stats:
            painter.setBrush(Theme::palette().textMuted);
            painter.drawLine(QPointF(cx - 4, cy + 4), QPointF(cx + 4, cy + 4));
            painter.drawRect(QRectF(cx - 4, cy + 0, 2.5, 4));
            painter.drawRect(QRectF(cx - 1, cy - 4, 2.5, 8));
            painter.drawRect(QRectF(cx + 2, cy - 2, 2.5, 6));
            break;
        case Icon::Export:
            painter.drawLine(QPointF(cx, cy - 4), QPointF(cx, cy + 2));
            painter.drawLine(QPointF(cx - 2.5, cy - 1.5), QPointF(cx, cy - 4));
            painter.drawLine(QPointF(cx + 2.5, cy - 1.5), QPointF(cx, cy - 4));
            painter.drawLine(QPointF(cx - 4, cy + 3.5), QPointF(cx + 4, cy + 3.5));
            break;
        case Icon::Trash:
            painter.drawLine(QPointF(cx - 4, cy - 3), QPointF(cx + 4, cy - 3));
            painter.drawRect(QRectF(cx - 2.5, cy - 1.5, 5, 7));
            painter.drawLine(QPointF(cx - 1.5, cy - 3), QPointF(cx - 1.5, cy - 4));
            painter.drawLine(QPointF(cx + 1.5, cy - 3), QPointF(cx + 1.5, cy - 4));
            break;
        }
    }

private:
    Icon m_icon;
};

class HistoryVerdictDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

  if (option.state & QStyle::State_Selected) {
      painter->fillRect(option.rect, Theme::palette().brandWeak);
    }

  const QString verdict = index.data(Qt::DisplayRole).toString();
  QColor bg = Theme::palette().bgSunken;
  QColor border = Theme::palette().border;
  QColor fg = Theme::palette().textMuted;
  QString text = QStringLiteral("待定");

  if (verdict == QStringLiteral("ok")) {
    bg = Theme::palette().okWeak;
    border = Theme::palette().okWeak.darker(115);
    fg = Theme::palette().ok;
    text = QStringLiteral("合格");
  } else if (verdict == QStringLiteral("warn")) {
    bg = Theme::palette().warnWeak;
    border = Theme::palette().warnWeak.darker(115);
    fg = Theme::palette().warn.darker(140);
    text = QStringLiteral("临界");
  } else if (verdict == QStringLiteral("err")) {
    bg = Theme::palette().errWeak;
    border = Theme::palette().errWeak.darker(115);
    fg = Theme::palette().err;
    text = QStringLiteral("超差");
  }

        const QRect pillRect = option.rect.adjusted(6, 5, -6, -5);
        painter->setPen(border);
        painter->setBrush(bg);
        painter->drawRoundedRect(pillRect, pillRect.height() / 2.0, pillRect.height() / 2.0);

        painter->setPen(Qt::NoPen);
        painter->setBrush(fg);
        const qreal dotR = 3.0;
        const qreal dotX = pillRect.left() + pillRect.height() / 2.0;
        const qreal dotY = pillRect.center().y();
        painter->drawEllipse(QPointF(dotX, dotY), dotR, dotR);

        painter->setPen(fg);
        painter->setBrush(Qt::NoBrush);
        const QRect textRect = pillRect.adjusted(qRound(pillRect.height() / 2.0), 0, 0, 0);
        painter->drawText(textRect, Qt::AlignCenter, text);
        painter->restore();
    }
};

class HistoryNumericDelegate : public QStyledItemDelegate
{
public:
using QStyledItemDelegate::QStyledItemDelegate;

void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override
{
QStyledItemDelegate::initStyleOption(option, index);
option->font = QFont(QStringLiteral("Consolas"), option->font.pointSize());
}
};

class HistoryThickDelegate : public QStyledItemDelegate
{
public:
using QStyledItemDelegate::QStyledItemDelegate;

void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override
{
QStyledItemDelegate::initStyleOption(option, index);
option->font = QFont(QStringLiteral("Consolas"), option->font.pointSize());
const auto verdictIdx = index.sibling(index.row(), 5);
const QString verdict = verdictIdx.data(Qt::DisplayRole).toString();
const auto &p = Theme::palette();
if (verdict == QStringLiteral("ok")) {
option->palette.setColor(QPalette::Text, p.ok);
} else if (verdict == QStringLiteral("warn")) {
option->palette.setColor(QPalette::Text, p.warn);
} else if (verdict == QStringLiteral("err")) {
option->palette.setColor(QPalette::Text, p.err);
}
}
};
}

HistoryPanel::HistoryPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *header = new PanelHeaderWidget(QStringLiteral("测量记录 · 0 条"));
    m_titleLabel = header->titleLabel();
    m_titleLabel->setObjectName(QStringLiteral("historyTitleLabel"));

 auto *search = new SearchLineEdit;
 search->setObjectName(QStringLiteral("historySearchInput"));
 search->setPlaceholderText(QStringLiteral("搜索序号 / 日期..."));
 search->setFixedWidth(140);
 search->setFixedHeight(24);
 search->setStyleSheet(QStringLiteral("QLineEdit{background:%1;border:1px solid %2;border-radius:6px;padding:4px 8px;color:%3;font-size:11px;}")
 .arg(Theme::palette().bgPanel.name(), Theme::palette().border.name(), Theme::palette().text.name()));
 header->rightLayout()->addWidget(search);

 const auto makeToolButton = [](HeaderToolButton::Icon icon, const QString &name, const QString &text) {
 auto *button = new HeaderToolButton(icon, text);
 button->setObjectName(name);
 return button;
 };
 header->rightLayout()->addWidget(makeToolButton(HeaderToolButton::Icon::Filter, QStringLiteral("historyFilterButton"), QStringLiteral("筛选")));
 header->rightLayout()->addWidget(makeToolButton(HeaderToolButton::Icon::Stats, QStringLiteral("historyStatsButton"), QStringLiteral("统计")));
 header->rightLayout()->addWidget(makeToolButton(HeaderToolButton::Icon::Export, QStringLiteral("historyExportButton"), QStringLiteral("导出")));
 header->rightLayout()->addWidget(makeToolButton(HeaderToolButton::Icon::Trash, QStringLiteral("historyDeleteButton"), QStringLiteral("删除")));
    layout->addWidget(header);

    m_table = new QTableView;
    m_table->setObjectName(QStringLiteral("historyTable"));
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(28);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
 m_table->setStyleSheet(QStringLiteral("QTableView{border:1px solid %1;border-radius:8px;background:white;alternate-background-color:%2;gridline-color:%3;}"
 "QHeaderView::section{background:%4;border:none;border-bottom:1px solid %3;padding:6px;color:%5;font-size:10.5px;font-weight:600;text-transform:uppercase;letter-spacing:0.02em;}"
 "QTableView::item:selected{background:%6;color:%5;}")
 .arg(Theme::palette().border.name(), Theme::palette().bgSunken.name(), Theme::palette().divider.name(), Theme::palette().bgSunken.name(), Theme::palette().textMuted.name(),
 Theme::palette().brandWeak.name()));

auto *verdictDelegate = new HistoryVerdictDelegate(m_table);
verdictDelegate->setObjectName(QStringLiteral("historyVerdictDelegate"));
m_table->setItemDelegateForColumn(5, verdictDelegate);

auto *thickDelegate = new HistoryThickDelegate(m_table);
thickDelegate->setObjectName(QStringLiteral("historyThickDelegate"));
m_table->setItemDelegateForColumn(6, thickDelegate);

auto *numericDelegate = new HistoryNumericDelegate(m_table);
numericDelegate->setObjectName(QStringLiteral("historyNumericDelegate"));
for (int column : {7, 8, 9, 10, 12, 13}) {
        m_table->setItemDelegateForColumn(column, numericDelegate);
    }

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
