#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include/SDL/SDL.h"
#include "ControllerData.h"
#include "Connection.h"
#include "Defines.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QStatusBar>


class MainWindow: public QMainWindow
{
    Q_OBJECT
private:
    QWidget              *m_cWidget;
    QGridLayout          *m_cLayout;
    QLabel               *m_cStatusLabel;

    QLineEdit            *m_cServerEdit;
    QPushButton          *m_cBtnConnect;
    QPushButton          *m_cBtnPing;
    QPushButton          *m_cBtnControl;

    QProgressBar         *m_ceThrottle;
    QProgressBar         *m_ceRoll;
    QProgressBar         *m_cePitch;
    QProgressBar         *m_ceYaw;

    int                   m_aData[4];
    Connection           *m_cConnection;
    SDL_Joystick*         m_cJoystick;
    ControllerData*       m_cData;

    void normolizeData();

private slots:
    void openConnection();
    void closeConnection();
    void startControl();
    void stopControl();

protected:
    void closeEvent(QCloseEvent *event);

public:
    MainWindow();
    ~MainWindow();

public slots:
    void updateData(int* data);

signals:
    void stopUpdate();
};

#endif // MAINWINDOW_H
