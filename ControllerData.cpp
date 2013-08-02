#include "ControllerData.h"
#include "Defines.h"

ControllerData::ControllerData(SDL_Joystick *joy, QObject *parent)
{
  m_cJoystick = joy;
  m_bStop = false;
}

ControllerData::~ControllerData()
{
}

void ControllerData::stop()
{
  m_bStop = true;
}

void ControllerData::run()
{
  m_bStop = false;
  while(!m_bStop)
  {
    int* data;
    int axes = SDL_JoystickNumAxes(m_cJoystick);
    data = new int[axes];
    SDL_JoystickUpdate();
    for( int i = 0; i < axes; ++i )
    {
      data[i] = SDL_JoystickGetAxis( m_cJoystick, i );
    }
    emit dataUpdated( data );
    SDL_Delay( I_DATA_UPDATE_TIME );
  }
}
