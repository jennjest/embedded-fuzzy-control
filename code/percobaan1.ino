#include <DHT.h>

#define SOIL_PIN 34

#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

const int SOIL_DRY_RAW = 3000;
const int SOIL_WET_RAW = 1200;

void setup()
{
  Serial.begin(115200);
  dht.begin();
}

void loop()
{
  float temperature = dht.readTemperature();

  int raw = analogRead(SOIL_PIN);

  float soilPercent =
    ((float)(SOIL_DRY_RAW - raw) /
     (SOIL_DRY_RAW - SOIL_WET_RAW)) * 100.0;

  soilPercent = constrain(soilPercent, 0, 100);

  Serial.print("Suhu: ");
  Serial.print(temperature, 1);
  Serial.println(" C");

  Serial.print("Kelembapan Tanah: ");
  Serial.print(soilPercent, 1);
  Serial.println(" %");

  Serial.println("--------------------");

  delay(2000);
}