#include "mbed.h"
#include "MotPaP.hpp"
#include "Manege.hpp"
#include "Injection.hpp"


int main()
{
    // Declaration du bouton purge
    InterruptIn bpPurge(PA_8);
    bpPurge.mode(PullUp);
    
    // Declaration pin moteur
    DigitalOut pinA1(A0);
    DigitalOut pinA2(A1);
    DigitalOut pinB1(A2);
    DigitalOut pinB2(A3);

    // Declaration pin valve injection
    DigitalOut pinValve(PB_0);

    // Declaration pin module photo
    DigitalOut pinPhotoIn(PB_6);
    DigitalOut pinPhotoOut(PB_1);

    // Declaration pin barriere immat
    DigitalOut pinLed(PA_12);
    InterruptIn pinCapLum(PA_11);

    // Instanciation moteur PaP
    MotPaP moteur(pinA1, pinA2, pinB1, pinB2);
    moteur.setReduction(4096*12.5);

    // Instanciation electrovanne injection
    Injection sysInject(pinValve);

    // Instanciation biblio applicative
    Manege enchante(moteur, sysInject, pinPhotoIn, pinPhotoOut, pinLed, pinCapLum, bpPurge);

    // Declaration pin leds IHM
    DigitalOut pinLed1(A4);
    DigitalOut pinLed2(A5);
    DigitalOut pinLed3(A6);

    pinLed1.write(1); 
    pinLed2.write(1);
    pinLed3.write(1);

    // Initialisation du systeme (homing moteur)
    pinLed.write(1);
    moteur.commandePas(-5000);
    enchante.init();
    //moteur.resetCodeurAbsolu();
    //moteur.commandePas(4096*12.5);
    //printf("pas actuel : %d\n", moteur.getPasActuel());

    enchante.marche();

    printf("Hello main loop\n");

    while (1) {
        //printf("attente de chauvin en reunion, il devrait pas tarder\n");
        
        ThisThread::sleep_for(10ms);

    }

    return 0;
}

// moteur.commandePas(64);
// sysInject.injecter();
