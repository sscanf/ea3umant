#ifndef QENCODER_H
#define QENCODER_H

#include <QObject>
#include "gpio.h"

#define INT_BASE 0x2000B000

class QEncoder : public QObject
{
    Q_OBJECT
public:
    explicit QEncoder(int bitData, int bitClock, QObject *parent = 0);
    int value();
    void setClockTime(int tiempo);


private:

    void clockPulse();
    quint16 gray2hex (quint16 gray);
    int getSample();
    void DelayMicrosecondsNoSleep (int delay_us);
    int interrupts(int flag);
    volatile unsigned *m_intrupt;


private:
    int     m_bitData;
    int     m_bitClock;
    QGpio   m_gpio;
    int     m_tiempo;

signals:

public slots:
};

#endif // QENCODER_H
