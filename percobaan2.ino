#include <DHT.h>

#define SOIL_PIN 34

#define DHT_PIN 4
#define DHT_TYPE DHT22

#define LED_MERAH_PIN  25
#define LED_KUNING_PIN 26
#define LED_HIJAU_PIN  27

DHT dht(DHT_PIN, DHT_TYPE);

const int SOIL_DRY_RAW = 3000;
const int SOIL_WET_RAW = 1200;

const unsigned long SOIL_INTERVAL_MS = 500;
const unsigned long DHT_INTERVAL_MS  = 2000;

unsigned long lastSoilReadTime = 0;
unsigned long lastDHTReadTime  = 0;

float lastTemperature = 25.0;

unsigned long ledStartTime = 0;
unsigned long ledDurationMs = 0;

int activeLedPin = -1;

float trimf(float x, float a, float b, float c)
{
  if (x <= a || x >= c)
    return 0.0;

  if (x == b)
    return 1.0;

  if (x < b)
    return (x - a) / (b - a);

  return (c - x) / (c - b);
}

float mfKering(float x)
{
  if (x <= 10)
    return 1.0;

  if (x >= 35)
    return 0.0;

  return (35.0 - x) / 25.0;
}

float mfLembab(float x)
{
  return trimf(x, 20, 45, 70);
}

float mfBasah(float x)
{
  return trimf(x, 55, 100, 110);
}

float mfDingin(float x)
{
  return trimf(x, 15, 20, 28);
}

float mfNormal(float x)
{
  return trimf(x, 25, 28.5, 32);
}

float mfPanas(float x)
{
  return trimf(x, 30, 40, 45);
}

float mfSingkat(float x)
{
  return trimf(x, -5, 0, 10);
}

float mfSedang(float x)
{
  return trimf(x, 8, 14, 20);
}

float mfLama(float x)
{
  return trimf(x, 18, 30, 35);
}

float alphaSingkat = 0;
float alphaSedang  = 0;
float alphaLama    = 0;

float fuzzyInference(float soil, float temp)
{
  float kering = mfKering(soil);
  float lembab = mfLembab(soil);
  float basah  = mfBasah(soil);

  float dingin = mfDingin(temp);
  float normal = mfNormal(temp);
  float panas  = mfPanas(temp);

  float r1 = min(kering, dingin);
  float r2 = min(kering, normal);
  float r3 = min(kering, panas);

  float r4 = min(lembab, dingin);
  float r5 = min(lembab, normal);
  float r6 = min(lembab, panas);

  float r7 = min(basah, dingin);
  float r8 = min(basah, normal);
  float r9 = min(basah, panas);

  alphaSingkat = max(r7, r8);

  alphaSedang = max(
    r4,
    max(r5, max(r6, r9))
  );

  alphaLama = max(
    r1,
    max(r2, r3)
  );

  float sumWeighted = 0.0;
  float sumMembership = 0.0;

  for (float y = 0; y <= 30; y += 0.5)
  {
    float clipSingkat = min(
      alphaSingkat,
      mfSingkat(y)
    );

    float clipSedang = min(
      alphaSedang,
      mfSedang(y)
    );

    float clipLama = min(
      alphaLama,
      mfLama(y)
    );

    float aggregated = max(
      clipSingkat,
      max(clipSedang, clipLama)
    );

    sumWeighted += aggregated * y;
    sumMembership += aggregated;
  }

  if (sumMembership == 0)
  {
    return 0;
  }

  return sumWeighted / sumMembership;
}

float readSoilMoisturePercent()
{
  int raw = analogRead(SOIL_PIN);

  float percent =
    ((float)(SOIL_DRY_RAW - raw) /
     (SOIL_DRY_RAW - SOIL_WET_RAW)) * 100.0;

  percent = constrain(
    percent,
    0,
    100
  );

  return percent;
}

void allLedOff()
{
  digitalWrite(LED_MERAH_PIN, LOW);
  digitalWrite(LED_KUNING_PIN, LOW);
  digitalWrite(LED_HIJAU_PIN, LOW);
}

void updateOutput(
  float durasi,
  unsigned long now
)
{
  if (durasi < 8.0)
  {
    allLedOff();

    digitalWrite(
      LED_HIJAU_PIN,
      HIGH
    );

    activeLedPin = LED_HIJAU_PIN;

    return;
  }

  if (durasi < 18.0)
  {
    allLedOff();

    digitalWrite(
      LED_KUNING_PIN,
      HIGH
    );

    activeLedPin = LED_KUNING_PIN;

    ledStartTime = now;

    ledDurationMs =
      (unsigned long)(durasi * 1000.0);

    return;
  }

  allLedOff();

  digitalWrite(
    LED_MERAH_PIN,
    HIGH
  );

  activeLedPin = LED_MERAH_PIN;

  ledStartTime = now;

  ledDurationMs =
    (unsigned long)(durasi * 1000.0);
}

void setup()
{
  Serial.begin(115200);

  delay(500);

  pinMode(
    LED_MERAH_PIN,
    OUTPUT
  );

  pinMode(
    LED_KUNING_PIN,
    OUTPUT
  );

  pinMode(
    LED_HIJAU_PIN,
    OUTPUT
  );

  allLedOff();

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

    float soilPercent =
      readSoilMoisturePercent();

    float durasi =
      fuzzyInference(
        soilPercent,
        lastTemperature
      );

    Serial.print("Suhu: ");
    Serial.print(lastTemperature, 1);
    Serial.println(" C");

    Serial.print("Kelembapan: ");
    Serial.print(soilPercent, 1);
    Serial.println(" %");

    updateOutput(
      durasi,
      now
    );
  }

  if (activeLedPin != LED_HIJAU_PIN && activeLedPin != -1)
  {
    if (now - ledStartTime >= ledDurationMs)
    {
      digitalWrite(
        activeLedPin,
        LOW
      );

      activeLedPin = -1;
    }
  }

  delay(10);
}