#ifndef ANTPOSITIONER_H
#define ANTPOSITIONER_H

#include <QObject>
#include <QThread>
#include "gpio.h"

#define MOTOR_FORWARD    5
#define MOTOR_BACK       6

class trackerElev: public QThread
{
    Q_OBJECT

    enum{
        STOP,
        FORWARD,
        BACK
    };

public:
    explicit trackerElev(QObject *parent = 0);
    void    run         ();
    void    position    (float  position);
    void    target      (float  target);

private:
    void    motor       (int    action);

private:

    QGpio   m_gpio;
    float   m_position;
    float   m_target;

signals:
    void    motorRunning();
    void    motorStopped();

};

#endif // ANTPOSITIONER_H
