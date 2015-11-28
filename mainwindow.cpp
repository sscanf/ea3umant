#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moon2.h"
#include "QDebug"
#include <QTime>
#include <QDate>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_lastMinute    = 0;
    m_state         = automatic;
    m_elev          = 0;
    m_azim          = 0;
    m_targetElev    = 0;
    m_targetAzim    = 0;

    m_pEncoder = new QEncReader(this);
    connect (m_pEncoder,     SIGNAL(sample(int)), this, SLOT (on_sample(int)), Qt::DirectConnection);
    m_pEncoder->start();

    m_pTrackerElev = new trackerElev (this);
    connect (m_pTrackerElev, SIGNAL (motorStopped()), this, SLOT (on_motorElevStopped()), Qt::DirectConnection);
    connect (m_pTrackerElev, SIGNAL (motorRunning()), this, SLOT (on_motorElevRunning()), Qt::DirectConnection);
    m_pTrackerElev->start();

    m_pTrackerAzim = new trackerAzim (this);
    connect (m_pTrackerAzim, SIGNAL (motorStopped()), this, SLOT (on_motorAzimStopped()), Qt::DirectConnection);
    connect (m_pTrackerAzim, SIGNAL (motorRunning()), this, SLOT (on_motorAzimRunning()), Qt::DirectConnection);
    m_pTrackerAzim->start();

    m_pTimer = new QTimer (this);
    connect (m_pTimer, SIGNAL(timeout()), this, SLOT (on_timer()));
    m_pTimer->start(100);

    m_pTime = new QTimer (this);
    connect (m_pTime, SIGNAL(timeout()), this, SLOT (on_time()));
    m_pTime->start(1000);

    connect (this,          SIGNAL (changePosition()),   this,   SLOT (on_ChangePosition()));
    connect (this,          SIGNAL (refreshList()),      this,   SLOT (showPositions()));
    connect (ui->switchAuto,SIGNAL (statusChanged(bool)),this,   SLOT (on_autoChanged(bool)));

    ui->w_azimut->setSkin("Tachometer-360");
    ui->w_azimut->setMinimum(0);
    ui->w_azimut->setMaximum(360);
    ui->w_azimut->setValue(0);
    ui->w_azimut->setStartAngle(0);
    ui->w_azimut->setEndAngle(360);
    ui->w_azimut->setNeedleOrigin(0.486, 0.466);
//    ui->w_azimut->setShowOverlay(false);

    ui->w_elvacion->setSkin("Tachometer-45");
    ui->w_elvacion->setMinimum(-90);
    ui->w_elvacion->setMaximum(270);
    ui->w_elvacion->setValue(1);
    ui->w_elvacion->setStartAngle(-170);
    ui->w_elvacion->setEndAngle(170);
    ui->w_elvacion->setNeedleOrigin(0.486, 0.466);
//    ui->w_elevacion->setShowOverlay(false);

    ui->switchPtt->setSkin ("Beryl");
    ui->switchAuto->setSkin ("Beryl");

//    antData = m_pEncoder->getAzimut();
//    qDebug() << antData;

    m_pFile = new QFile ("/dev/umencoders");
    m_pFile->open (QIODevice::ReadOnly);

    m_pModel = new QStandardItemModel (5,7);
    m_pModel->setHorizontalHeaderItem( 0, new QStandardItem( "TIME" ) );
    m_pModel->setHorizontalHeaderItem( 1, new QStandardItem( "AZ" ) );
    m_pModel->setHorizontalHeaderItem( 2, new QStandardItem( "EL" ) );
    //m_pModel->setHorizontalHeaderItem( 3, new QStandardItem( "RA" ) );
    m_pModel->setHorizontalHeaderItem( 3, new QStandardItem( "DEC" ) );
    //m_pModel->setHorizontalHeaderItem( 5, new QStandardItem( "TOP RA" ) );
    //m_pModel->setHorizontalHeaderItem( 6, new QStandardItem( "TOP DEC" ) );
    m_pModel->setHorizontalHeaderItem( 4, new QStandardItem( "LST" ) );
    m_pModel->setHorizontalHeaderItem( 5, new QStandardItem( "HA" ) );
    m_pModel->setHorizontalHeaderItem( 6, new QStandardItem( "DIST" ) );
    showPositions();
}

void MainWindow::showPositions()
{
    double  RA, Dec, topRA, topDec, LST, HA, Az, El, dist;

    QTime   time = QTime::currentTime();
    QDate   date = QDate::currentDate();

    time = time.addSecs(-240);

    for (int n=0;n<m_listItems.count();n++)
        delete m_listItems.at (n);
    m_listItems.clear();

    for (int n=0;n<6;n++)
    {
        int     hour = time.hour();
        int     minute = time.minute();
        double  UT = hour+minute/60.;

        moon2(date.year(), date.month(), date.day(),
              UT,
              1.96, 41.27,
              &RA, &Dec, &topRA, &topDec, &LST, &HA, &Az, &El, &dist);

        m_listItems.append (new QStandardItem (QString ("%1:%2").arg(hour).arg(minute)));
        m_listItems.append (new QStandardItem (QString::number (Az,'.',2)));
        m_listItems.append (new QStandardItem (QString::number (El,'.',2)));
        //m_listItems.append (new QStandardItem (QString::number (RA,'.',2)));
        m_listItems.append (new QStandardItem (QString::number (Dec,'.',2)));
        //m_listItems.append (new QStandardItem (QString::number (topRA,'.',2)));
        //m_listItems.append (new QStandardItem (QString::number (topDec,'.',2)));
        m_listItems.append (new QStandardItem (QString::number (LST,'.',2)));
        m_listItems.append (new QStandardItem (QString::number (HA,'.',2)));
        m_listItems.append (new QStandardItem (QString::number (dist,'.',2)));
        time = time.addSecs (120);
    }

    int x=0;
    for (int n=0;n<5;n++)
        for (int y=0;y<7;y++)
            m_pModel->setItem (n,y,m_listItems.at (x++));

    ui->treeView->setModel (m_pModel);
    QModelIndex index = m_pModel->index (2,0);
    ui->treeView->setCurrentIndex(index);
}

