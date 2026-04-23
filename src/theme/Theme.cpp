#include "theme/Theme.h"

namespace Theme {

const Palette &palette()
{
    static const Palette paletteValue{
        QColor("#F4F6F9"),
        QColor("#FFFFFF"),
        QColor("#FAFBFC"),
        QColor("#EEF1F5"),
        QColor("#0F1B2D"),
        QColor("#E2E6EC"),
        QColor("#EEF0F4"),
        QColor("#0B1220"),
        QColor("#5B6573"),
        QColor("#5070D7"),
        QColor("#35A167"),
        QColor("#C28B21"),
        QColor("#C65C35")
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
        .arg(p.bgPanel.name(), p.border.name(), p.text.name());
}

QString buttonStyle()
{
    const auto &p = palette();
    return QStringLiteral("QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:6px 10px;color:%3;}"
                          "QPushButton:hover{background:%4;}"
                          "QPushButton#primaryButton{background:%5;color:white;border-color:%5;}")
        .arg(p.bgPanel.name(), p.border.name(), p.text.name(), p.bgSunken.name(), p.brand.name());
}

QString railButtonStyle(bool active)
{
    const auto &p = palette();
    if (active) {
        return QStringLiteral(
                   "QPushButton{background:#EAF0FF;border:1px solid #D8E3FF;border-radius:8px;padding:4px;color:%1;font-size:10px;font-weight:700;}"
                   "QPushButton:hover{background:#E3EBFF;}"
               )
            .arg(p.brand.name());
    }

    return QStringLiteral(
               "QPushButton{background:transparent;border:1px solid transparent;border-radius:8px;padding:4px;color:%1;font-size:10px;font-weight:600;}"
               "QPushButton:hover{background:%2;border-color:%3;color:%4;}"
           )
        .arg(p.textMuted.name(), p.bgPanel.name(), p.border.name(), p.text.name());
}

}
