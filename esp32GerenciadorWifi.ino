#include "WiFi.h"
#include <PubSubClient.h>
#include <EEPROM.h>
#define EEPROM_SIZE 255   
const char* ssidAP = "name"; // Nome da rede wifi que o esp vai criar
const char* passwordAP = "pass"; // senha da rede wifi que o esp vai criar
const unsigned short int addressWifi = 0; // endereço da memoria
const unsigned short int addressWifiPassword = 30;// endereço da memoria
const unsigned short int addressMqttServe = 50;// endereço da memoria
const unsigned short int addressMqttPort = 100;// endereço da memoria
const unsigned short int addressMqttUser = 110;// endereço da memoria
const unsigned short int addressMqttPassword = 150;// endereço da memoria
unsigned short int msgAlert;
WiFiServer server(80);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Funções de inciacialização
void initEspWebServer() {
  //inicia o esp no modo AP
  Serial.print("Criando o Access Piont (AP): ");
  Serial.print(ssidAP); // nome da rede
  Serial.print("Senha para conexão: ");
  Serial.print(passwordAP); //Senha da rede
  wifiScan(1);
  WiFi.softAP(ssidAP, passwordAP);
  Serial.println("");
  Serial.println("AP do WiFi OK.");
  Serial.println("No IP: ");
  Serial.println(WiFi.softAPIP());
}
void initWifiClient() {
  int countWifi = wifiScan(0);
  if (EEPROM.read(addressWifi) > 0) {
    String ssid = searchEeprom(addressWifi);
    String password = searchEeprom(addressWifiPassword);
    Serial.print("Credenciais wifi SSID:  ");
    Serial.print(ssid);
    Serial.print("   Pass:  ");
    Serial.println(password);
    WiFi.begin(ssid, password);
    WiFi.disconnect();
  }
}
void connectMqtt() {
  if (EEPROM.read(addressMqttServe) > 0 && EEPROM.read(addressMqttPort) > 0) {
    String id = "PLANTAE_1";
    String user = searchEeprom(addressMqttUser);
    String pass = searchEeprom(addressMqttPassword);
    String mqttServe = searchEeprom(addressMqttServe);

    int port = searchEeprom(addressMqttPort).toInt();
    Serial.print("Credenciais mqtt ID:  ");
    Serial.print(id);
    Serial.print("   User:  ");
    Serial.print(user);
    Serial.print("   Pass:  ");
    Serial.print(pass);
    Serial.print("   MqttServe:  ");
    Serial.print(mqttServe);
    Serial.print("   Port:  ");
    Serial.println(port);

    mqttClient.setServer(mqttServe.c_str(), port);
    mqttClient.setCallback(callback);
    mqttClient.connect(id.c_str(), user.c_str(), pass.c_str());
  }
}
void callback(char* topic, byte* payload, unsigned int length) {
  //recebe os topicos
  Serial.print("Mensagem Recebida [");
  Serial.print(topic);
  Serial.print("] ");
}
void subscribeTopicsMqtt() {
   //mqttClient.subscribe("topic"); // inscrição nos topicos mqtt
  
}
// Fim funções de inciacialização

void setup() {
  Serial.begin(115200);
  pinMode(23, INPUT);
  WiFi.mode(WIFI_AP_STA);  //define esp32 AP and STA como ponto de acesso e cliente
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("failed to initialise EEPROM");
    ESP.restart();
  }
  initEspWebServer();
  initWifiClient();
}

void loop() {
  verifyRequestClientWebServe();
  verifyConnection();
  if (digitalRead(23) == 1 && msgAlert == 0) {
    publish("botaoAvo", "1");
    msgAlert = 1;
  }
}

