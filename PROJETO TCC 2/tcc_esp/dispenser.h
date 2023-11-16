#ifndef DISPENSER__H
#define DISPENSER__H
#include <Arduino.h>

extern bool isStarted;
extern bool despertando;
void processaMensagem(String topic, String msg);
void salvaHorario(String topic, String msg);
void iniciaContagem();
void startDispenser();
void dispenseMedicine();

void ligarAlarme();
void desligarAlarme();
void setup_dispenser();
void tocarAlarme();

#endif
