#include "trackerElev.h"

trackerElev::trackerElev(QObject *parent) : QThread(parent)
{
    m_position      = -1;
    m_target        = -1;

    motor(STOP);
}

void trackerElev::position (float position)
{
    m_position = position;
}

void trackerElev::target (float target)
{
    m_target = target;
}

void trackerElev::motor(int action)
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

void trackerElev::run()
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
