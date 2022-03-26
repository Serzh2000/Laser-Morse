#include <LCD.h>
#include <LiquidCrystal_I2C.h>
 #include <Wire.h>
// Define I2C Address where the PCF8574* is
#define I2C_ADDR    0x27 
 
// Define LCD Pins
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
 
// Initialize LiquadCrystal with pin setup
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

 int pos_lcd,pos_lcd2;




int LDR_Pin = A0; //analog pin 0
int led = 13;

// 10k between GND and A0
// LDR between 5V and A0

// настроить чувствительность фотоэлемента
#define LEVEL_LDR    800

#define MORSE_EMPTY   0

// Этими символами мы будем обозначать точки и тире.
#define MORSE_DOT     '*'
#define MORSE_DASH    '-'


  //Длина точки и тире для 24 слов в минуту (wpm) = 150 мс и 700 мс,
  // Для 12 wpm = 700 и 1200

#define MORSE_TIME_DOT 150
#define MORSE_TIME_DASH 700

// Максимальная длина символа азбуки Морзе (в точках и тире)
#define MAX_MORSE_SYMBOL_LENGTH  8
char* morseSymbol[MAX_MORSE_SYMBOL_LENGTH];
unsigned int morseSymbolLen;
char* newMorseSignal; // Новый введенный сигнал - точка или тире.

// Таблица кодов Морзе. N-ный элемент кода соответствует n-ному символу раскладки.
char* code[] = {
  "*-","-***","*--","--*","-**","*","***-","--**","**","*---",
  "-*-","*-**","--","-*","---","*--*","*-*","***","-","**-",
  "**-*","****","-*-*","---*","----","--*-","-*--","-**-","**-**","**--",
  "*-*-",
  "*----","**---","***--","****-","*****","-****","--***","---**","----*","-----",
  "......","*-*-*-","---***","-*-*-","-*--*-","*----*","*-**-*","-****-","-**-*","**--**","--**--",
  "-***-","********","*--*-*","**-*-",
  ""
};

// Кириллическая раскладка.
char* layoutCyrillic[] = {
  "а","б","в","г","д","е","ж","з","и","й",
  "к","л","м","н","о","п","р","с","т","у",
  "ф","х","ц","ч","ш","щ","ы","ь","э","ю",
  "я",
  "1","2","3","4","5","6","7","8","9","0",
  ".",",",":",";","(","\'","\"","-","/","?","!",
  " *DELIMITER* "," *ERR* ","@"," *END* ",
  ""
  };
// Латинская раскладка.
char* layoutLatin[] = {
  "a","b","w","g","d","e","v","z","i","j",
  "k","l","m","n","o","p","r","s","t","u",
  "f","h","c","o","ch","q","y","x","e","u",
  "a",
  "1","2","3","4","5","6","7","8","9","0",
  ".",",",":",";","(","\'","\"","-","/","?","!",
  " *DELIMITER* "," *ERR* ","@"," *END* ",
  ""
};



void setup(){
  Serial.begin(9600);
  
  pos_lcd=0;
  lcd.begin (16,2);
 
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
 
  // Reset cursor to home
  lcd.home ();                 
 
  // Print Hello World
 // lcd.print("GOTOV priem"); 
  
  
  
  pinMode(led, OUTPUT);  

  morseSymbolLen = 0;

 
  newMorseSignal = MORSE_EMPTY;
  
}
int counter_high = 0;
int counter_low = 0;
int i;
void loop(){

  int LDRReading = analogRead(LDR_Pin); 
  if (LDRReading >= LEVEL_LDR){
  counter_high++ ;

   if ( counter_low > MORSE_TIME_DASH) {
    sendMorseSymbol();
    morseSymbolLen=0;
  }

     counter_low=0;
     digitalWrite(led, HIGH);

  
  } else {
  counter_low++;

  if ( (counter_high < MORSE_TIME_DASH ) &&( counter_high >MORSE_TIME_DOT)){
    Serial.print(".");
    newMorseSignal="*";
    morseSymbol[morseSymbolLen++] = newMorseSignal;
  }
  if ( counter_high > MORSE_TIME_DASH ){
        Serial.print("-");
        newMorseSignal="-";
        morseSymbol[morseSymbolLen++] = newMorseSignal;

  }

      counter_high=0;
      digitalWrite(led, LOW);

  }

 
}
void sendMorseSymbol() 
{
  boolean est;
  int i, j;
  est=-1;
  if (morseSymbolLen < 1) 
  {
  return;
  }
 

   
      // Символ из таблицы кодов Морзе соответствует введенному символу.
      //  Отправим символ на компьютер.
    String str1;
    str1="";
    for (i=0;i<morseSymbolLen;i++)
  {  
  str1=str1+morseSymbol[i];
  }     
      
    for (i=0;i<56;i++)
  {
  String str2(code[i]);
  if (str1.compareTo(str2)==0)
    {
    Serial.print(str2);
    est=true;
    break;
    }
       }
      
      if (est!=-1)
  {
  Serial.print(layoutLatin[i]);

  //   Вывод символа на LCD экран
  if (pos_lcd>16 ) 
    {
    Serial.print("vtorayStroka");
    pos_lcd2=pos_lcd-17;  
    Serial.print(pos_lcd);
    lcd.setCursor(pos_lcd2,1 ); //Пишем в LCD на 2 строк
    }
  pos_lcd=pos_lcd+1;
  
  if (pos_lcd>32) 
    {
    lcd.clear();
    pos_lcd=0;
    }
  lcd.print(layoutLatin[i]); 
  lcd.setBacklight(HIGH);     // Backlight on
  }
      morseSymbolLen = 0;
      return;
   
}
