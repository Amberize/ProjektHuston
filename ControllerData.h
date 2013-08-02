#ifndef CONTROLLERDATA_H
#define CONTROLLERDATA_H

#include <QtGui/QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QThread>

#include "include/SDL/SDL.h"

class ControllerData : public QThread
{
  Q_OBJECT
private:
  SDL_Joystick* m_cJoystick;
  bool          m_bStop;
protected:
  void run();

public:
  ControllerData(SDL_Joystick* joy, QObject* parent = 0);
  ~ControllerData();

public slots:
  void stop();

signals:
  void dataUpdated(int*);
};

#endif // CONTROLLERDATA_H
