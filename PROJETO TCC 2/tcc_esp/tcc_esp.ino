#include <Arduino.h>
#include <PubSubClient.h>
#include <RTClib.h>
#include <WiFi.h>

#include "dispenser.h"
#include "motor.h"

const char* ssid = "Henrique Lopes";
const char* password = "Henrique";
WiFiClient wifiClient;
char* topics[] = {"segunda", "terca", "quarta", "quinta", "sexta", "sabado", "domingo", "excluir", "dia_vazio", "excluir_tudo", "inciar"};

const char* mqttServer = "192.168.137.1";
const int mqttPort = 1883;
const char* clientId = "18741155-810b-4ab7-9c81-93a63352a132";

PubSubClient MQTT(wifiClient);
void keepConnections();
void conectWiFi();
void conectMQTT();
void receivePackage(char* topic, byte* payload, unsigned int length);

void setup(void) {
    Serial.begin(115200);
    MQTT.setServer(mqttServer, mqttPort);
    MQTT.setKeepAlive(3);
    MQTT.setCallback(receivePackage);
    setup_dispenser();
    setup_motor();
}

void loop(void) {
    keepConnections();

    // Verifica se o botão de liga/desliga foi pressionado
if (isStarted) {
        dispenseMedicine();
        // acender o led verde
        digitalWrite(dispenseLedPin, HIGH);
        digitalWrite(CadastroLed, LOW);
    } else {
        digitalWrite(CadastroLed, HIGH);
        // apagar o led verde
        digitalWrite(dispenseLedPin, LOW);
    }

    if (digitalRead(resetPin) == HIGH) {
        desligarAlarme();
        moveMotorPosicaoInicial();
    }

    if (despertando) {
        tocarAlarme();
    }

    if (isStarted) {
        dispenseMedicine();
    }
}

void keepConnections() {
    conectWiFi();
    conectMQTT();
    MQTT.loop();
}

void conectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(mqttServer);

        if (MQTT.connect(clientId)) {
            Serial.println("Conectado ao Broker com sucesso!");
            subscribleTopics();
        } else {
            Serial.println("A conexão ao broker não pôde ser estabelecida! Faremos uma nova tentativa em 5 segundos.");
            delay(5000);
        }
    }
}

void subscribleTopics() {
    for (char* topic : topics) {
        MQTT.subscribe(topic);
    }
}

void conectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;

    Serial.print("Conectando-se na rede...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
        delay(10);

    Serial.println();
    Serial.print("Conectado com sucesso, na rede: ");
    Serial.println(ssid);
}

void receivePackage(char* topic, byte* payload, unsigned int length) {
    String msg;

    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }

    String topicString = (String)topic;

    Serial.print(topicString + ": ");
    Serial.println(msg);

    processaMensagem(topicString, msg);
}
