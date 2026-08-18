#include <DHT.h>

#define SOIL_PIN 34

#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

const unsigned long SOIL_INTERVAL_MS = 500;
const unsigned long DHT_INTERVAL_MS  = 2000;

unsigned long lastSoilReadTime = 0;
unsigned long lastDHTReadTime = 0;

float lastTemperature = 25.0;

const int SOIL_DRY_RAW = 3000;
const int SOIL_WET_RAW = 1200;

float readSoilMoisturePercent()
{
  int raw = analogRead(SOIL_PIN);

  float percent =
    ((float)(SOIL_DRY_RAW - raw) /
     (SOIL_DRY_RAW - SOIL_WET_RAW)) * 100.0;

  percent = constrain(percent, 0, 100);

  return percent;
}

void setup()
{
  Serial.begin(115200);

  delay(500);

  analogReadResolution(12);

  dht.begin();
}

void loop()
{
  unsigned long now = millis();

  if (now - lastDHTReadTime >= DHT_INTERVAL_MS)
  {
    lastDHTReadTime = now;

    float temperature = dht.readTemperature();

    if (!isnan(temperature))
    {
      lastTemperature = temperature;
    }
  }

  if (now - lastSoilReadTime >= SOIL_INTERVAL_MS)
  {
    lastSoilReadTime = now;

    float soilPercent = readSoilMoisturePercent();

    Serial.print("Suhu: ");
    Serial.print(lastTemperature, 1);
    Serial.println(" C");

    Serial.print("Kelembapan: ");
    Serial.print(soilPercent, 1);
    Serial.println(" %");

    Serial.println("--------------------");
  }

  delay(10);
}