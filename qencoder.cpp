#include <QThread>
#include <QDebug>
#include "qencoder.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

QEncoder::QEncoder(int bitData, int bitClock, QObject *parent) :
    QObject     (parent),
    m_bitData   (bitData),
    m_bitClock  (bitClock)
{
    qDebug() << "------------------------------- > " << m_bitData << m_bitClock;
    m_gpio.setOutput(bitClock);
    m_gpio.setInput (bitData);
    m_tiempo=1;


    int memfd;
    void *int_map;

    memfd = open("/dev/mem",O_RDWR|O_SYNC);
    if(memfd < 0)
      {
      printf("Mem open error\n");
      return;
      }

    int_map = mmap(NULL,4096,PROT_READ|PROT_WRITE,
                    MAP_SHARED,memfd,INT_BASE);
    close(memfd);

    if( int_map == MAP_FAILED)
    {
        printf("Map failed\n");
        return;
    }
    m_intrupt = (volatile unsigned *)int_map;
                // timer pointer

}

int QEncoder::interrupts(int flag)
  {
  static unsigned int sav132 = 0;
  static unsigned int sav133 = 0;
  static unsigned int sav134 = 0;

  if(flag == 0)    // disable
    {
    if(sav132 != 0)
      {
      // Interrupts already disabled so avoid printf
      return(0);
      }

    if( (*(m_intrupt+128) | *(m_intrupt+129) | *(m_intrupt+130)) != 0)
      {
      printf("Pending interrupts\n");  // may be OK but probably
      return(0);                       // better to wait for the
      }                                // pending interrupts to
                                       // clear

    sav134 = *(m_intrupt+134);
    *(m_intrupt+137) = sav134;
    sav132 = *(m_intrupt+132);  // save current interrupts
    *(m_intrupt+135) = sav132;  // disable active interrupts
    sav133 = *(m_intrupt+133);
    *(m_intrupt+136) = sav133;
    }
  else            // flag = 1 enable
    {
    if(sav132 == 0)
      {
      printf("Interrupts not disabled\n");
      return(0);
      }

    *(m_intrupt+132) = sav132;    // restore saved interrupts
    *(m_intrupt+133) = sav133;
    *(m_intrupt+134) = sav134;
    sav132 = 0;                 // indicates interrupts enabled
    }
   return(1);
}


void QEncoder::clockPulse()
{
    m_gpio.bitSet (m_bitClock);
    DelayMicrosecondsNoSleep(5);
    m_gpio.bitClear (m_bitClock);
    DelayMicrosecondsNoSleep(5);
}

//*****************************************************
//*****************************************************
//********** DELAY FOR # uS WITHOUT SLEEPING **********
//*****************************************************
//*****************************************************
//Using delayMicroseconds lets the linux scheduler decide to jump to another process.  Using this function avoids letting the
//scheduler know we are pausing and provides much faster operation if you are needing to use lots of delays.
void QEncoder::DelayMicrosecondsNoSleep (int delay_us)
{
    long int start_time;
    long int time_difference;
    struct timespec gettime_now;

    clock_gettime(CLOCK_REALTIME, &gettime_now);
    start_time = gettime_now.tv_nsec;		//Get nS value
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &gettime_now);
        time_difference = gettime_now.tv_nsec - start_time;
        if (time_difference < 0)
            time_difference += 1000000000;				//(Rolls over every 1 second)
        if (time_difference > (delay_us * 1000))		//Delay for # nS
            break;
    }
}

int QEncoder::value()
{
    int sample = getSample();
    //qDebug() << sample;

    return gray2hex(sample);
}

int QEncoder::getSample()
{
    quint16 sample=0;

    interrupts(0);
    m_gpio.bitSet (m_bitClock);

    for (int n=0;n<13;n++)
    {
        clockPulse();
        int bit = m_gpio.bitValue(m_bitData);
        //qDebug() << bit;
        sample |= bit;
        sample <<=1;
    }
    interrupts(1);
    return sample;
}

void QEncoder::setClockTime (int time)
{
    m_tiempo = time;
}

quint16 QEncoder::gray2hex (quint16 gray)
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

    return binary;
}
