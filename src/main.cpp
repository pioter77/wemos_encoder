#include <Arduino.h>
#define pinA 12 //d6
#define pinB 13 //d7
#define btn1 14 //d5
int val;
int encoder0Pos=0;
int pinA_last;
int n=LOW;

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
  if(flag1)
  {
    encoder0Pos=0;
     pinA_last=digitalRead(pinA);
     Serial.println(encoder0Pos);

     flag1=0;
    delay(350);
    interrupts();
  }

 // n=digitalRead(pinA);
  if(flag2)
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
      delay(50);
      Serial.println(encoder0Pos);
       flag2=0;
      interrupts();
  }
  //pinA_last=n;
 

}