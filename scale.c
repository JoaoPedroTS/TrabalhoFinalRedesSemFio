#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "lab120";
const char* password = "labredes120";
const char* serverURL = "http://150.162.235.112:5000/data";

WiFiClient client;

float previousWeight = 0.0;
float currentWeight = 0.0;

// Itens do estoque
String items[] = {"chocolate", "docinho", "brownie", "biscoito"};
int numItems = sizeof(items) / sizeof(items[0]);

void setup() {
  Serial.begin(115200);

  // Conexão com a rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Geração de um peso aleatório na faixa de 60 a 160
  float weight = random(60, 161);

  if (weight != currentWeight) {
    previousWeight = currentWeight;
    currentWeight = weight;

    Serial.print("Previous Weight: ");
    Serial.println(previousWeight);
    Serial.print("Current Weight: ");
    Serial.println(currentWeight);

    // Identificação do item no estoque com base no peso
    String item = getItemFromWeight(currentWeight);

    if (item != "") {
      Serial.print("Item in stock: ");
      Serial.println(item);

      // Criação do objeto JSON
      StaticJsonDocument<200> json;
      json["item"] = item;
      json["change"] = currentWeight - previousWeight;

      // Conversão do objeto JSON em uma string
      String jsonStr;
      serializeJson(json, jsonStr);

      // Envio dos dados para o servidor Flask
      sendRequest(jsonStr);
    }
  }

  delay(5000);
}

// Função para identificar o item no estoque com base no peso
String getItemFromWeight(float weight) {
  if (weight >= 100 && weight < 200) {
    return items[0];  // chocolate
  } else if (weight >= 200 && weight < 300) {
    return items[1];  // docinho
  } else if (weight >= 300 && weight < 400) {
    return items[2];  // brownie
  } else if (weight >= 400 && weight < 500) {
    return items[3];  // biscoito
  }

  return "";  // Nenhum item encontrado
}

// Função para enviar uma requisição HTTP POST com dados JSON
void sendRequest(String data) {
  HTTPClient http;
  http.begin(client, serverURL);

  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(data);
  
  if (httpResponseCode == 200) {
    Serial.println("Data sent to server successfully");
  } else {
    Serial.print("Error sending data to server. HTTP response code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}