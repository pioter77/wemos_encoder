
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_ADDR   0x3C
#define btn1 0 //d3
#define btn2 2 //d4
#define btn3 14 //d5

#define del_btn1 350
#define del_encoder 250

int val;
int encoder0Pos=8;
int pinA_last;
int n=LOW;

byte counter1=0;
unsigned long time_last_buton1_pressed=0;
unsigned long time_last_buton2_pressed=0;
unsigned long time_last_buton3_pressed=0;

//int encoder0Pos_prev=encoder0Pos;
volatile bool flag3=0;
volatile bool flag1=0;
volatile bool flag2=0;
volatile bool flag4=0;

void ICACHE_RAM_ATTR isr1()
{
noInterrupts();
flag1=1;
}

void ICACHE_RAM_ATTR isr2()
{
noInterrupts();
flag2=1;
}

void ICACHE_RAM_ATTR isr3()
{
noInterrupts();
flag3=1;
}

Adafruit_SSD1306 OLED(-1);

void setup() {
  Serial.begin(115200);
  Serial.println("Basic Encoder Test:");
 
  pinMode(btn1,INPUT_PULLUP);
   pinMode(btn2,INPUT_PULLUP);
    pinMode(btn3,INPUT_PULLUP);
  
  //pinA_last=digitalRead(pinA_last);
  attachInterrupt(digitalPinToInterrupt(btn1),isr1,FALLING);
   attachInterrupt(digitalPinToInterrupt(btn2),isr2,FALLING);
   attachInterrupt(digitalPinToInterrupt(btn3),isr3,FALLING);

    OLED.begin(SSD1306_SWITCHCAPVCC,OLED_ADDR);
  OLED.clearDisplay();
  OLED.setFont(NULL);
   OLED.setTextWrap(false);
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
  flag4=1;
 
}

void loop()
{
  
  unsigned long ac_time=millis();


  if(flag1 && ac_time-time_last_buton1_pressed>del_btn1)
  {
    flag4=1;

    encoder0Pos-=8;
    counter1--;
    if(encoder0Pos<8) 
    {encoder0Pos=8;
    counter1=0;
    }
     Serial.println(encoder0Pos);

     flag1=0;
   time_last_buton1_pressed=millis();
    interrupts();
  }


   if(flag2 && ac_time-time_last_buton2_pressed>del_btn1)
  {
    flag4=1;

    encoder0Pos=8;
    counter1=0;
    Serial.println(encoder0Pos);

     flag2=0;
   time_last_buton2_pressed=millis();
    interrupts();
  }

   if(flag3 && ac_time-time_last_buton3_pressed>del_btn1)
  {
    flag4=1;

    encoder0Pos+=8;
    counter1++;
     if(encoder0Pos>57)
     { encoder0Pos=57;
     counter1=7;
     }
     Serial.println(encoder0Pos);

     flag3=0;
   time_last_buton3_pressed=millis();
    interrupts();
  }

 
  
  if(flag4)
  {
   // noInterrupts();
 OLED.clearDisplay();
 OLED.setCursor(0,0);
OLED.println("pozycja:");
OLED.setCursor(64,0);
OLED.println(encoder0Pos);
OLED.setCursor(0,10);
OLED.println(counter1);
OLED.setCursor(108,0);
OLED.println(counter1+1);
OLED.setCursor(114,0);
OLED.println("-");
OLED.setCursor(120,0);

OLED.println(8);
OLED.drawLine(126,8,126,63,WHITE);
OLED.drawRect(125,encoder0Pos,3,7,WHITE);

 OLED.display(); //output 'display buffer' to screen  
 flag4=0;
 //interrupts();
}
}
