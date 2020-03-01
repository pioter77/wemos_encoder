
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
int encoder0Pos=0;
int pinA_last;
int n=LOW;

int menu_rows=8;
int menu_vertical_space=63-8+1;
int menu_slider_height=1;
byte counter1=1;
unsigned long time_last_buton1_pressed=0;
unsigned long time_last_buton2_pressed=0;
unsigned long time_last_buton3_pressed=0;

//int encoder0Pos_prev=encoder0Pos;
volatile bool flag3=0;
volatile bool flag1=0;
volatile bool flag2=0;
volatile bool flag4=0; //aktualizacja wyswietlacza

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

void menu_display();
int menu_slider_calc(int *rows,int *vert_space);
void menu_row( int lp,const char txt[],int x_start,int *actual_pos);

Adafruit_SSD1306 OLED(-1);

void setup() {
  Serial.begin(115200);
  Serial.println("Basic Encoder Test:");
 menu_slider_height=menu_slider_calc(&menu_rows,&menu_vertical_space);

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
 //Serial.println(menu_slider_height);
}

void loop()
{
  
  unsigned long ac_time=millis();


  if(flag1 && ac_time-time_last_buton1_pressed>del_btn1)
  {
    flag4=1;

    encoder0Pos-=8;
    counter1--;
    if(encoder0Pos<0) 
    {encoder0Pos=0;
    counter1=1;
    }
     Serial.println(encoder0Pos);

     flag1=0;
   time_last_buton1_pressed=millis();
    interrupts();
  }


   if(flag2 && ac_time-time_last_buton2_pressed>del_btn1)
  {
    flag4=1;

    encoder0Pos=0;
    counter1=1;
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
     if(encoder0Pos>56)//jakby zmieniac ilosc plansz to to by ptrzeba bylo razy 2 dla 2giej planszy?
     { encoder0Pos=56;//albo dac flage drugiej planszy na 1 chyba lepsze rozwiazanie i potem menu_display na podstawie tej flagi wielkosci bedzie odpowedniego ifa wyswietlac
     counter1=8;
     }
     Serial.println(encoder0Pos);

     flag3=0;
   time_last_buton3_pressed=millis();
    interrupts();
  }

 
  
  if(flag4)
  {
  menu_display();
 flag4=0;
 //interrupts();
}

}

int menu_slider_calc(int *rows,int *vert_space)
{
  int ret=(*vert_space/ *rows<1) ? 1: *vert_space/ *rows;
  return ret;
}

void menu_row( int lp,const char txt[],int x_start,int *actual_pos)
{
  int y_tmp=(lp==0 || lp%8==0)  ? 0:(lp*8);

  if(lp==*actual_pos/8)//wybrana wiec podswietlic bo na tej opcje najechalismy
  {
    OLED.fillRect(0,encoder0Pos,110,9,WHITE);
    OLED.setTextColor(BLACK);
    OLED.setCursor(x_start,y_tmp);
    OLED.println(txt);
    OLED.setTextColor(WHITE);
  }else{
    OLED.setCursor(x_start,y_tmp);
    OLED.println(txt);
  }
 
 // OLED.println("tescik");
  //Serial.println("fcja");
  //Serial.println(*txt);
}

void menu_display()
{
   OLED.clearDisplay();

OLED.setCursor(64,0);
OLED.println(encoder0Pos);
//OLED.setCursor(0,8);
//OLED.println(counter1);
OLED.setCursor(111,0);
OLED.println(counter1);
OLED.setCursor(117,0);
OLED.println("/");
OLED.setCursor(123,0);

OLED.println(8);
OLED.drawLine(126,8,126,63,WHITE);
OLED.drawRect(125,counter1*7+1,3,menu_slider_height,WHITE);



menu_row(0,"pozycja:",0,&encoder0Pos);
menu_row(1,"opcja1",0,&encoder0Pos);
menu_row(2,"opcja2",0,&encoder0Pos);
menu_row(3,"opcja3",0,&encoder0Pos);
menu_row(4,"opcja4",0,&encoder0Pos);
menu_row(5,"opcja5",0,&encoder0Pos);
menu_row(6,"opcja6",0,&encoder0Pos);
menu_row(7,"opcja7",0,&encoder0Pos);


 OLED.display(); //output 'display buffer' to screen  
}