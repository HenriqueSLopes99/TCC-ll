#ifndef MOTOR__H
#define MOTOR__H
#include <Arduino.h>

// Defina os pinos para o motor de passo
#define motorPin1 0
#define motorPin2 2
#define motorPin3 4
#define motorPin4 15

// Defina o pino do buzzer
#define buzzerPin 26     // Substitua pelo número do pino GPIO ao qual o buzzer está conectado
#define buzzerLedPinAmarelo 13  // Substitua pelo pino do LED do buzzer
#define buzzerLedPinAzul 12
#define buzzerLedPinLaranja 33

#define resetPin 35   // volta para a posição inicial


// Define os pinos dos LEDs
#define CadastroLed 14     // LED que indica que está havendo cadastro
#define dispenseLedPin 32  // LED que indica que a função de dispensar está ligada
 


void moveMotorPosicaoInicial();
void moveMotorPosicao(int posicao);
void setup_motor();
#endif