void publish(char* topic, char* payload) {
  //publicar no topico a informação
  char msg[50];
  snprintf(msg, 50, payload);
  mqttClient.publish(topic, payload);
  Serial.print("Topico publicado com sucesso:");
  Serial.print(topic);
  Serial.print("     MSG: ");
  Serial.println(msg);
}
void verifyConnection() {
  static uint64_t timeNewTry = 0;
  if (esp_timer_get_time() > timeNewTry ) {
    if (WiFi.status() != WL_CONNECTED) {  // verifica conexão com wifi
      WiFi.reconnect();
      Serial.println("Falha ao conectar com wifi, reconectando");
    } else {
      Serial.println("Wifi conectado");
      if (!mqttClient.connected()) {  // verifica conexão com o broker mqtt
        connectMqtt();
        Serial.print("Falha ao conectar com broker MQTT, reconectando ");
        Serial.println(mqttClient.state());
      } else {
        Serial.println("MQTT conectado");
      }
    }
    timeNewTry = esp_timer_get_time() + 5000000;
    msgAlert = 0;
  }
}
void verifyConnectionBroker() {
}
void verifyRequestClientWebServe() {
  static bool statusWebServe;
  if (WiFi.softAPgetStationNum() > 0) {
    if (!statusWebServe) {
      server.begin();
    }
    String webRequest;
    WiFiClient webClient = server.available();  // Listen for incoming clients
    bool postRequest;
    if (webClient) {                            // If a new webClient connects,
      char finishLine = NULL;                   // make a String to hold incoming data from the webClient
      while (webClient.connected()) {           // loop while the webClient's connected
        if (webClient.available()) {            // if there's bytes to read from the webClient,
          char charRequest = webClient.read();  // read a byte, then                           // print it out the serial monitor
          //Serial.write(charRequest);
          webRequest += charRequest;
          if (charRequest == '\n') {
            if (finishLine == NULL) {  // se a solicitação envoiou \r\n significa que o cliente termininou de enviar os dados
              webClient.println("HTTP/1.1 200 OK");
              webClient.println("Content-type:text/html");
              webClient.println("Connection: close");
              webClient.println();
              if (webRequest.indexOf("GET /saveInfo?") != -1) {
                sendWebPage(webClient, "save");
                //pego as variveis dentro da request
                saveResponseFormConfig(webRequest);
                //tento iniciar o wifi com as novas configurações
                initWifiClient();
              } else if (webRequest.indexOf("GET /config") != -1) {
                sendWebPage(webClient, "config");  //envia a webpage de configuração
              } else {
                sendWebPage(webClient, "home");  //envia a webpage inicial para o usuario
              }
              break;
            } else {
              finishLine = NULL;
            }
          } else if (charRequest != '\r') {
            finishLine = charRequest;
          }
        }
      }
      webClient.stop();
      webRequest = "";
    }
  } else {
    if (statusWebServe) {
      server.stop();
    }
  }
}
void saveResponseFormConfig(String webRequest) {
  String wifi = callBackRequest("wifi=", webRequest);
  String wifiPassword = callBackRequest("wifiPassword=", webRequest);
  String mqttServe = callBackRequest("mqttServe=", webRequest);
  String mqttPort = callBackRequest("mqttPort=", webRequest);
  String mqttUser = callBackRequest("mqttUser=", webRequest);
  String mqttPassword = callBackRequest("mqttPassword=", webRequest);

  saveStringEeprom(addressMqttServe, mqttServe);
  saveStringEeprom(addressMqttPort, mqttPort);
  saveStringEeprom(addressMqttUser, mqttUser);
  saveStringEeprom(addressMqttPassword, mqttPassword);
  saveStringEeprom(addressWifi, wifi);
  saveStringEeprom(addressWifiPassword, wifiPassword);
}
String searchEeprom(unsigned short int address) {
  char value;
  String msg;
  unsigned short int size = EEPROM.read(address);
  if (size > 0) {
    address++;
    for (unsigned short int i = address; i < address + size; i++) {
      value = EEPROM.read(i);
      msg += value;
    }
  }
  return msg;
}
void saveStringEeprom(unsigned short int address, String value) {
  unsigned short int length = value.length();
  EEPROM.write(address, length);
  address += 1;
  for (short int i = 0; i < length; i++) {
    EEPROM.write(address, value.charAt(i));
    address++;
  }
  EEPROM.commit();
}
String callBackRequest(String name, String webRequest) {
  //recebe as variaveis do formulario preenchido
  String value;
  short int beginPosition = webRequest.indexOf(name);
  if (beginPosition != -1) {
    short int endPosition = webRequest.indexOf("&", beginPosition);
    for (unsigned short int x = beginPosition + name.length(); x < endPosition; x++) {
      value += webRequest[x];
    }
  }

  return value;
}
void sendWebPage(WiFiClient webClient, String page) {
  if (page.equals("home")) {
    String statusWifi = "Conectado";
    String statusMqtt = "Conectado";
    if (WiFi.softAPgetStationNum() == 0) {
      statusWifi = "Desconectado";
      statusMqtt = "Desconectado";
    } else if (!mqttClient.connected()) {
      statusWifi = "Conectado";
      statusMqtt = "Desconectado";
    }
    webClient.println(webPageHead());
    webClient.println(webPageBody());
    webClient.println(webPageStatus(statusWifi, statusMqtt));
    webClient.println(webPageFooter());
    webClient.println();

  } else if (page.equals("config")) {
    String ssid = searchEeprom(addressWifi);
    String mqttServe = searchEeprom(addressMqttServe);
    String port = searchEeprom(addressMqttPort);
    String user = searchEeprom(addressMqttUser);
    webClient.println(webPageHead());
    webClient.println(webPageBody());
    webClient.println(wifiList());
    webClient.println(webPageFormConfig(ssid, mqttServe, port, user));
    webClient.println(webPageFooter());
    webClient.println();
  } else if (page.equals("save")) {
    webClient.println(webPageSave());
  }
}
String wifiList() {
  int countWifi = wifiScan(0);  //Retorna a quantidade de redes wifi disponíveis
  String msg = "<table class=''><tbody class=''> <tr><th style = 'text-align:left;' > Nome da rede</th><th style = 'text-align:left;'> Sinal</th></tr>";
  //Exibe as redes wifi encontradas
  if (countWifi > 0) {
    for (int i = 0; i < countWifi; ++i) {
      msg += "<tr><th style = 'text-align:left;' onclick=wifi.value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</th><th> " + WiFi.RSSI(i) + "</th></tr>";
    }
    msg += "</tbody></table>";
  } else {
    msg += "<br><h3>Nenhuma rede encontrada.</h3>";
  }
  return msg;
}
unsigned short int wifiScan(short int acao) {
  static unsigned short int amountWifi;
  if (acao == 1 || amountWifi == 0) {  // realiza uma nov a leitura
    amountWifi = WiFi.scanNetworks();
  }

  return amountWifi;
}
