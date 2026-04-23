#pragma once

#include <QString>

class MeasurementSpec
{
public:
    double min = 10.0;
    double max = 13.0;
    double target = 11.5;

    [[nodiscard]] QString verdict(double value) const;
};
