#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moon2.h"
#include "QDebug"
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pTimer = new QTimer (this);
    connect (m_pTimer, SIGNAL(timeout()), this, SLOT (on_timer()));
    m_pTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timer()
{
    QTime   time = QTime::currentTime();
    QDate   date = QDate::currentDate();
    int     hour = time.hour();
    int     minute = time.minute();
    double  RA, Dec, topRA, topDec, LST, HA, Az, El, dist;
    double  UT = hour+minute/60.;

    moon2(date.year(), date.month(), date.day(),
          UT,
          1.96, 41.27,
          &RA, &Dec, &topRA, &topDec, &LST, &HA, &Az, &El, &dist);

    qDebug() << Az << El;

}
