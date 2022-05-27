#include "timezone.h"

static TimeZones availableTimeZones()
{
    TimeZones time_zones;
    for(const auto& ianald: QTimeZone::availableTimeZoneIds()) {
        time_zones.push_back(TimeZone::fromTimeZone(QTimeZone(ianald)));
    }
    return time_zones;
}

/*static*/
TimeZones TimeZone::allAvailableTimeZones() {
    static const TimeZones timeZones = availableTimeZones();
    return timeZones;
}

/*static*/
TimeZone TimeZone::fromTimeZone(const QTimeZone& tz) {
    TimeZone time_zone;
    time_zone.timeZoneImpl_ = tz;
    return time_zone;
}

QString TimeZone::name() const {
    //timeZoneImpl_.displayName(QTimeZone::GenericTime, QTimeZone::LongName);
    return QString::fromLatin1(timeZoneImpl_.id());
}

QDateTime TimeZone::currentDateTime() const {
    return QDateTime::currentDateTime().toTimeZone(timeZoneImpl_);
}

