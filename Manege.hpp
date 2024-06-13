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
         DigitalOut &pinPhotoOut, DigitalOut &pinLed, InterruptIn &pinCapLum, InterruptIn &bpPurge);

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
  InterruptIn &bpPurge;

  bool running;
  bool reFlag_pinCapLum;
  bool feFlag_pinCapLum;
  bool reFlag_bpPurge;
  bool feFlag_bpPurge;

  EventQueue eventQueue;
  Thread thread;

  Timeout timeout;
  Timer t;
  bool timedOut;

  void cycle(void);
  void handleCapLumRE();
  void handleCapLumFE();
  void handlePurgeRE();
  void handlePurgeFE();
  void handleTimeout(void);
};

#endif // MANEGE_HPP