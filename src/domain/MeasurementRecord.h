#pragma once

#include <QString>

struct MeasurementRecord {
    int seq {};
    int batch {};
    int point {};
    int group {};
    QString label;
    double thick {};
    double hmin {};
    double hmax {};
    double left {};
    double right {};
    int station {};
    QString date;
    QString time;
    QString verdict;
};
