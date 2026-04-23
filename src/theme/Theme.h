#pragma once

#include <QColor>
#include <QString>

namespace Theme {

struct Palette {
    QColor bgApp;
    QColor bgPanel;
    QColor bgRail;
    QColor bgSunken;
    QColor bgHeader;
    QColor border;
    QColor divider;
    QColor text;
    QColor textMuted;
    QColor brand;
    QColor ok;
    QColor warn;
    QColor err;
};

const Palette &palette();
QString frameStyle();
QString titleStyle();
QString fieldStyle();
QString buttonStyle();
QString railButtonStyle(bool active);

}
