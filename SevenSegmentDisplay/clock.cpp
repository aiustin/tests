#include "clock.h"

#include <QDateTime>

Clock::Clock(Resolution res, QObject* parent /*= nullptr*/) :
    QObject(parent)
,   resolution_(res)
{
    // TODO: should depend on the resolution_
    lastValue_ = QDateTime::currentDateTimeUtc().time().minute();
    startTimer(100);
}

void Clock::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    // TODO: should depend on the resolution_
    const int current_value = QDateTime::currentDateTimeUtc().time().minute();
    if (lastValue_ != current_value) {
        emit timeChanged();
        lastValue_ = current_value;
    }
}
