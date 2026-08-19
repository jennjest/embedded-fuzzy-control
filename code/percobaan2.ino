#include <DHT.h>

#define SOIL_PIN 34

#define DHT_PIN 4
#define DHT_TYPE DHT22

#define LED_MERAH_PIN 25
#define LED_KUNING_PIN 26
#define LED_HIJAU_PIN 27

DHT dht(DHT_PIN, DHT_TYPE);

const int SOIL_DRY_RAW = 3000;
const int SOIL_WET_RAW = 1200;


float suhuDingin(float x)
{
  if (x <= 24) return 1.0;
  if (x >= 28) return 0.0;

  return (28.0 - x) / (28.0 - 24.0);
}

float suhuNormal(float x)
{
  if (x <= 25 || x >= 45) return 0.0;
  if (x == 30) return 1.0;

  if (x < 30)
    return (x - 25.0) / (30.0 - 25.0);

  return (45.0 - x) / (45.0 - 30.0);
}

float suhuPanas(float x)
{
  if (x <= 40) return 0.0;
  if (x >= 60) return 1.0;

  return (x - 40.0) / (60.0 - 40.0);
}


float tanahKering(float x)
{
  if (x <= 20) return 1.0;
  if (x >= 35) return 0.0;

  return (35.0 - x) / (35.0 - 20.0);
}

float tanahLembap(float x)
{
  if (x <= 25 || x >= 50) return 0.0;
  if (x == 35) return 1.0;

  if (x < 35)
    return (x - 25.0) / (35.0 - 25.0);

  return (50.0 - x) / (50.0 - 35.0);
}

float tanahBasah(float x)
{
  if (x <= 45) return 0.0;
  if (x >= 70) return 1.0;
  return (x - 45.0) / (70.0 - 45.0);
}


void setLED(int ledPin)
{
  digitalWrite(LED_MERAH_PIN, LOW);
  digitalWrite(LED_KUNING_PIN, LOW);
  digitalWrite(LED_HIJAU_PIN, LOW);

  digitalWrite(ledPin, HIGH);
}


void setup()
{
  Serial.begin(115200);

  dht.begin();

  pinMode(LED_MERAH_PIN, OUTPUT);
  pinMode(LED_KUNING_PIN, OUTPUT);
  pinMode(LED_HIJAU_PIN, OUTPUT);

  digitalWrite(LED_MERAH_PIN, LOW);
  digitalWrite(LED_KUNING_PIN, LOW);
  digitalWrite(LED_HIJAU_PIN, LOW);
}


void loop()
{
  float temperature = dht.readTemperature();

  int rawSoil = analogRead(SOIL_PIN);

  float soilPercent =
    ((float)(SOIL_DRY_RAW - rawSoil) /
     (SOIL_DRY_RAW - SOIL_WET_RAW)) * 100.0;

  soilPercent = constrain(soilPercent, 0, 100);

  if (isnan(temperature))
  {
    Serial.println("Gagal membaca DHT22");
    delay(2000);
    return;
  }


  float dingin = suhuDingin(temperature);
  float normal = suhuNormal(temperature);
  float panas  = suhuPanas(temperature);

  float kering = tanahKering(soilPercent);
  float lembap = tanahLembap(soilPercent);
  float basah  = tanahBasah(soilPercent);

  
  float r1 = min(dingin, kering);
  float r2 = min(dingin, lembap);
  float r3 = min(dingin, basah);

  float r4 = min(normal, kering);
  float r5 = min(normal, lembap);
  float r6 = min(normal, basah);

  float r7 = min(panas, kering);
  float r8 = min(panas, lembap);
  float r9 = min(panas, basah);


  float numerator =
      (r1 * 0)   +
      (r2 * 50)  +
      (r3 * 100) +
      (r4 * 0)   +
      (r5 * 100) +
      (r6 * 100) +
      (r7 * 0)   +
      (r8 * 50)  +
      (r9 * 50);

  float denominator =
      r1 + r2 + r3 +
      r4 + r5 + r6 +
      r7 + r8 + r9;

  float output = 0;

  if (denominator > 0)
  {
    output = numerator / denominator;
  }


  String ledStatus;

  if (output >= 75)
  {
    setLED(LED_HIJAU_PIN);
    ledStatus = "HIJAU";
  }
  else if (output >= 25)
  {
    setLED(LED_KUNING_PIN);
    ledStatus = "KUNING";
  }
  else
  {
    setLED(LED_MERAH_PIN);
    ledStatus = "MERAH";
  }


  Serial.print("Suhu: ");
  Serial.print(temperature, 1);
  Serial.println(" C");

  Serial.print("Kelembapan Tanah: ");
  Serial.print(soilPercent, 1);
  Serial.println(" %");

  Serial.print("Output: ");
  Serial.println(output, 1);

  Serial.print("LED: ");
  Serial.println(ledStatus);

  Serial.println("--------------------");

  delay(2000);
}