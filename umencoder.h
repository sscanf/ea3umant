#ifndef UMENCODER_H
#define UMENCODER_H

#include <QObject>
#include <QFile>

class umencoder : public QObject
{
    Q_OBJECT
public:
    explicit umencoder(QObject *parent = 0);
    void getPosition(int &azim, int &elev);
    void setPosition(int azim, int elev);
    void stop();

private:
    QFile *m_pFile;


signals:

public slots:
};

#endif // UMENCODER_H
