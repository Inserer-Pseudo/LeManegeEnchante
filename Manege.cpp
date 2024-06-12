#include "Manege.hpp"

Manege::Manege(MotPaP &moteur, Injection &valve, DigitalOut &pinPhotoIn,
			   DigitalOut &pinPhotoOut, DigitalOut &pinLed,
			   InterruptIn &pinCapLum)
	: etatActuel(INITIAL), moteur(moteur), valve(valve), pinPhotoIn(pinPhotoIn),
	  pinPhotoOut(pinPhotoOut), pinLed(pinLed), pinCapLum(pinCapLum)
{}

int Manege::init(void)
{
	ThisThread::sleep_for(1s);
	printf("start init\n");
	bool initDone = false;
	timedOut = false;

	pinCapLum.fall(callback(this, &Manege::handleCapLumRE));
	//pinCapLum.rise(callback([this](void){ this->handleCapLumChange(true); }));

	timeout.attach(callback(this, &Manege::handleTimeout), 10s);

	pinLed.write(1);

	moteur.prepCommandePas(200);

	while (initDone == 0 && timedOut == 0)
	{
		if (reFlag_pinCapLum)
		{
			printf("front cap\n");
			moteur.PosAZero();
			etatActuel = INITIAL;
			initDone = true;
			reFlag_pinCapLum = false;
		}

		moteur.execCommandePas();
	}

	if (timedOut)
	{
		return -1;
	}

	timeout.detach();
	printf("init done\n");
	return 0;
}

void Manege::marche()
{
	running = true;
	eventQueue.call(callback(this, &Manege::cycle));
	thread.start(callback(&eventQueue, &EventQueue::dispatch_forever));
}

void Manege::stop()
{
	running = false;
	eventQueue.break_dispatch();
}

int Manege::getEtatActuel() { return etatActuel; }

void Manege::cycle()
{
	while (running)
	{
		switch (etatActuel)
		{
		case INITIAL:
			// Tourne jusqu'a position entree si pos !=
			if (moteur.getAngleActuel() != 0)
				moteur.commandeAngle(0.0);
			// Attente de declenchement
			if (moteur.getAngleActuel() == 0 && reFlag_pinCapLum)
				reFlag_pinCapLum = false;
			etatActuel = TOURNE_PHOTO;
			break;
		case TOURNE_PHOTO:
			// Tourne jusqu'a position photo
			moteur.commandeAngle(90.0);
			ThisThread::sleep_for(3s);
			etatActuel = PHOTO;
			break;
		case PHOTO:
			// Declenchement photo
			pinPhotoOut.write(1);
			ThisThread::sleep_for(200ms);
			pinPhotoOut.write(0);
			etatActuel = ATTENTE_PHOTO;
			break;
		case ATTENTE_PHOTO:
			// Attente tempo photo
			ThisThread::sleep_for(5s);
			etatActuel = TOURNE_INJECTION;
			break;
		case TOURNE_INJECTION:
			// Tourne jusqu'a position injection
			moteur.commandeAngle(90.0);
			etatActuel = INJECTION;
			break;
		case INJECTION:
			// Effectuer depose insecticide
			valve.injecter();
			etatActuel = TOURNE_EVAC;
			break;
		case TOURNE_EVAC:
			// Tourne jusqu'a position sortie
			moteur.commandeAngle(90.0);
			etatActuel = FIN;
			break;
		case FIN:

			etatActuel = INITIAL;
			break;
		}
	}
}

void Manege::handleCapLumRE()
{
	reFlag_pinCapLum = true;
}

void Manege::handleTimeout(void) { timedOut = true; }