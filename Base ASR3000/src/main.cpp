#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

// Definicacao de constantes
const int csPin = 5;      // Chip Select ( Slave Select do protocolo SPI ) do modulo Lora
const int resetPin = 2;    // Reset do modulo LoRa
const int irqPin = 4;      // Pino DI0

byte localAddress = 0xBB; // Endereco deste dispositivo LoRa
byte msgCount = 0;         // Contador de mensagens enviadas
byte destination = 0xFF;  // Endereco do dispositivo para enviar a mensagem (0xFF envia para todos devices )

StaticJsonDocument<512> deserializedDoc;
// Setup do Microcontrolador
void setup()
{
  // inicializacao da serial
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(" Comunicacao LoRa Duplex - Ping & Pong ");

  LoRa.setPins(csPin, resetPin, irqPin); // set CS , reset , IRQ pin
  // Inicializa o radio LoRa em 915 MHz e checa se esta ok!
  if (!LoRa.begin(915E6))
  {
    Serial.println(" Erro ao iniciar modulo LoRa . Verifique a coenxao dos seus pinos !! ");
    while (true);
  }
  Serial.println(" Modulo LoRa iniciado com sucesso !!! :) ");

}

// Funcao para receber mensagem
void onReceive(int packetSize)
{
  if (packetSize == 0)
    return; // Se nenhuma mesnagem foi recebida , retorna nada

  // int recipient = LoRa.read();       // Endereco de quem ta recebendo
  // byte sender = LoRa.read();         // Endereco do remetente
  // byte incomingMsgId = LoRa.read();  // Mensagem
  

  DeserializationError err = deserializeJson(deserializedDoc, LoRa);
  deserializedDoc["Redundancia Paraquedas Estabilizador"] = deserializedDoc["PPE"];
  deserializedDoc["Comercial Paraquedas Estabilizador"] = deserializedDoc["PPE"];
  deserializedDoc["Comercial Paraquedas Principal"] =deserializedDoc["PPP"];
  deserializedDoc["RSSI"] = LoRa.packetRssi();
  serializeJson(deserializedDoc, Serial);
  Serial.println();
}

// Loop do microcontrolador - Operacoes de comunicacao LoRa
void loop()
{
  
  // parse for a packet , and call onReceive with the result :
  onReceive(LoRa.parsePacket());
}