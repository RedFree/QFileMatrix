#include "widgets/VerdictPillWidget.h"

#include <QPainter>

#include "theme/Theme.h"

VerdictPillWidget::VerdictPillWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(20);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setProperty("verdict", QStringLiteral("ok"));
}

QSize VerdictPillWidget::sizeHint() const
{
    return QSize(52, 20);
}

void VerdictPillWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QString v = property("verdict").toString();
    QColor bg = Theme::palette().bgSunken;
    QColor border = Theme::palette().border;
    QColor fg = Theme::palette().textMuted;
    QString text = QStringLiteral("待定");

    if (v == QStringLiteral("ok")) {
        bg = Theme::palette().okWeak;
        border = Theme::palette().okWeak.darker(115);
        fg = Theme::palette().ok;
        text = QStringLiteral("合格");
    } else if (v == QStringLiteral("warn")) {
        bg = Theme::palette().warnWeak;
        border = Theme::palette().warnWeak.darker(115);
        fg = Theme::palette().warn.darker(140);
        text = QStringLiteral("临界");
    } else if (v == QStringLiteral("err")) {
        bg = Theme::palette().errWeak;
        border = Theme::palette().errWeak.darker(115);
        fg = Theme::palette().err;
        text = QStringLiteral("超差");
    }

    const QRect pillRect = rect().adjusted(0, 1, 0, -1);
    painter.setPen(border);
    painter.setBrush(bg);
    painter.drawRoundedRect(pillRect, pillRect.height() / 2.0, pillRect.height() / 2.0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(fg);
    const qreal dotR = 3.0;
    const qreal dotX = pillRect.left() + pillRect.height() / 2.0;
    const qreal dotY = pillRect.center().y();
    painter.drawEllipse(QPointF(dotX, dotY), dotR, dotR);

    painter.setPen(fg);
    painter.setBrush(Qt::NoBrush);
    const QFont f(QStringLiteral("Consolas"), 9);
    painter.setFont(f);
    const QRect textRect = pillRect.adjusted(qRound(pillRect.height() / 2.0) + 2, 0, -4, 0);
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
}
