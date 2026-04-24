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
    QColor("#C9D0DA"),
    QColor("#EEF0F4"),
    QColor("#0B1220"),
    QColor("#5B6573"),
    QColor("#007ACB"),
    QColor("#005AAF"),
    QColor("#D9F2FF"),
    QColor("#17A750"),
    QColor("#D3FBDA"),
    QColor("#E2A600"),
    QColor("#FFEEB4"),
    QColor("#E62B34"),
    QColor("#FFE0DA")
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
    .arg(p.bgPanel.name(), p.borderStrong.name(), p.text.name());
}

QString buttonStyle()
{
    const auto &p = palette();
  return QStringLiteral("QPushButton{background:%1;border:1px solid %2;border-radius:6px;padding:6px 10px;color:%3;}"
    "QPushButton:hover{background:%4;}"
    "QPushButton#primaryButton{background:%5;color:white;border-color:%6;}")
    .arg(p.bgPanel.name(), p.borderStrong.name(), p.text.name(), p.bgSunken.name(), p.brand.name(), p.brandStrong.name());
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
