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
  QColor borderStrong;
  QColor divider;
  QColor text;
  QColor textMuted;
  QColor brand;
  QColor brandStrong;
  QColor brandWeak;
  QColor ok;
  QColor okWeak;
  QColor warn;
  QColor warnWeak;
  QColor err;
  QColor errWeak;
};

const Palette &palette();
QString frameStyle();
QString titleStyle();
QString fieldStyle();
QString buttonStyle();
QString railButtonStyle(bool active);

}
