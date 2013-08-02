#include "MainWindow.h"

MainWindow::MainWindow()
{
  for( int i = 0; i < 4; ++i )
    setWindowTitle("Projekt Huston");
  m_cStatusLabel = new QLabel("No connection", this);
  statusBar()->addWidget(m_cStatusLabel);

  m_cWidget = new QWidget(this);
  m_cLayout = new QGridLayout(m_cWidget);

  m_cServerEdit = new QLineEdit(S_TEST_ADDRESS, m_cWidget);
  m_cBtnConnect = new QPushButton("Connect", m_cWidget);
  m_cBtnPing    = new QPushButton("Ping", m_cWidget);
  m_cBtnControl = new QPushButton("Start control", m_cWidget);
  m_ceThrottle  = new QProgressBar(m_cWidget);
  m_ceRoll      = new QProgressBar(m_cWidget);
  m_cePitch     = new QProgressBar(m_cWidget);
  m_ceYaw       = new QProgressBar(m_cWidget);

  m_ceThrottle->setRange(I_MIN_DATA,I_MAX_DATA);
  m_ceRoll->setRange(I_MIN_DATA,I_MAX_DATA);
  m_cePitch->setRange(I_MIN_DATA,I_MAX_DATA);
  m_ceYaw->setRange(I_MIN_DATA,I_MAX_DATA);


  m_cBtnControl->setCheckable( true );

  m_cLayout->addWidget(m_cServerEdit, 0, 0, 1, 2);
  m_cLayout->addWidget(m_cBtnConnect, 0, 2);
  m_cLayout->addWidget(m_cBtnPing, 0, 3);
  m_cLayout->addWidget(m_cBtnControl, 1, 3);

  QLabel* tmp_lb;
  tmp_lb = new QLabel("Throttle", m_cWidget);
  tmp_lb->setFixedWidth( I_LABELS_WIDTH );
  m_cLayout->addWidget( tmp_lb, 1, 0);
  m_cLayout->addWidget(m_ceThrottle, 1, 1, 1, 2);

  m_cLayout->addWidget(new QLabel("Roll", m_cWidget), 2, 0);
  m_cLayout->addWidget(m_ceRoll, 2, 1, 1, 2);

  m_cLayout->addWidget(new QLabel("Pitch", m_cWidget), 3, 0);
  m_cLayout->addWidget(m_cePitch, 3, 1, 1, 2);

  m_cLayout->addWidget(new QLabel("Yaw", m_cWidget), 4, 0);
  m_cLayout->addWidget(m_ceYaw, 4, 1, 1, 2);

  setCentralWidget(m_cWidget);

  m_cConnection = new Connection(m_cWidget);


  //SDL
  m_cJoystick = NULL;
  m_cData = NULL;
  SDL_Init( SDL_INIT_JOYSTICK );

  if(SDL_NumJoysticks() > 0)
  {
    m_cJoystick = SDL_JoystickOpen(0);
    m_cStatusLabel->setText( "Joystick: " + QString(SDL_JoystickName(0)) );
    m_cData = new ControllerData(m_cJoystick, this);
  }

  connect( this,    SIGNAL( stopUpdate()      ),
           m_cData,   SLOT( stop()            ) );
  connect( m_cData, SIGNAL( dataUpdated(int*) ),
           this,      SLOT( updateData(int*)  ) );
  connect(m_cBtnPing, SIGNAL(clicked()), m_cConnection, SLOT(sendPing()));
  connect(m_cBtnConnect, SIGNAL(clicked()), this, SLOT(openConnection()));
  connect(m_cBtnControl, SIGNAL(clicked()), this, SLOT(startControl()));

  setFixedHeight(200);
  resize(600, height());

}

MainWindow::~MainWindow()
{
  emit stopUpdate();
  if( m_cData->isRunning() )
    while( ! m_cData->isFinished() );
  if( m_cJoystick != NULL ) SDL_JoystickClose( m_cJoystick );
  SDL_Quit();
}

