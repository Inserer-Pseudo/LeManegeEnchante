#include "Injection.hpp"

Injection::Injection(DigitalOut& valvePin) : valvePin(valvePin){
    valvePin.write(0);
    dureeGoutte = 20;
}

void Injection::closeValve(void){
    valvePin.write(0);
    }


void Injection::openValve(void){
    valvePin.write(1);
}

bool Injection::injecter(void){
    openValve();
    ThisThread::sleep_for(dureeGoutte);
    closeValve();
    return true;
}

void Injection::setDureeGoutte(int dureeGoutte){
    this->dureeGoutte = dureeGoutte;
}


