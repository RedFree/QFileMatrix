#pragma once

#include <QVector>

struct ProfilePoint {
    double x {};
    double y {};
};

using ProfileData = QVector<ProfilePoint>;
