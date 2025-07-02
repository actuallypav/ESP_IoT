#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#include "DHT.h"
#define TIME_ZONE 0
#define LED_PIN LED_BUILTIN


#define DHTPIN 4 //Digital pin
#define DHTTYPE DHT11 //DHT 11

DHT dht(DHTPIN, DHTTYPE);

float h ;
float t;
unsigned long lastMillis = 0;
const long interval = 5000;

#define AWS_IOT_PUBLISH_TOPIC "Temp" //fill this with the topic

WiFiClientSecure net;

BearSSL::X509List cert(root_ca);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

PubSubClient client(net);

time_t now;
time_t nowish = 1150416000;

void NTPConnect(void) {
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0, "time.google.com", "time.cloudflare.com");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void connectAWS() {
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  delay(100);

  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    delay(1000);
  }

  NTPConnect();

  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, 8883);

  Serial.println("Connecting to AWS IoT");

  while (!client.connect(THINGNAME))
  {
    //error to explain why im not connecting
    Serial.println("Connect failed, state: " + String(client.state()));
    delay(1000);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  Serial.println("AWS IoT Connected!");
}

void publishMessage() {
  StaticJsonDocument<200> doc;
  doc["thingname"] = THINGNAME;
  doc["time"] = now;
  doc["humidity"] = h;
  doc["temperature"] = t;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  Serial.print("Publishing to topic: ");
  Serial.println(AWS_IOT_PUBLISH_TOPIC);
  Serial.print("Message: ");
  Serial.println(jsonBuffer);

  if (!client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer)) {
    Serial.println("Publish failed!");
  }
}

void setup() {
  Serial.begin(115200);
  connectAWS();
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  digitalWrite(LED_PIN, LOW);
  delay(100);               
  digitalWrite(LED_PIN, HIGH);


  if (isnan(h) || isnan(t)) //Check if read failed and exit early TODO change to report in IoT and alarm?
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity "));
  Serial.print(h);
  Serial.print(F("% Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  delay(2000);

  now = time(nullptr);

  if (!client.connected())
  {
    connectAWS();
  }
  else
  {
    client.loop();
    if (millis() - lastMillis > 300000) // send a message every 5 mins
    {
      lastMillis = millis();
      publishMessage();
    }
  }
}
