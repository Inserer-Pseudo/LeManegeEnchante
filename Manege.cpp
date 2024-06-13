#include "Manege.hpp"

Manege::Manege(MotPaP &moteur, Injection &valve, DigitalOut &pinPhotoIn,
               DigitalOut &pinPhotoOut, DigitalOut &pinLed,
               InterruptIn &pinCapLum, InterruptIn &bpPurge)
    : etatActuel(INITIAL), moteur(moteur), valve(valve), pinPhotoIn(pinPhotoIn),
      pinPhotoOut(pinPhotoOut), pinLed(pinLed), pinCapLum(pinCapLum), bpPurge(bpPurge) {
		  bpPurge.rise(callback(this, &Manege::handlePurgeRE));
		  bpPurge.fall(callback(this, &Manege::handlePurgeFE));
	  }

int Manege::init(void) {
  printf("start init\n");
  bool initDone = false;
  timedOut = false;

  pinLed.write(1);

  pinCapLum.rise(callback(this, &Manege::handleCapLumRE));
  pinCapLum.fall(callback(this, &Manege::handleCapLumFE));
  //pinCapLum.rise(callback([this](void){ this->handleCapLumChange(true); }));

  // timeout.attach(callback(this, &Manege::handleTimeout), 60s);
  t.start();

  moteur.prepCommandePas(51220);

  reFlag_pinCapLum = false;

  bool premierFront = false;
  bool secondFront = false;
  int posPremierFront = 0;
  int posSecondFront = 0;
  while (initDone == 0 && timedOut == 0) {
    moteur.execCommandePas();

    if (reFlag_pinCapLum == true) {
		//posPremierFront = moteur.getPasActuel();
		//("premier front %d\n",posPremierFront);
		//premierFront  = true;
		moteur.commandePas(200);
		moteur.resetCodeurAbsolu();
		initDone = true;
      	reFlag_pinCapLum = false;
    }
	// if (feFlag_pinCapLum == true) {
	// 	posSecondFront = moteur.getPasActuel();
	// 	printf("second front %d\n",posSecondFront);
	// 	secondFront = true;
    //   feFlag_pinCapLum = false;
    // }
	// if (premierFront && secondFront){
	// 	printf("alignement... %d\n", -(posSecondFront - posPremierFront));
	// 	moteur.commandePas(-(posSecondFront - posPremierFront)); // devrait etre / 2 mais backlash
	// 	initDone = true;
	// }

    // if (t.elapsed_time().count() >= 20000000) {
    //   printf("init failed\n");
    //   printf("The time taken was %llu milliseconds\n",
    //   std::chrono::duration_cast<std::chrono::milliseconds>(t.elapsed_time()).count());
    //   t.stop();
    //   return -1;
    // }
  }

  // timeout.detach();
  printf("init done\n");
  etatActuel = INITIAL;
  return 0;
}

void Manege::marche() {
  running = true;
  eventQueue.call(callback(this, &Manege::cycle));
  thread.start(callback(&eventQueue, &EventQueue::dispatch_forever));
}

void Manege::stop() {
  running = false;
  eventQueue.break_dispatch();
}

int Manege::getEtatActuel() { return etatActuel; }

void Manege::cycle() {
  while (running) {
    switch (etatActuel) {
    case INITIAL:
		printf("INITIAL\n");
      // Tourne jusqu'a position entree si pos !=
        //moteur.commandePas(12800);
      // Attente de declenchement
      if (feFlag_pinCapLum)
        feFlag_pinCapLum = false;
      etatActuel = TOURNE_PHOTO;
      break;
    case TOURNE_PHOTO:
		printf("TOURNE_PHOTO\n");
      // Tourne jusqu'a position photo
      moteur.commandePas(12800);
      etatActuel = PHOTO;
      break;
    case PHOTO:
		printf("PHOTO\n");
      // Declenchement photo
      pinPhotoOut.write(1);
      ThisThread::sleep_for(2s);
      pinPhotoOut.write(0);
      etatActuel = ATTENTE_PHOTO;
      break;
    case ATTENTE_PHOTO:
      // Attente tempo photo
	  printf("ATTENTE_PHOTO\n");
      ThisThread::sleep_for(5s);
      etatActuel = TOURNE_INJECTION;
      break;
    case TOURNE_INJECTION:
      // Tourne jusqu'a position injection
	  printf("TOURNE_INJECTION\n");
      moteur.commandePas(12800);
      etatActuel = INJECTION;
      break;
    case INJECTION:
      // Effectuer depose insecticide
	  printf("INJECTION\n");
      valve.injecter();
	  ThisThread::sleep_for(2s);
      etatActuel = TOURNE_EVAC;
      break;
    case TOURNE_EVAC:
      // Tourne jusqu'a position sortie
	  printf("TOURNE_EVAC\n");
      moteur.commandePas(12800);
      etatActuel = FIN;
      break;
    case FIN:
		printf("FIN\n");
		moteur.commandePas(12800);
      etatActuel = INITIAL;
      break;
    }
  }
}

void Manege::handleCapLumRE() { reFlag_pinCapLum = true; }
void Manege::handleCapLumFE() { feFlag_pinCapLum = true; }
void Manege::handlePurgeRE() { 
	//valve.openValve();
}
void Manege::handlePurgeFE() {
	//valve.closeValve(); 
}

void Manege::handleTimeout(void) { timedOut = true; }