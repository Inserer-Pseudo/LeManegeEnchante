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

    // Declaration pin valve injection
    DigitalOut pinValve(D3);

    // Declaration pin module photo
    DigitalOut pinPhotoIn(PB_1);
    DigitalOut pinPhotoOut(PC_14);

    // Declaration pin barriere immat
    DigitalOut pinLed(PA_12);
    InterruptIn pinCapLum(PA_2);

    // Instanciation moteur PaP
    MotPaP moteur(pinA1, pinA2, pinB1, pinB2);

    // Instanciation electrovanne injection
    Injection sysInject(pinValve);

    // Instanciation biblio applicative
    Manege enchante(moteur, sysInject, pinPhotoIn, pinPhotoOut, pinLed, pinCapLum);

    // Declaration pin leds IHM
    DigitalOut pinLed1(A4);
    DigitalOut pinLed2(A5);
    DigitalOut pinLed3(A6);

    pinLed1.write(1); 
    pinLed2.write(1);
    pinLed3.write(1);

    // Initialisation du systeme (homing moteur)
    pinLed.write(1);
    enchante.init();

    printf("Hello main loop\n");

    while (1) {
        
    }

    return 0;
}

// moteur.commandePas(64);
// sysInject.injecter();
