// secrets.h file
#define SECRET_SSID "Himanshu"
#define SECRET_PASS "12345678"
#define SECRET_CH_ID 2635740
#define SECRET_WRITE_APIKEY "91CO0S5OBMY4YRAS"
#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <DHT.h>

// DHT sensor setup
#define DHTPIN 2      // Pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11 (change to DHT22 if using DHT22)
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(115200);
  dht.begin(); // Initialize DHT sensor
  ThingSpeak.begin(client);
  connectToWiFi();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    connectToWiFi();
  }

  // Read data from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Update ThingSpeak fields
  ThingSpeak.setField(1, temperature); // Field 1: Temperature
  ThingSpeak.setField(2, humidity);    // Field 2: Humidity (optional)

  // Write data to ThingSpeak
  int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(responseCode == 200){
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(responseCode));
  }

  delay(60000); // Wait 60 seconds before the next update
}

void connectToWiFi() {
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    delay(5000); // Wait for 5 seconds before retrying
  }
  Serial.println("Connected.");
}