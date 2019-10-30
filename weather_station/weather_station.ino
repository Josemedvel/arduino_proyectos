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
int currLine = 0;
float temperature;
float pressure;
float altitude;

void lineJump() {
  currLine ++;
  lcd.setCursor(0, currLine);
}
void cleanDisplay() {
  lcd.clear();
}
void readPrintTemp() {
  lcd.print("Temp = ");
  temperature = bmp.readTemperature();
  lcd.print(temperature);
  lcd.print(" *C");
}
void readPrintHumidity() {//this sensor is also able to measure temperature but is less precise than the BMP280, that is why I only use the humidity sensor
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  lcd.print("Humedad = ");
  lcd.print(event.relative_humidity);
  lcd.print(" %");
}
void readPrintPressure() {
  lcd.print("Presion = ");
  pressure = bmp.readPressure() / 100; // I divide by 100 because Pascal is very small and less used than hPa or mbar
  lcd.print(pressure);
  lcd.print(" hPa");
}
void readPrintAltitude() {
  lcd.print("Altura = ");
  altitude = bmp.readAltitude(1020); //the argument is this function should be the current air pressure in your zone and moment
  lcd.print(altitude);
  lcd.print(" m");
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  dht.humidity().getSensor(&sensor);
  lcd.begin(20, 4);
  lcd.backlight();
  Serial.println("Starting");
  delay(500);

  if (!bmp.begin()) {
    Serial.println("Try to reboot");
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
  cleanDisplay();
  lcd.setCursor(0, 0);
  readPrintTemp();
  lineJump();
  readPrintPressure();
  lineJump();
  readPrintAltitude();
  lineJump();
  readPrintHumidity();
  currLine = 0;
  delay(2000);
}
