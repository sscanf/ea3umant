#ifndef GPIO_H
#define GPIO_H

#include <QtGlobal>
#include <QObject>

#define BCM2708_PERI_BASE   0x20200000
#define GPIO_BASE           (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define PAGE_SIZE           (4*1024)
#define BLOCK_SIZE          (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(m_pGpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(m_pGpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(m_pGpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_READ(g)  *(m_pGpio + 13) &= (1<<(g))

#define GPIO_SET *(m_pGpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(m_pGpio+10) // clears bits which are 1 ignores bits which are 0

class QGpio : public QObject
{
    Q_OBJECT
public:
    explicit QGpio(QObject *parent = 0);
    bool init();
    void setInput   (int bit);
    void setOutput  (int bit);
    void bitSet     (int bit);
    void bitClear   (int bit);
    bool bitValue   (int bit);
    void setAlt     (int bit, int func);

    quint8 gpioRead (int bit);


private:
    volatile unsigned   *m_pGpio;
    int                  m_memfd;
    void                *m_gpioMap;

};

#endif // GPIO_H
