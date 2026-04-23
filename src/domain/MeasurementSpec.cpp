#include "domain/MeasurementSpec.h"

QString MeasurementSpec::verdict(double value) const
{
    if (value < min || value > max) {
        return QStringLiteral("err");
    }

    if (value < min + 0.4 || value > max - 0.4) {
        return QStringLiteral("warn");
    }

    return QStringLiteral("ok");
}
