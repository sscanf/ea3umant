#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <qencoder.h>
#include <QFile>
#include <QStandardItem>
#include <QList>
#include "qencreader.h"
#include "trackerElev.h"
#include "trackerazim.h"


#define MINIUM_ELEV -110

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum {
        manual,
        automatic,
        error
    };

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow         *ui;
    QTimer                 *m_pTimer;
    QTimer                 *m_pTime;
    QEncReader             *m_pEncoder;
    int                     antData;
    QFile                  *m_pFile;
    float                   ant_elev;
    float                   ant_azim;
    float                   m_elev;
    float                   m_azim;
    float                   m_targetElev;
    float                   m_targetAzim;
    QStandardItemModel     *m_pModel;
    QList <QStandardItem *> m_listItems;
    int                     m_lastMinute;
    int                     m_state;
    trackerElev            *m_pTrackerElev;
    trackerAzim            *m_pTrackerAzim;

private slots:

    void showPositions       ();
    void on_ChangePosition   ();
    void on_timer            ();
    void on_time             ();
    void on_sample           (int);
    void on_autoChanged      (bool);
    void on_motorElevStopped ();
    void on_motorElevRunning ();
    void on_motorAzimStopped ();
    void on_motorAzimRunning ();

signals:

    void changePosition     ();
    void refreshList        ();

};

#endif // MAINWINDOW_H
