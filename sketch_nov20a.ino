#define BLYNK_TEMPLATE_ID "TMPL3m76ETFrJ"
#define BLYNK_TEMPLATE_NAME "Smart Energy Monitor"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(&Serial);

char auth[] = "h_QKs9dL_1Kn448odfaqh4hIy5fvknPd";

// WiFi credentials
char ssid[] = "Smart Energy Monitor";
char pass[] = "12345678";

unsigned long lastMillis = 0;
void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconnecting to WiFi...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
  }
}

void checkBlynk() {
  if (!Blynk.connected()) {
    Serial.println("Reconnecting to Blynk...");
    Blynk.connect();
    if (Blynk.connected()) {
      Serial.println("Connected to Blynk");
    }
  }
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Blynk.config(auth, "blynk.cloud", 80);
  checkBlynk(); 
}

void loop() {
  checkWiFi();
  checkBlynk();

  if (Blynk.connected()) {
    Blynk.run();
  }

  float voltage = pzem.voltage();
  if (!isnan(voltage)) {
    Serial.print("Voltage: "); Serial.print(voltage, 1); Serial.println(" V");
  } else {
    Serial.println("Error reading voltage");
  }

  float current = pzem.current();
  if (!isnan(current)) {
    Serial.print("Current: "); Serial.print(current, 3); Serial.println(" A");
  } else {
    Serial.println("Error reading current");
  }

  float power = pzem.power();
  if (!isnan(power)) {
    Serial.print("Power: "); Serial.print(voltage*Current, 3); Serial.println(" W");
  } else {
    Serial.println("Error reading power");
  }

  float energy = pzem.energy();
  if (!isnan(energy)) {
    Serial.print("Energy: "); Serial.print((voltage * Current) / 1000, 3); Serial.println(" kWh");
  } else {
    Serial.println("Error reading energy");
  }

  Serial.println();
  delay(2000);
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    Blynk.virtualWrite(V0, voltage);
    Blynk.virtualWrite(V1, Current);
    Blynk.virtualWrite(V2, voltage * Current);
    Blynk.virtualWrite(V3, (voltage * Current) / 1000);
    Serial.println("Data sent to Blynk");
  }
}