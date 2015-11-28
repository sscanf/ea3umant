#ifndef QENCREADER_H
#define QENCREADER_H

#include <QObject>
#include <QThread>
#include "gpio.h"

#define DATA_ELEV    27
#define DATA_AZIM    17
#define CLOCK        22

class QEncReader : public QThread
{
    Q_OBJECT
public:
    explicit QEncReader(QObject *parent = 0);
    void     run ();
    int      getElevacion();
    int      getAzimut();

private:
    void    clockPulse();
    quint16 gray2hex (quint16 gray);
    quint16 getSample(int channel);

private:
    int     m_encoderAz;
    int     m_encoderEl;
    QGpio   m_gpio;

signals:

    void sample(int data);

public slots:
};

#endif // QENCREADER_H
