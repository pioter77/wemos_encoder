#include <Arduino.h>
#define pinA 12 //d6
#define pinB 13 //d7
#define btn1 14 //d5

#define del_btn1 350
#define del_encoder 50

int val;
int encoder0Pos=0;
int pinA_last;
int n=LOW;

unsigned long time_last_buton_pressed=0;
unsigned long time_last_ancoder_active=0;

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

void setup() {
  Serial.begin(115200);
  Serial.println("Basic Encoder Test:");
  pinMode(pinA,INPUT);
  pinMode(pinB,INPUT);
  pinMode(btn1,INPUT_PULLUP);
  
  //pinA_last=digitalRead(pinA_last);
  attachInterrupt(digitalPinToInterrupt(btn1),isr1,FALLING);
  attachInterrupt(digitalPinToInterrupt(pinA),isr2,RISING);
   
}

void loop()
{
  unsigned long ac_time=millis();

  if(flag1 && ac_time-time_last_buton_pressed>del_btn1)
  {
    encoder0Pos=0;
     pinA_last=digitalRead(pinA);
     Serial.println(encoder0Pos);

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

        encoder0Pos--;
      }else{
           // Serial.println("petla3");

        encoder0Pos++;
      }
      time_last_buton_pressed=millis();
      Serial.println(encoder0Pos);
       flag2=0;
      interrupts();
  }
  //pinA_last=n;
 

}