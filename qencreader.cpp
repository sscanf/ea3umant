#include "qencreader.h"
#include <QDebug>

QEncReader::QEncReader(QObject *parent) :
    QThread(parent)
{
    m_encoderAz=0;
    m_encoderEl=0;
    m_gpio.setOutput(CLOCK);
    m_gpio.setInput (DATA_AZIM);
    m_gpio.setInput (DATA_ELEV);
}

void QEncReader::clockPulse()
{
}

quint16 QEncReader::getSample(int channel)
{
    quint16 sample=0;
    quint16 a;

    for (int n=0;n<13;n++)
    {
        m_gpio.bitSet (CLOCK);
        for (a=0;a<400;a++);
        m_gpio.bitClear (CLOCK);
        for (a=0;a<400;a++);
        sample = (sample | m_gpio.bitValue(channel)) << 1;
    }
    sample >>= 1;
    return sample;
}

quint16 QEncReader::gray2hex (quint16 gray)
{
    int n,gTemp,bTemp,tmp,tmp2;
    quint16 binary = 0xffff;

    quint16 bMask = 0x8000;
    quint16 gMask = 0x4000;

    binary = 0;
    tmp = gray & 0x8000;
    binary |= 0x7fff;

    for (n=0;n<16;n++)
    {
        bTemp = binary & bMask;
        gTemp = gray & gMask;

        tmp = gTemp << 1;

        tmp2 = tmp^bTemp;
        if (tmp2 == 0)
        {
            tmp = ~bMask >> 1;
            binary &= tmp;
        }

        bMask >>= 1;
        bMask &= 0x7fff;
        gMask >>= 1;
        gMask &= 0x7fff;
    }

    return binary-152;
}

int QEncReader::getAzimut()
{
    return m_encoderAz;
}

int QEncReader::getElevacion()
{
    return m_encoderEl;
}

void QEncReader::run()
{
    while (1)
    {
        m_gpio.bitSet (CLOCK);
        m_encoderAz = gray2hex ( getSample (DATA_AZIM));
        emit sample (m_encoderAz);
        usleep(1);
//        m_encoderEl = m_elevacion.value();
//        usleep(100);
    }
}

