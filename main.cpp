#include "mbed.h"
#include "MotPaP.hpp"
#include "Manege.hpp"
#include "Injection.hpp"


int main()
{
    // Declaration pin moteur
    DigitalOut pinA1(A0);
    DigitalOut pinA2(A1);
    DigitalOut pinB1(A2);
    DigitalOut pinB2(A3);

    // Declaration pin leds IHM
    DigitalOut pinLed1(A4);
    DigitalOut pinLed2(A5);
    DigitalOut pinLed3(A6);

    // Declaration pin valve injection
    DigitalOut pinValve(D3);

    // Declaration pin module photo
    DigitalOut pinPhotoIn(D6);
    DigitalOut pinPhotoOut(D7);

    // Declaration pin barriere immat
    DigitalOut pinLed(D8);
    InterruptIn pinCapLum(D9);

    // Instanciation moteur PaP
    MotPaP moteur(pinA1, pinA2, pinB1, pinB2);

    // Instanciation electrovanne injection
    Injection sysInject(pinValve);

    // Instanciation biblio applicative
    Manege enchante(moteur, sysInject, pinPhotoIn, pinPhotoOut, pinLed, pinCapLum);

    // Initialisation du systeme
    enchante.init();


    while (1) {
        ThisThread::sleep_for(5s);
    }

    return 0;
}

// moteur.commandePas(64);
// sysInject.injecter();
