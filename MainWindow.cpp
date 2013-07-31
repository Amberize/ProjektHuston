#include "MainWindow.h"

#include <QStatusBar>
#include <QListIterator>
#include <QStringList>

#define UPDATE_KEYS_TIME 100
#define TEST_ADDRESS "192.168.103.30:5000"

MainWindow::MainWindow() {
    setWindowTitle("Projekt Huston");
    status_label = new QLabel(QString("No connection"), this);
    statusBar()->addWidget(status_label);

    central_widget = new QWidget(this);
    layout = new QGridLayout(central_widget);

    server_edit = new QLineEdit(TEST_ADDRESS, central_widget);

    connect_button = new QPushButton(QString("Connect"), central_widget);
    QObject::connect(connect_button, SIGNAL(clicked()), this, SLOT(openConnection()));

    ping_button = new QPushButton(QString("Ping"), central_widget);

    control_button = new QPushButton(QString("Start control"), central_widget);
    QObject::connect(control_button, SIGNAL(clicked()), this, SLOT(startControl()));

    throttle_label = new QLabel(QString("Throttle"), central_widget);

    throttle_edit = new QLineEdit(QString("1500"), central_widget);

    roll_label = new QLabel(QString("Roll"), central_widget);

    roll_edit = new QLineEdit(QString("1500"), central_widget);

    pitch_label = new QLabel(QString("Pitch"), central_widget);

    pitch_edit = new QLineEdit(QString("1500"), central_widget);

    yaw_label = new QLabel(QString("Yaw"), central_widget);

    yaw_edit = new QLineEdit(QString("1500"), central_widget);

    layout->addWidget(server_edit, 0, 0);
    layout->addWidget(connect_button, 0, 1);
    layout->addWidget(ping_button, 0, 2);
    layout->addWidget(control_button, 1, 2);

    layout->addWidget(throttle_label, 1, 0);
    layout->addWidget(throttle_edit, 1, 1);

    layout->addWidget(roll_label, 2, 0);
    layout->addWidget(roll_edit, 2, 1);

    layout->addWidget(pitch_label, 3, 0);
    layout->addWidget(pitch_edit, 3, 1);

    layout->addWidget(yaw_label, 4, 0);
    layout->addWidget(yaw_edit, 4, 1);

    setCentralWidget(central_widget);

    connection = new Connection(this);
    QObject::connect(ping_button, SIGNAL(clicked()), connection, SLOT(sendPing()));

    pressed_keys = new QList<int>();
    keys_timer = new QTimer(this);
    QObject::connect(keys_timer, SIGNAL(timeout()), this, SLOT(processKeys()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if( !pressed_keys->contains(key) )
    {
        pressed_keys->append(key);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int index = pressed_keys->indexOf(event->key());
    pressed_keys->removeAt(index);
}

void MainWindow::processKeys()
{
    keys_multiplier = 1;

    if( pressed_keys->contains(Qt::Key_Control) )
        keys_multiplier *= 10;

    if( pressed_keys->contains(Qt::Key_Shift) )
        keys_multiplier *= 2;

    QVector<int> values(getFlyingValues());

    int key;
    QListIterator<int> it(*pressed_keys);
    while( it.hasNext() )
    {
        key = it.next();

        switch(key)
        {
        //trottle
        case Qt::Key_Up:
            values[0] += keys_multiplier;
            break;
        case Qt::Key_Down:
            values[0] -= keys_multiplier;
            break;
        //roll
        case Qt::Key_D:
            values[1] += keys_multiplier;
            break;
        case Qt::Key_A:
            values[1] -= keys_multiplier;
            break;
        //pith
        case Qt::Key_W:
            values[2] += keys_multiplier;
            break;
        case Qt::Key_S:
            values[2] -= keys_multiplier;
            break;
        //yaw
        case Qt::Key_Right:
            values[3] += keys_multiplier;
            break;
        case Qt::Key_Left:
            values[3] -= keys_multiplier;
            break;
        }
    }

    for(int i = 0; i < 4; i++)
        if( values[i] > 2000 )
            values[i] = 2000;

    if(!pressed_keys->isEmpty())
        setFlyingValues(values);
    else
    {
        QVector<int> values(4, 1500);
        setFlyingValues(values);
    }

    if( connection->isValid() )
        connection->sendFlyingData(values);
}

QVector<int> MainWindow::getFlyingValues()
{
    QVector<int> values;

    values.append(throttle_edit->displayText().toInt());
    values.append(roll_edit->displayText().toInt());
    values.append(pitch_edit->displayText().toInt());
    values.append(yaw_edit->displayText().toInt());

    return values;
}

void MainWindow::setFlyingValues(QVector<int> values)
{
    throttle_edit->setText(QString::number(values[0]));
    roll_edit->setText(QString::number(values[1]));
    pitch_edit->setText(QString::number(values[2]));
    yaw_edit->setText(QString::number(values[3]));
}

void MainWindow::openConnection()
{
    QStringList address = server_edit->displayText().split(":");
    connection->connectToHost(address.at(0), address.at(1).toInt());

    connect_button->setText("Disconnect");
    QObject::disconnect(connect_button, SIGNAL(clicked()), this, SLOT(openConnection()));
    QObject::connect(connect_button, SIGNAL(clicked()), this, SLOT(closeConnection()));
}

void MainWindow::closeConnection()
{
    connection->close();

    connect_button->setText("Connect");
    QObject::disconnect(connect_button, SIGNAL(clicked()), this, SLOT(closeConnection()));
    QObject::connect(connect_button, SIGNAL(clicked()), this, SLOT(openConnection()));
}

void MainWindow::startControl()
{
    keys_timer->start(UPDATE_KEYS_TIME);

    control_button->setText("Stop control");
    QObject::disconnect(control_button, SIGNAL(clicked()), this, SLOT(startControl()));
    QObject::connect(control_button, SIGNAL(clicked()), this, SLOT(stopControl()));

    this->setFocus();
}

void MainWindow::stopControl()
{
    keys_timer->stop();

    control_button->setText("Start contol");
    QObject::connect(control_button, SIGNAL(clicked()), this, SLOT(startControl()));
    QObject::disconnect(control_button, SIGNAL(clicked()), this, SLOT(stopControl()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if( connection->isOpen() )
    {
        connection->close();
    }
}
