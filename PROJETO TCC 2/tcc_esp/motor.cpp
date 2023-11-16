#include <Stepper.h>
#include <Arduino.h>

// Defina os pinos do motor de passo
const int motorPino1 = 0;
const int motorPino2 = 2;
const int motorPino3 = 4;
const int motorPino4 = 15;

// Defina o número de passos por revolução
const int passosPorRevolucao = 64;
const int passosPorDia = passosPorRevolucao / 8;
const int posicaoInicial = 7;

// 0-domingo, 1-segunda, ..., 6-sábado, 7-dia_vazio
const int posicoes[8] = {0, 8, 16, 24, 32, 40, 48, 56};

// Crie um objeto Stepper
Stepper motor(passosPorRevolucao, motorPino1, motorPino2, motorPino3, motorPino4);

int passoAtual = 0;

void setup_motor() {
    // Inicialize o motor
    motor.setSpeed(300);  // Velocidade do motor (ajuste conforme necessário)
    pinMode(21,INPUT);
    pinMode(22,INPUT);

}

void moveMotorPosicaoInicial() {
    motor.step((posicaoInicial - passoAtual) * passosPorDia);
}

// 0-domingo, 1-segunda, ..., 6-sábado, 7-dia_vazio
void moveMotorPosicao(int posicao) {
    motor.step(-(posicaoInicial - posicao) * passosPorDia);
    passoAtual = posicao;
}
