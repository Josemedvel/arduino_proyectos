#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>


#define ANCHO 128
#define ALTO 64
#define OLED_RESET 4
Adafruit_SSD1306 oled(ANCHO,ALTO, &Wire, OLED_RESET);
void pintarLineas(){
  oled.drawFastHLine(0,0,ANCHO-1,WHITE);
  //oled.drawLine(0,0,0,ALTO-1, WHITE);
  oled.drawFastHLine(0,ALTO-1,ANCHO-1, WHITE);
  //oled.drawLine(ANCHO-1,0,ANCHO-1,ALTO-1, WHITE);
  //oled.drawLine((ANCHO-1)/2,0,(ANCHO-1)/2,ALTO-1, WHITE);
  oled.display();
}
void pintarPelota(int x, int y){
  oled.drawPixel(x,y,WHITE);
  oled.display();
}

void pintarJugador(int list[3]){
  oled.drawFastVLine(list[0],list[1]-16/2,16, WHITE);
  oled.display();
}
void compMovJug(int Xvalue, int jug[2],int velJ){
  if(Xvalue > 523){
    velJ = map(Xvalue,0,1023,0,6);
    if((velJ + jug[1]) > ALTO-1){
      velJ=0;
    }
  }
  else if(Xvalue < 503){
    velJ = map(Xvalue,0,1023,6,0)*(-1);
    if((velJ + jug[1]) < 0){
      velJ=0;
    }
    
  }
 else{
  velJ = 0;
 }
}

const int pinJoyX = A0;
const int pinJoyY = A1;
const int pinJoyButton = 9;
int pelX = (ANCHO-1)/2;
int pelY = (ALTO-1)/2;
int dirP;
void moverPelota(int dirP, int velP){
  float coseno = cos(dirP*2*PI/360)*velP;
  Serial.print("coseno :" );
  Serial.println(coseno);
  int newXP = pelX + round(coseno);
  Serial.print("newXP :" );
  Serial.println(newXP);
  float seno = sin(dirP*2*PI/360)*velP;
  Serial.print("seno :" );
  Serial.println(seno);
  int newYP = pelY + round(seno);
  Serial.print("newYP :" );
  Serial.println(newYP);
  pelX = newXP;
  pelY = newYP;
  pintarPelota(newXP, newYP);
}
int velP = 2;
int velJ1 = 1;

int jug1[2] = {0,(ALTO-1)/2};   
void setup() {
  Serial.begin(4800);
  randomSeed(analogRead(0));
  dirP = random(0,361);
  Serial.print("numero random = ");
  Serial.println(dirP);
  //delay(100000);
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  pinMode(pinJoyButton , INPUT_PULLUP);   //activar resistencia pull up 
  pintarLineas();
  pintarPelota(pelX,pelY);
  pintarJugador(jug1);
  Serial.println(dirP);
}

void loop() {
  oled.clearDisplay();
  
  int Xvalue = 0;
  Xvalue = analogRead(pinJoyX);

 //aquí empiezan los problemas, funciona asi, pero no en una función 
  if(Xvalue > 523){
    velJ1 = map(Xvalue,0,1023,0,6);
    if((velJ1 + jug1[1]) > ALTO-1){
      velJ1=0;
    }
  }
  else if(Xvalue < 503){
    velJ1 = map(Xvalue,0,1023,6,0)*(-1);
    if((velJ1 + jug1[1]) < 0){
      velJ1=0;
    }
    
  }
 else{
  velJ1 = 0;
 }

 //hasta aquí
  //compMovJug(Xvalue, jug1, velJ1);
 
  
  //Serial.println(velJ1);
  jug1[1] += velJ1; 
  pintarLineas();
  pintarJugador(jug1);
  moverPelota(dirP, velP);
  /*Serial.print("X:" );
  Serial.print(Xvalue);
  Serial.println(" | Pulsador: ");
  */
}
