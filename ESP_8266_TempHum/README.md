# ESP8266 NodeMCU Temperature/Humidity Sensor to AWS IoT

This repository contains a simple script for the **ESP-8266 NodeMCU** that reads temperature and humidity data from a **DHT11 sensor** and publishes the data to **AWS IoT** using MQTT.

## Contents

- **`main.ino`**: The main script for setting up the WiFi connection, MQTT communication with AWS IoT, reading sensor data, and publishing it.
- **`secrets.h`**: A configuration file for storing sensitive information like WiFi credentials, MQTT settings, and certificates.

## Requirements

- **Hardware**: 
  - ESP-8266 NodeMCU
  - DHT11 Temperature and Humidity Sensor

- **Software Libraries**: 
  - `ESP8266WiFi.h`
  - `WiFiClientSecure.h`
  - `PubSubClient.h`
  - `ArduinoJson.h`
  - `DHT.h`

- **Certificates**:
  - `Certificate.pem`
  - `root_ca.pem`
  - `private_key.pem`
  - All of these are available for creation/automatic provisioning of certificates with this [repository](https://github.com/actuallypav/EST_Service).

## Setup Instructions

1. **Install the necessary libraries**:
   - In the Arduino IDE, go to **Sketch > Include Library > Manage Libraries**.
   - Install `ESP8266`, `PubSubClient`, `ArduinoJson`, and `DHT` libraries.

2. **Configure WiFi and MQTT credentials**:
   - Open the `secrets.h` file.
   - Fill in your WiFi credentials (SSID and password).
   - Enter your AWS IoT thing name, MQTT host URL, and add the appropriate certificates (root CA, client cert, and private key).

3. **Upload the code**:
   - Open the `main.ino` file in the Arduino IDE.
   - Select the appropriate board and port for your ESP-8266.
   - Upload the sketch to your ESP-8266.

4. **Monitor the Serial Output**:
   - Open the Serial Monitor to check the status and verify the connection to AWS IoT and the sensor readings (it will fail if it tries to reconnect after the first few messages "appear" to send).

## How It Works

1. The script connects to your WiFi network using the credentials provided in `secrets.h`.
2. The time is synchronized with an NTP server to ensure accurate timestamps.
3. The script then reads temperature and humidity data from the DHT11 sensor.
4. The data (along with a timestamp) is formatted as a JSON message and published to the specified MQTT topic on AWS IoT.
5. If the connection to AWS IoT is lost, the script will attempt to reconnect.