#include <WiFi.h>
#include <PubSubClient.h>

/* CONFIGURAÇÕES DO WIFI */
const char* ssid = "Nome_da_Rede";       // Nome da rede WiFi
const char* password = "Senha_da_Rede";  // Senha da rede WiFi

/* CONFIGURAÇÕES DO MQTT */
const char* mqttServer = "broker.hivemq.com";  // Endereço do Broker MQTT
const int mqttPort = 1883;                     // Porta TCP do Broker MQTT
const char* mqttUser = "Melin";                     // Usuário MQTT
const char* mqttPassword = "1234";                 // Senha MQTT

const int relePin = 32;  // Pino do ESP32 conectado ao relé

WiFiClient espClient;                // Cliente de Rede WiFi
PubSubClient clientMqtt(espClient);  // Cria uma instancia de um cliente MQTT

void setup() {
  pinMode(relePin, OUTPUT);     // Configura o pino do relé como saída
  digitalWrite(relePin, HIGH);  // Desliga o relé inicialmente

  Serial.begin(9600);          // Configura a porta serial
  WiFi.begin(ssid, password);  // Configura o WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Tentando se conectar na rede: ");
    Serial.println(ssid);
  }

  Serial.println("Conectado na Rede WiFi.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  clientMqtt.setServer(mqttServer, mqttPort);
  clientMqtt.setCallback(callback);

  /* Verifica se o cliente está conectado ao Broker */
  while (!clientMqtt.connected()) {

    Serial.println("Conectando ao Broker MQTT...");

    String clientId = "ESP32Client_" + String(random(0xffff), HEX);
    Serial.println("clientId = " + clientId);

    /* Conecta o cliente ao Broker MQTT.
       Configurações de credenciais e Last Will podem ser configuradas aqui*/
    if (clientMqtt.connect(clientId.c_str(), mqttUser, mqttPassword)) {

      Serial.println("O cliente " + clientId + " foi conectado com sucesso");
      clientMqtt.subscribe("unifor/cps/abrir-trava");
    } else {

      // Estado do cliente MQTT. Quando a conexão falhar pode ser usado para obter informações sobre a falha
      int clientState = clientMqtt.state();

      Serial.print("Falha ao se conectar. ");
      Serial.println("Estado do cliente: " + (String)clientState);

      delay(2000);
    }
  }
}

/* A função de callback é chamada quando uma mensagem chega, devido a assinatura de um tópico.
Ou seja, toda vez que chegar uma mensagem em um tópico inscrito por este cliente, a função
callback será chamada. */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Uma mensagem chegou no tópico: ");
  Serial.println(topic);

  Serial.print("Payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  if (strcmp(topic, "unifor/cps/abrir-trava") == 0) {
    if (payload[0] == '0') {
         Serial.println("\nTrava");
         Serial.println("\nTrava acionada");
      digitalWrite(relePin, LOW); // Liga o relé, acionando a trava
    } 
    if (payload[0] == '1') {
      Serial.println("\nTrava desligada");
       digitalWrite(relePin, HIGH); // Liga o relé, acionando a trava
      // Neste caso, não é necessário fazer nada, já que a trava será desligada pelo próprio relé
    }
  }

  Serial.println();
  Serial.println("-----------------------");
}

void loop() {
  clientMqtt.loop();
}
