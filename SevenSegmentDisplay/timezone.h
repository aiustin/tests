#ifndef TIMEZONE_H
#define TIMEZONE_H

#include <QTimeZone>

class TimeZone;
using TimeZones = std::vector<TimeZone>;

class TimeZone
{
public:
    static TimeZones allAvailableTimeZones();
    static TimeZone fromTimeZone(const QTimeZone& tz);

public:
    QString name() const;
    QDateTime currentDateTime() const;

private:
    QTimeZone timeZoneImpl_;
};

Q_DECLARE_METATYPE(TimeZone);

#endif // TIMEZONE_H
