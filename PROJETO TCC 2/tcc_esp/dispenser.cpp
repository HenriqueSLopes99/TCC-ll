#include "dispenser.h"
#include <RTClib.h>
#include "motor.h"

String HORARIOS_MARCADOS[8][2];

RTC_DS3231 rtc;

bool isStarted = false;
bool despertando = false;
bool buzzerLedState = false;

enum topicos {
    DOMINGO = 0,
    SEGUNDA,
    TERCA,
    QUARTA,
    QUINTA,
    SEXTA,
    SABADO,
    DIA_VAZIO,
    EXCLUIR,
    EXCLUIR_TUDO,
    INICIAR
};

typedef enum topicos t_topicos;

t_topicos topicosMapper(String topic) {
    if (topic.equals("dia_vazio")) return DIA_VAZIO;
    if (topic.equals("domingo")) return DOMINGO;
    if (topic.equals("segunda")) return SEGUNDA;
    if (topic.equals("terca")) return TERCA;
    if (topic.equals("quarta")) return QUARTA;
    if (topic.equals("quinta")) return QUINTA;
    if (topic.equals("sexta")) return SEXTA;
    if (topic.equals("sabado")) return SABADO;
    if (topic.equals("excluir")) return EXCLUIR;
    if (topic.equals("excluir_tudo")) return EXCLUIR_TUDO;
    if (topic.equals("iniciar")) return INICIAR;
}

void salvaHorario(String topico, String msg) {
    // HORA MARCADA DA SEGUNDA
    HORARIOS_MARCADOS[topicosMapper(topico)][0] = msg.substring(0, 2);
    // MINUTO MARCADO DA SEGUNDA
    HORARIOS_MARCADOS[topicosMapper(topico)][1] = msg.substring(3, 5);
}

void setup_dispenser() {
    rtc.begin();  // Inicialize o RTC
//    if (!rtc.isrunning()) {
//        Serial.println("RTC não está funcionando! Inicializando...");
//        rtc.adjust(DateTime(F(DATE), F(TIME)));  // Ajuste a data e hora a partir da compilação
//    }
}

void excluiHorario(t_topicos dia) {
    // HORA MARCADA DA SEGUNDA
    HORARIOS_MARCADOS[dia][0] = "";
    // MINUTO MARCADO DA SEGUNDA
    HORARIOS_MARCADOS[dia][1] = "";
}

void processaMensagem(String topic, String msg) {
    switch (topicosMapper(topic)) {
        case DIA_VAZIO:
            // MOVER PARA A POSIÇÃO 0
            moveMotorPosicaoInicial();
            break;

        case SEGUNDA:
        case TERCA:
        case QUARTA:
        case QUINTA:
        case SEXTA:
        case SABADO:
        case DOMINGO:
            moveMotorPosicao(topicosMapper(topic));
            // função que verifica se a mensagem NÃO É VAZIA
            if (!msg.equals("vazio")) salvaHorario(topic, msg);
            break;

        case EXCLUIR:
            excluiHorario(topicosMapper(topic));
            moveMotorPosicao(topicosMapper(topic));
            break;

        case EXCLUIR_TUDO:
            for (int i = 0; i < 8; i++) {
                excluiHorario((t_topicos)i);
            }
            break;

        case INICIAR:
            startDispenser();
            break;

        default:
            break;
    }
}

void startDispenser() {
    isStarted = true;
}

void dispenseMedicine() {
    // Verifica a hora atual e gira o motor de passo quando um horário programado for alcançado
    DateTime now = rtc.now();
    int currentHour = now.hour();
    int currentMinute = now.minute();
    int weekDay = now.dayOfTheWeek();  // 0-domingo, 1-segunda, ..., 6-sábado

    if ((HORARIOS_MARCADOS[weekDay][0].toInt() == currentHour) &&( HORARIOS_MARCADOS[weekDay][1].toInt() == currentMinute)) {
        moveMotorPosicao(weekDay);
        ligarAlarme();
        // apagar horário
        excluiHorario((t_topicos)weekDay);
    }
}

void ligarAlarme() {
    despertando = true;
}

void desligarAlarme() {
    despertando = false;
}

void toggleBuzzerLed() {
    buzzerLedState = !buzzerLedState;
    digitalWrite(buzzerLedPinAmarelo, buzzerLedState);
    digitalWrite(buzzerLedPinAzul, buzzerLedState);
    digitalWrite(buzzerLedPinLaranja, buzzerLedState);
}

void tocarAlarme() {
    // Ativar o buzzer por um curto período de tempo
    digitalWrite(buzzerPin, HIGH);
    toggleBuzzerLed();             // Ativar o LED do buzzer
    delay(500);                    // Tempo de ativação (500 milissegundos)
    digitalWrite(buzzerPin, LOW);  // Desligar o buzzer
    toggleBuzzerLed();             // Desativar o LED do buzzer
    delay(500);
}
