// Electrovanne.hpp
#ifndef ELECTROVANNE_H
#define ELECTROVANNE_H

#include "mbed.h"
#include <cstdint>

class Injection {
public:
    Injection(DigitalOut& valvePin);
    void openValve(void);
    void closeValve(void);
    bool injecter(void);

    void setDureeGoutte(int);

private:
    DigitalOut& valvePin;

    int dureeGoutte;
};

#endif // ELECTROVANNE_H
