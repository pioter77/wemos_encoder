#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_ADDR   0x3C
#define pinA 12 //d6
#define pinB 13 //d7
#define btn1 14 //d5

#define del_btn1 350
#define del_encoder 250

int val;
int encoder0Pos=0;
int pinA_last;
int n=LOW;

unsigned long time_last_buton_pressed=0;
unsigned long time_last_ancoder_active=0;
int encoder0Pos_prev=encoder0Pos;
volatile bool flag3=0;
volatile bool flag1=0;
volatile bool flag2=0;

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

Adafruit_SSD1306 OLED(-1);

void setup() {
  Serial.begin(115200);
  Serial.println("Basic Encoder Test:");
  pinMode(pinA,INPUT);
  pinMode(pinB,INPUT);
  pinMode(btn1,INPUT_PULLUP);
  
  //pinA_last=digitalRead(pinA_last);
  attachInterrupt(digitalPinToInterrupt(btn1),isr1,FALLING);
  attachInterrupt(digitalPinToInterrupt(pinA),isr2,ONLOW);
   
    OLED.begin(SSD1306_SWITCHCAPVCC,OLED_ADDR);
  OLED.clearDisplay();
  OLED.setFont(NULL);
   OLED.setTextWrap(false);
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
 
}

void loop()
{
  
  unsigned long ac_time=millis();
  if(ac_time-time_last_ancoder_active<del_encoder) 
  {
    flag2=0;
  interrupts();
  }

  if(flag1 && ac_time-time_last_buton_pressed>del_btn1)
  {
    flag3=1;

    encoder0Pos=0;
     pinA_last=digitalRead(pinA);
    // Serial.println(encoder0Pos);

     flag1=0;
   time_last_buton_pressed=millis();
    interrupts();
  }

 // n=digitalRead(pinA);
  if(flag2 && ac_time-time_last_ancoder_active>del_encoder)
  {
   // n=flag2;
    //Serial.println("petla1");
      if(digitalRead(pinB)==LOW)
      {
           // Serial.println("petla2");
           flag3=1;
        encoder0Pos--;
        if(encoder0Pos<0) encoder0Pos=0;
      }else{
           // Serial.println("petla3");
        flag3=1;
        encoder0Pos++;
        if(encoder0Pos>57) encoder0Pos=57;

      }
      time_last_ancoder_active=millis();
     // Serial.println(encoder0Pos);
       flag2=0;
      interrupts();
  }
  //pinA_last=n;
  
  if(flag3)
  {
   // noInterrupts();
 OLED.clearDisplay();
 OLED.setCursor(0,0);
OLED.println("pozycja:");
OLED.setCursor(64,0);
OLED.println(encoder0Pos);
OLED.drawLine(126,0,126,63,WHITE);
OLED.drawRect(125,encoder0Pos,3,7,WHITE);

 OLED.display(); //output 'display buffer' to screen  
 flag3=0;
 //interrupts();
}
}