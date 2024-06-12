#ifndef MOTPAP_H
#define MOTPAP_H

#include "mbed.h"
#include <cstdint>

#define PAS_PAR_TOUR 128

class MotPaP {
public:
    MotPaP(DigitalOut& bobineA1, DigitalOut& bobineA2, DigitalOut& bobineB1, DigitalOut& bobineB2);

    void tourner(bool);

    void PosAZero();
    int stopMove(void);
    void commandePas(int pas);
    int off(void);
    void commandeAngle(float angle);
    int getPasActuel(void);
    float getAngleActuel(void);

    int prepCommandePas(int pas);
    int execCommandePas(void);

private:
    DigitalOut& bobineA1;
    DigitalOut& bobineA2;
    DigitalOut& bobineB1;
    DigitalOut& bobineB2;

    Timer t;
    uint32_t tempsPasPrecedent;

    int posActuelle;
    int etapePas;
    int phases;
    int pasRestants;


    void demiPas(int);
    int angleEnPas(float angle);
    void setOutputs(void);
};

#endif // MOTPAP_H