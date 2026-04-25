#include "theme/Theme.h"

#include <cmath>

namespace Theme {

QColor oklch(qreal l, qreal c, qreal h)
{
    qreal hRad = h * 3.14159265358979323846 / 180.0;
    qreal C = c * l;
    qreal a = C * std::cos(hRad);
    qreal b = C * std::sin(hRad);
    qreal r_ = l + 0.396337777 * a + 0.215485757 * b;
    qreal g_ = l + 0.100561486 * a + 0.854903528 * b;
    qreal b_ = l + 0.470504760 * a + 0.185030621 * b;
    r_ = std::fmax(0.0, std::fmin(1.0, r_));
    g_ = std::fmax(0.0, std::fmin(1.0, g_));
    b_ = std::fmax(0.0, std::fmin(1.0, b_));
    return QColor(static_cast<int>(r_ * 255), static_cast<int>(g_ * 255), static_cast<int>(b_ * 255));
}

const Palette &palette()
{
    static const Palette paletteValue{
        QColor("#F4F6F9"),
        QColor("#FFFFFF"),
        QColor("#FAFBFC"),
        QColor("#EEF1F5"),
        QColor("#0F1B2D"),
        QColor("#E2E6EC"),
        QColor("#C9D0DA"),
        QColor("#EEF0F4"),
        QColor("#0B1220"),
        QColor("#2A3240"),
        QColor("#5B6573"),
        QColor("#8A93A1"),
        oklch(0.56, 0.16, 245),
        oklch(0.46, 0.17, 245),
        oklch(0.95, 0.04, 245),
        oklch(0.64, 0.17, 150),
        oklch(0.95, 0.06, 150),
        oklch(0.76, 0.17, 85),
        oklch(0.96, 0.08, 85),
        oklch(0.60, 0.22, 25),
        oklch(0.96, 0.07, 25),
        oklch(0.58, 0.22, 15),
        oklch(0.55, 0.19, 260)
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