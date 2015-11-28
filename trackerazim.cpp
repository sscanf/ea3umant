
#include "trackerazim.h"

trackerAzim::trackerAzim(QObject *parent) : QThread(parent)
{
    m_position      = -1;
    m_target        = -1;

    motor(STOP);
}

void trackerAzim::position (float position)
{
    m_position = position;
}

void trackerAzim::target (float target)
{
    m_target = target;
}

void trackerAzim::motor(int action)
{
    switch (action)
    {
        case STOP:
            m_gpio.bitClear (MOTOR_FORWARD);
            m_gpio.bitClear (MOTOR_BACK);
            emit motorStopped();
        break;

        case FORWARD:
            m_gpio.bitSet   (MOTOR_FORWARD);
            m_gpio.bitClear (MOTOR_BACK);
            emit motorRunning();
        break;

        case BACK:
            m_gpio.bitClear (MOTOR_FORWARD);
            m_gpio.bitSet   (MOTOR_BACK);
            emit motorRunning();
        break;
    }
}

void trackerAzim::run()
{
    while (1)
    {
       if ( m_target>-1 && m_position>-1 )
       {
           if (m_target==m_position)
               motor(STOP);

           if (m_target> m_position)
               motor(FORWARD);

           if ( m_target<m_position)
               motor(BACK);
       }
       usleep(1000);
    }
}

