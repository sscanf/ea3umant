#include "gpio.h"
#include <QDebug>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

QGpio::QGpio(QObject *parent) :
    QObject(parent)
{
    init();
}

bool QGpio::init ()
{

    qDebug() << "Inicializando GPIO";
    /* open /dev/mem */
     if ((m_memfd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        qDebug() << "can't open /dev/mem \n";
        return EXIT_FAILURE;
     }

     /* mmap GPIO */
     m_gpioMap = mmap(
        NULL,             //Any adddress in our space will do
        BLOCK_SIZE,       //Map length
        PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
        MAP_SHARED,       //Shared with other processes
        m_memfd,           //File to map
        GPIO_BASE         //Offset to GPIO peripheral
     );

     close(m_memfd); //No need to keep mem_fd open after mmap

     if (m_gpioMap == MAP_FAILED) {
        qDebug () << "mmap error %d\n" << (int)m_gpioMap;//errno also set!
        return EXIT_FAILURE;
     }

     // Always use volatile pointer!
     m_pGpio = (volatile unsigned *)m_gpioMap;

     qDebug() << "GPIO Inicializado";
     return EXIT_SUCCESS;
}


void QGpio::setInput(int bit)
{
    INP_GPIO(bit);
}

void QGpio::setOutput (int bit)
{
    INP_GPIO(bit);
    OUT_GPIO(bit);
}

void QGpio::bitSet (int bit)
{
    GPIO_SET = 1<<bit;
}

void QGpio::bitClear (int bit)
{
    GPIO_CLR = 1<<bit;
}

bool QGpio::bitValue (int bit)
{
    return GPIO_READ (bit);
}

void QGpio::setAlt (int bit, int func)
{
    SET_GPIO_ALT (bit,func);
}
