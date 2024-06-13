#include "MotPaP.hpp"

MotPaP::MotPaP(DigitalOut &bobineA1, DigitalOut &bobineA2, DigitalOut &bobineB1, DigitalOut &bobineB2)
    : bobineA1(bobineA1), bobineA2(bobineA2), bobineB1(bobineB1), bobineB2(bobineB2)
{
    etapePas = 0;
    posActuelle = 0;
    t1.start();  // Lance le compteur lancement programme
}

void MotPaP::resetCodeurAbsolu(void)
{
    posActuelle = 0;
}

void MotPaP::commandePas(int pas)
{
    // moteur : 2048 pas par tour soit 4096 demi pas par tour

    pasRestants = pas;
    while (pasRestants != 0)
    {
        tempsExec1 = t1.elapsed_time().count(); // Temps ecoule en microsecondes

        if (tempsExec1 - tempsPasPrecedent1 >= 1000) {
            if (pasRestants > 0)
            {
                demiPas(1);
                pasRestants--;
                majPosActuelle(true);
            }
            else
            {
                demiPas(-1);
                pasRestants++;
                majPosActuelle(false);
            }
        tempsPasPrecedent1 = tempsExec1;
        }
    }
}

int MotPaP::prepCommandePas(int pas)
{
    pasRestants = pas;
    return pasRestants;
}

int MotPaP::execCommandePas(void)
{
    tempsExec2 = t1.elapsed_time().count(); // Temps ecoule en microsecondes

    if (pasRestants == 0)
        return -1;

    if (tempsExec2 - tempsPasPrecedent2 >= 800) {
        if (pasRestants > 0)
        {
            demiPas(1);
            pasRestants--;
            majPosActuelle(true);
        }
        else
        {
            demiPas(-1);
            pasRestants--;
            majPosActuelle(false);
        }
        tempsPasPrecedent2 = tempsExec2;
    }
    return 0;
}

int MotPaP::resetPasRestants(void)
{
    pasRestants = 0;
    return 0;
}

void MotPaP::commandeAngle(float angle)
{
    int pas = angleEnPas(angle);
    commandePas(pas);
}

void MotPaP::demiPas(int direction)
{
    etapePas += (direction);

    if (etapePas < 0)
        etapePas = 7;
    if (etapePas > 7)
        etapePas = 0;

    switch (etapePas)
    {
    case 0:
        phases = 0x1;
        break; // 0001
    case 1:
        phases = 0x3;
        break; // 0011
    case 2:
        phases = 0x2;
        break; // 0010
    case 3:
        phases = 0x6;
        break; // 0110
    case 4:
        phases = 0x4;
        break; // 0100
    case 5:
        phases = 0xC;
        break; // 1100
    case 6:
        phases = 0x8;
        break; // 1000
    case 7:
        phases = 0x9;
        break; // 1001

    default:
        phases = 0x0;
        break; // 0000
    }

    setOutputs();
}

int MotPaP::relacherMoteur(void)
{
    bobineA1 = 0;
    bobineA2 = 0;
    bobineB1 = 0;
    bobineB2 = 0;

    return 0;
}

int MotPaP::angleEnPas(float angle)
{
    float pasParTour = 360.0 / 1.8; // 1.8° par pas
    return static_cast<int>(angle * pasParTour / 360.0);
}

float MotPaP::pasEnAngle(int pas)
{
    // float pasParTour = 360.0 / 1.8; // 1.8° par pas
    // return static_cast<int>(angle * pasParTour / 360.0);
    return 0;
}

void MotPaP::setOutputs(void)
{
    bobineA1 = (phases & 0x1) > 0; // Prend le bit le plus à droite
    bobineA2 = (phases & 0x2) > 0; // Prend le deuxième bit à partir de la droite
    bobineB1 = (phases & 0x4) > 0; // Prend le troisième bit à partir de la droite
    bobineB2 = (phases & 0x8) > 0; // Prend le quatrième bit à partir de la droite
}

void MotPaP::majPosActuelle(bool ajoute)
{
    if(ajoute){
        if(posActuelle + 1 > posMax) posActuelle = 0;
        posActuelle ++;
    } else { 
        if(posActuelle == 0) posActuelle = posMax;
        posActuelle --;
    }
}

int MotPaP::getPasActuel(void)
{
    return posActuelle;
}

void MotPaP::setReduction(float reduc){
    this->reduction = reduc;
}
