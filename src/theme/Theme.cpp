#include "theme/Theme.h"

namespace Theme {

QColor withAlpha(const QColor &color, int alpha)
{
    QColor c(color);
    c.setAlpha(alpha);
    return c;
}

const Palette &palette()
{
    static const Palette paletteValue{
        QColor("#F4F6F9"),
        QColor("#FFFFFF"),
        QColor("#FAFBFC"),
        QColor("#EEF1F5"),
        QColor("#0F1B2D"),
        QColor("#EDF1F7"),
        QColor("#B8C1D0"),
        QColor("#D3DAE6"),
        QColor(255, 255, 255, 31),
        QColor(255, 255, 255, 15),
        QColor("#0E1218"),
        QColor(6, 12, 22, 180),
        QColor(36, 42, 52, 72),
        QColor("#C3C7CF"),
        QColor(7, 10, 16, 92),
        QColor("#6A8BE6"),
        QColor(0, 0, 0, 25),
        QColor("#E2E6EC"),
        QColor("#C9D0DA"),
        QColor("#EEF0F4"),
        QColor("#0B1220"),
        QColor("#2A3240"),
        QColor("#5B6573"),
        QColor("#8A93A1"),
        QColor("#4F6BFF"),
        QColor("#334DCC"),
        QColor("#EEF2FF"),
        QColor("#36CFC9"),
        QColor("#357A4D"),
        QColor("#EAF6EE"),
        QColor("#B88311"),
        QColor("#FFF4D8"),
        QColor("#C44A38"),
        QColor("#FCE9E5"),
        QColor("#C44A38"),
        QColor("#547AFF")
    };
    return paletteValue;
}

QString frameStyle()
{
    const auto &p = palette();
    return QStringLiteral("background:%1;border:1px solid %2;border-radius:10px;")
        .arg(p.bgPanel.name(), p.border.name());
}

QString titleStyle()
{
    const auto &p = palette();
    return QStringLiteral("color:%1;font-size:11px;font-weight:600;text-transform:uppercase;")
        .arg(p.textMuted.name());
}

QString fieldStyle()
{
    const auto &p = palette();
    return QStringLiteral("QLineEdit,QSpinBox,QDoubleSpinBox{background:%1;border:1px solid %2;border-radius:6px;padding:4px 8px;color:%3;}"
        "QSpinBox::up-button,QSpinBox::down-button{width:18px;border:none;background:transparent;}")
        .arg(p.bgPanel.name(), p.borderStrong.name(), p.text1.name());
}

QString buttonStyle()
{
    const auto &p = palette();
    return QStringLiteral("QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:6px 10px;color:%3;}"
        "QPushButton:hover{background:%4;}"
        "QPushButton#primaryButton{background:%5;color:white;border-color:%6;}")
        .arg(p.bgPanel.name(), p.borderStrong.name(), p.text1.name(), p.bgSunken.name(), p.brand.name(), p.brandStrong.name());
}

QString railButtonStyle(bool active)
{
    const auto &p = palette();
    if (active) {
        return QStringLiteral(
            "QPushButton{background:%1;border:none;border-radius:6px;padding:4px;color:%2;font-size:10px;font-weight:700;}"
            "QPushButton:hover{background:%3;}"
        )
        .arg(p.brandWeak.name(), p.brand.name(), p.brandWeak.darker(105).name());
    }

    return QStringLiteral(
        "QPushButton{background:transparent;border:none;border-radius:6px;padding:4px;color:%1;font-size:10px;font-weight:600;}"
        "QPushButton:hover{background:%2;color:%3;}"
    )
    .arg(p.textMuted.name(), p.bgSunken.name(), p.text.name());
}

}
