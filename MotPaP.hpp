#ifndef MOTPAP_H
#define MOTPAP_H

#include "mbed.h"
#include <cstdint>

#define posMax 51200

class MotPaP {
public:
  MotPaP(DigitalOut &bobineA1, DigitalOut &bobineA2, DigitalOut &bobineB1,
         DigitalOut &bobineB2);

  void resetCodeurAbsolu(void);
  int resetPasRestants(void);

  // Commandes synchrones
  void commandePas(int pas);
  void commandeAngle(float angle);
  // Commande asynchrone
  int prepCommandePas(int pas);
  int execCommandePas(void);

  int relacherMoteur(void);

  // Getters / Setters
  int getPasActuel(void);
  float getAngleActuel(void);
  void setReduction(float);

private:
  // Stockage interne instance bobine
  DigitalOut &bobineA1;
  DigitalOut &bobineA2;
  DigitalOut &bobineB1;
  DigitalOut &bobineB2;

  // Timer execution des pas
  Timer t1;
  uint32_t tempsExec1;
  uint32_t tempsPasPrecedent1;
  Timer t2;
  uint32_t tempsExec2;
  uint32_t tempsPasPrecedent2;

  int posActuelle;
  int etapePas;
  int phases;
  int pasRestants;

  float reduction;

  void demiPas(int);
  void majPosActuelle(bool);
  int angleEnPas(float);
  float pasEnAngle(int);
  void setOutputs(void);
};

#endif // MOTPAP_H