void MainWindow::updateData(int *data)
{
  //throttle axis
#ifdef INVERT_THROTTLE
  if( data[I_THROTTLE_AXIS] > 0 )
    m_aData[A_THROTTLE] = I_MID_DATA + (double)(I_MID_DATA - I_MIN_DATA) * ( (double)data[I_THROTTLE_AXIS] / I_INT_MAX );
  else
    m_aData[A_THROTTLE] = I_MID_DATA + (double)(I_MAX_DATA - I_MID_DATA) * ( (double)data[I_THROTTLE_AXIS] / I_INT_MAX );
#else
  if( data[I_THROTTLE_AXIS] > 0 )
    m_aData[A_THROTTLE] = I_MID_DATA - (double)(I_MAX_DATA - I_MID_DATA) * ( (double)data[I_THROTTLE_AXIS] / I_INT_MAX );
  else
    m_aData[A_THROTTLE] = I_MID_DATA - (double)(I_MID_DATA - I_MIN_DATA) * ( (double)data[I_THROTTLE_AXIS] / I_INT_MAX );
#endif

  //yaw axis
#ifdef INVERT_YAW
  if( data[I_YAW_AXIS] > 0 )
    m_aData[A_YAW] = I_MID_DATA + (double)(I_MID_DATA - I_MIN_DATA) * ( (double)data[I_YAW_AXIS] / I_INT_MAX );
  else
    m_aData[A_YAW] = I_MID_DATA + (double)(I_MAX_DATA - I_MID_DATA) * ( (double)data[I_YAW_AXIS] / I_INT_MAX );
#else
  if( data[I_YAW_AXIS] > 0 )
    m_aData[A_YAW] = I_MID_DATA - (double)(I_MAX_DATA - I_MID_DATA) * ( (double)data[I_YAW_AXIS] / I_INT_MAX );
  else
    m_aData[A_YAW] = I_MID_DATA - (double)(I_MID_DATA - I_MIN_DATA) * ( (double)data[I_YAW_AXIS] / I_INT_MAX );
#endif

  //pitch axis
#ifdef INVERT_PITCH
  if( data[I_PITCH_AXIS] > 0 )
    m_aData[A_PITCH] = I_MID_DATA + (double)(I_MID_DATA - I_MIN_DATA) * ( (double)data[I_PITCH_AXIS] / I_INT_MAX );
  else
    m_aData[A_PITCH] = I_MID_DATA + (double)(I_MAX_DATA - I_MID_DATA) * ( (double)data[I_PITCH_AXIS] / I_INT_MAX );
#else
  if( data[I_PITCH_AXIS] > 0 )
    m_aData[A_PITCH] = I_MID_DATA - (double)(I_MAX_DATA - I_MID_DATA) * ( (double)data[I_PITCH_AXIS] / I_INT_MAX );
  else
    m_aData[A_PITCH] = I_MID_DATA - (double)(I_MID_DATA - I_MIN_DATA) * ( (double)data[I_PITCH_AXIS] / I_INT_MAX );
#endif

  //roll axis
#ifdef INVERT_ROLL
  if( data[I_ROLL_AXIS] > 0 )
    m_aData[A_ROLL] = I_MID_DATA + (double)(I_MID_DATA - I_MIN_DATA) * ( (double)data[I_ROLL_AXIS] / I_INT_MAX );
  else
    m_aData[A_ROLL] = I_MID_DATA + (double)(I_MAX_DATA - I_MID_DATA) * ( (double)data[I_ROLL_AXIS] / I_INT_MAX );
#else
  if( data[I_ROLL_AXIS] > 0 )
    m_aData[A_ROLL] = I_MID_DATA - (double)(I_MAX_DATA - I_MID_DATA) * ( (double)data[I_ROLL_AXIS] / I_INT_MAX );
  else
    m_aData[A_ROLL] = I_MID_DATA - (double)(I_MID_DATA - I_MIN_DATA) * ( (double)data[I_ROLL_AXIS] / I_INT_MAX );
#endif

  delete [] data;

  normolizeData();

  m_cConnection->sendFlyingData(m_aData);

  m_ceThrottle->setValue( m_aData[A_THROTTLE] );
  m_ceYaw->setValue( m_aData[A_YAW] );
  m_cePitch->setValue( m_aData[A_PITCH] );
  m_ceRoll->setValue( m_aData[A_ROLL] );
}

void MainWindow::openConnection()
{
  QStringList address = m_cServerEdit->displayText().split(":");
  m_cConnection->connectToHost(address.at(0), address.at(1).toInt());

  m_cBtnConnect->setText("Disconnect");
  QObject::disconnect(m_cBtnConnect, SIGNAL(clicked()), this, SLOT(openConnection()));
  QObject::connect(m_cBtnConnect, SIGNAL(clicked()), this, SLOT(closeConnection()));
}

void MainWindow::closeConnection()
{
  m_cConnection->close();

  m_cBtnConnect->setText("Connect");
  QObject::disconnect(m_cBtnConnect, SIGNAL(clicked()), this, SLOT(closeConnection()));
  QObject::connect(m_cBtnConnect, SIGNAL(clicked()), this, SLOT(openConnection()));
}

void MainWindow::startControl()
{


  m_cBtnControl->setText("Stop control");
  QObject::disconnect(m_cBtnControl, SIGNAL(clicked()), this, SLOT(startControl()));
  QObject::connect(m_cBtnControl, SIGNAL(clicked()), this, SLOT(stopControl()));

  if(m_cData != NULL)
    m_cData->start();
}

void MainWindow::stopControl()
{
  if(m_cData != NULL)
    m_cData->stop();

  m_cBtnControl->setText("Start contol");
  QObject::connect(m_cBtnControl, SIGNAL(clicked()), this, SLOT(startControl()));
  QObject::disconnect(m_cBtnControl, SIGNAL(clicked()), this, SLOT(stopControl()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if( m_cConnection->isOpen() )
  {
    m_cConnection->close();
  }
}

void MainWindow::normolizeData()
{
  for(int i = 0; i < 4; ++i)
  {
    if(m_aData[i] > I_MAX_DATA) m_aData[i] = I_MAX_DATA;
    else
    {
      if(m_aData[i] < I_MIN_DATA) m_aData[i] = I_MIN_DATA;
#ifdef STABLE_MID_ROUDING
      else
        if((double)abs(I_MID_DATA - m_aData[i]) < ((double)I_MID_DATA * D_DELTA_MID))
          m_aData[i] = I_MID_DATA;
#endif

    }
  }
}
