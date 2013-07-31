#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Connection.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QList>
#include <QTimer>
#include <QVector>

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void openConnection();
    void closeConnection();

    void startControl();
    void stopControl();

    void processKeys();

private:
    QWidget *central_widget;
    QGridLayout *layout;
    QLabel *status_label;

    QLineEdit *server_edit;
    QPushButton *connect_button;
    QPushButton *ping_button;
    QPushButton *control_button;

    QLabel *throttle_label;
    QLineEdit *throttle_edit;

    QLabel *roll_label;
    QLineEdit *roll_edit;

    QLabel *pitch_label;
    QLineEdit *pitch_edit;

    QLabel *yaw_label;
    QLineEdit *yaw_edit;

    Connection *connection;

    QList<int> *pressed_keys;
    QTimer *keys_timer;
    int keys_multiplier;

    QVector<int> getFlyingValues();
    void setFlyingValues(QVector<int> values);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
