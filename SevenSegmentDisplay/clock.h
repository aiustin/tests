#ifndef CLOCK_H
#define CLOCK_H

#include <QObject>

class Clock : public QObject
{
    Q_OBJECT
public:
    enum class Resolution
    {
        //Second,
        Minute,
        //Hour
    };
public:
    explicit Clock(Resolution res, QObject* parent = nullptr);

signals:
    void timeChanged();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    Resolution resolution_ = Resolution::Minute;
    int lastValue_ = 0;
};

#endif // CLOCK_H