void MainWindow::on_autoChanged(bool bActive)
{
    if (bActive)
    {
        qDebug() << "AUTO ON";
        m_state = automatic;
    }
    else
    {
        m_state = manual;
        qDebug() << "AUTO OFF";

    }
}

void MainWindow::on_ChangePosition ()
{
    if (m_state != automatic)
        return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timer()
{
    float azim,elev;
    QTextStream in (m_pFile);
    QString line = in.readLine();

    QTextStream (&line) >> azim >> elev;
    azim = azim/2.;

    elev = elev/6.;
    elev-=10;

    if (m_state == automatic)
        ui->led_manauto->setPixmap (QPixmap (QString::fromUtf8(":/led_on")));
    else
        ui->led_manauto->setPixmap (QPixmap (QString::fromUtf8(":/led_off")));

    if (elev<0 || elev>360)
        ui->led_outRange->setPixmap (QPixmap (QString::fromUtf8(":/led_on")));
    else
        ui->led_outRange->setPixmap (QPixmap (QString::fromUtf8(":/led_off")));

    if (ant_elev<0 && elev>359)
        return;

    if (ant_elev>359 && elev<0)
        return;

    ui->azimut->setText(QString::number (azim,'.',2));
    ui->elevacion->setText (QString::number (elev,'.',2));

    ui->w_azimut->setValue (azim);
    ui->w_elvacion->setValue (elev);

    ant_elev = elev;

}

void MainWindow::on_time()
{    
    QString strDateTime;

    QDate date (QDate::currentDate());
    QTime time (QTime::currentTime());

    QString strData = QString ("%1/%2/%3").arg (date.day()).arg(date.month()).arg (date.year());
    QString strTime = QString ("%1:%2:%3").arg (time.hour()).arg(time.minute()).arg (time.second());

    strDateTime = strData + " " + strTime;
    ui->datetime->setText (strDateTime);

    if (m_lastMinute != time.minute())
    {
        m_lastMinute = time.minute();

        QModelIndex index = m_pModel->index (3,0);
        ui->treeView->setCurrentIndex(index);

        QModelIndex idx = ui->treeView->currentIndex();
        if(idx.isValid())
        {
            QStandardItem* item = m_pModel->item(idx.row(), idx.column());
            QString text = item->text();

            QString strTime = QString ("%1:%2").arg(time.hour()).arg(time.minute());

            if (strTime == text)
            {
                emit refreshList();
                emit changePosition();
            }
        }
        index = m_pModel->index (2,0);
        ui->treeView->setCurrentIndex(index);
    }
}

void MainWindow::on_sample(int data)
{
//    if ( data > antData-5 && data < antData+5)
//    {
        antData = data;
    /*    QTime   time = QTime::currentTime();
        QDate   date = QDate::currentDate();
        int     hour = time.hour();
        int     minute = me.minute();
        double  RA, Dec, topRA, topDec, LST, HA, Az, El, dist;
        double  UT = hour+minute/60.;

        moon2(date.year(), date.month(), date.day(),
              UT,
              1.96, 41.27,
              &RA, &Dec, &topRA, &topDec, &LST, &HA, &Az, &El, &dist);
    */
        float encoder_az = data/2.;
    //    float encoder_el = m_pEncoder->getElevacion()/2;

//       ui->azimut->setText(QString::number (encoder_az,'.',2));
    //    ui->elevacion->setText (QString::number (encoder_el,'.',2));

//        ui->w_azimut->setValue (encoder_az);
     //   ui->w_elvacion->setValue (encoder_el);
        //qDebug() << Az << El << m_azimut.value() << m_elevacion.value();
//    }
//    else
//        printf ("%d\n",data);
}


void MainWindow::on_motorElevStopped ()
{
    ui->led_motorEl->setPixmap (QPixmap (QString::fromUtf8(":/led_off")));
}

void MainWindow::on_motorElevRunning ()
{
    ui->led_motorEl->setPixmap (QPixmap (QString::fromUtf8(":/led_on")));
}

void MainWindow::on_motorAzimStopped ()
{
    ui->led_motorAz->setPixmap (QPixmap (QString::fromUtf8(":/led_off")));
}

void MainWindow::on_motorAzimRunning ()
{
    ui->led_motorAz->setPixmap (QPixmap (QString::fromUtf8(":/led_on")));
}
