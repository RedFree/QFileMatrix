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
  QColor headerText;
  QColor headerTextMuted;
  QColor headerTextSubtle;
  QColor headerDivider;
  QColor headerHover;
  QColor cameraBg;
  QColor cameraHud;
  QColor cameraStripe;
  QColor cameraStrip;
  QColor cameraPauseOverlay;
  QColor gaugeHighlight;
  QColor indicatorOutline;
  QColor border;
  QColor borderStrong;
  QColor divider;
  QColor text;
  QColor text1;
  QColor textMuted;
  QColor text3;
  QColor brand;
  QColor brandStrong;
  QColor brandWeak;
  QColor brandAccent;
  QColor ok;
  QColor okWeak;
  QColor warn;
  QColor warnWeak;
  QColor err;
  QColor errWeak;
  QColor accentTrace;
  QColor accentRef;
};

const Palette &palette();
QString frameStyle();
QString titleStyle();
QString fieldStyle();
QString buttonStyle();
QString railButtonStyle(bool active);

}
