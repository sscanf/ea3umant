#ifndef TRACKERAZIM_H
#define TRACKERAZIM_H

#include <QThread>
#include <QObject>
#include "gpio.h"

#define MOTOR_FORWARD     13
#define MOTOR_BACK        19

class trackerAzim : public QThread
{
    Q_OBJECT

    enum{
        STOP,
        FORWARD,
        BACK
    };

public:
    explicit trackerAzim(QObject *parent = 0);
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

#endif // TRACKERAZIM_H
