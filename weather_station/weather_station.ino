#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
#define DHTPIN 4
#define DHTTYPE    DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

float Temperatura;
float Presion;

void setup() {
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  dht.humidity().getSensor(&sensor);
  lcd.begin(20,4);
  lcd.backlight();
  Serial.println("iniciando");

  if (!bmp.begin()) {
    Serial.println("no encontrado");
    //while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
    lcd.clear();
    sensors_event_t event;
    dht.humidity().getEvent(&event);
    lcd.setCursor(0,0);
    lcd.print("Temp = ");
    lcd.print(bmp.readTemperature());
    lcd.print(" *C");
    lcd.setCursor(0,1);
    lcd.print("Presion = ");
    lcd.print(bmp.readPressure()/100);
    lcd.print(" hPa");
    lcd.setCursor(0,2);
    lcd.print("Altura = ");
    lcd.print(bmp.readAltitude(1020)); /* Adjusted to local forecast! */
    lcd.print(" m");
    lcd.setCursor(0,3);
    lcd.print("Humedad = ");
    lcd.print(event.relative_humidity);
    lcd.print(" %");
    delay(2000);
}
