#ifndef MANEGE_H
#define MANEGE_H

#include "Injection.hpp"
#include "MotPaP.hpp"
#include "mbed.h"

enum Etats {
  INITIAL,
  TOURNE_PHOTO,
  PHOTO,
  ATTENTE_PHOTO,
  TOURNE_INJECTION,
  INJECTION,
  TOURNE_EVAC,
  FIN
};

class Manege {
public:
  Manege(MotPaP &moteur, Injection &valve, DigitalOut &pinPhotoIn,
         DigitalOut &pinPhotoOut, DigitalOut &pinLed, InterruptIn &pinCapLum);

  int init(void);

  void marche(void);

  void stop(void);

  int getEtatActuel(void);

private:
  Etats etatActuel;

  MotPaP &moteur;
  Injection &valve;

  DigitalOut &pinPhotoIn;
  DigitalOut &pinPhotoOut;
  DigitalOut &pinLed;
  InterruptIn &pinCapLum;

  bool running;
  bool reFlag_pinCapLum;
  bool feFlag_pinCapLum;

  EventQueue eventQueue;
  Thread thread;

  Timeout timeout;
  bool timedOut;

  void cycle(void);
  void handleCapLumRE();
  void handleTimeout(void);
};

#endif // MANEGE_HPP