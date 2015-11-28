#include "umencoder.h"
#include <QTextStream>

umencoder::umencoder(QObject *parent) : QObject(parent)
{
    m_pFile = new QFile ("/dev/umencoders");
    m_pFile->open (QIODevice::ReadOnly);
}

void umencoder::getPosition(int &azim, int &elev)
{
    QTextStream in (m_pFile);
    QString line = in.readLine();
    QTextStream (&line) >> azim >> elev;
}

void umencoder::setPosition(int azim, int elev)
{
}

void umencoder::stop()
{
}
