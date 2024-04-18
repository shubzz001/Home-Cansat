#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define USERNAME "*******"
#define DEVICE_ID "****"
#define DEVICE_CREDENTIAL "**********"

#define SSID "Spider"
#define SSID_PASSWORD "*******"

#define DHTPIN D7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP280 bmp;
float bmptemp = 0.0;
float pressure = 0.0;

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Wire.begin(D2, D3); // Initialize I2C pins for BMP280
  bmp.begin(0x76); // I2C address 0x76 for BMP280
  thing.add_wifi(SSID, SSID_PASSWORD);
}

void loop() {
  thing.handle();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float pressure = bmp.readPressure() / 100.0;

  thing["DHT11"] >> [temperature, humidity](pson& out) {
    out["temperature"] = temperature;
    out["humidity"] = humidity;
  };

  thing["BMP280"] >> [bmptemp, pressure](pson& out) {
    out["bmp_280_Temperature"] = bmp.readTemperature();
    out["bmp_280_Pressure"] = bmp.readPressure();
    out["bmp_280_altitude"] = bmp.readAltitude();
  };
}


#define DHT_PIN D7
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;

float temperatureDHT = 0.0;
float humidityDHT = 0.0;
float temperatureBMP = 0.0;
float pressureBMP = 0.0;
float altitudeBMP = 0.0;

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(115200);
  dht.begin();
  bmp.begin(0x76);
  thing.add_wifi(SSID, SSID_PASSWORD);

  thing["DHT11"] >> [](pson& out) {
    out["temperature"] = temperatureDHT;
    out["humidity"] = humidityDHT;
  };

  thing["BMP280"] >> [](pson& out) {
    out["temperature"] = temperatureBMP;
    out["pressure"] = pressureBMP;
    out["altitude"] = altitudeBMP;
  };
}

void loop() {
  thing.handle();

  // Read DHT11 sensor values
  temperatureDHT = dht.readTemperature();
  humidityDHT = dht.readHumidity();

  // Read BMP280 sensor values
  temperatureBMP = bmp.readTemperature();
  pressureBMP = bmp.readPressure() / 100.0;
  altitudeBMP = bmp.readAltitude();

  delay(2000); // Adjust the delay as needed
}
