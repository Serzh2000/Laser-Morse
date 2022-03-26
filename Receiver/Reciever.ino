int LDR_Pin = A0; //analog pin 0
int led = 13;

// 10k between GND and A0
// LDR between 5V and A0

#define MORSE_EMPTY   0
// Этими символами мы будем обозначать точки и тире.
#define MORSE_DOT     '*'
#define MORSE_DOT2     '+'
#define MORSE_TIRE    '-'
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

char** currentLayout;
char** newLayout;


void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);  

  morseSymbolLen = 0;

 
  newMorseSignal = MORSE_EMPTY;
  
}
int counter_high = 0;
int counter_low = 0;
int i;
void loop(){

  int LDRReading = analogRead(LDR_Pin); 
  if (LDRReading >= 800){
  counter_high++ ;
   if ( counter_low > 0 ){
  //  Serial.print("Low\t");
 //   Serial.print(counter_low);
//     Serial.print("\n");
   }
   if ( counter_low > 1200) {
    //  for (i = 0; i< morseSymbolLen; i++) {
     
   // Serial.print( currentLayout[i]);
    // }
    sendMorseSymbol();
    morseSymbolLen=0;
      Serial.println();
    //currentLayout[0]=" ";
    //currentLayout[1]=" ";
    //currentLayout[2]=" ";
    //currentLayout[3]=" ";
    //currentLayout[4]=" ";
    //currentLayout[5]=" ";
    //currentLayout[6]=" ";
    //currentLayout[7]=" ";
    //currentLayout[8]=" ";
    //morseSymbolLen=0;
  }
     counter_low=0;
     digitalWrite(led, HIGH);

  
  } else {
//   Serial.print(".");
  counter_low++;
  if ( counter_high > 0 ){
//      Serial.print("High\t");  
//   Serial.print(counter_high);

  }
  if ( (counter_high < 1200 ) &&( counter_high >350)){
//      Serial.print(counter_high);
    Serial.print(".");
    newMorseSignal="*";
    morseSymbol[morseSymbolLen++] = newMorseSignal;
   // currentLayout[morseSymbolLen]=".";
   // morseSymbolLen=morseSymbolLen+1;
  }
  if ( counter_high > 1200 ){
 //         Serial.print(counter_high);
        Serial.print("-");
        newMorseSignal="-";
        morseSymbol[morseSymbolLen++] = newMorseSignal;
       // currentLayout[morseSymbolLen]="-";
       //     morseSymbolLen=morseSymbolLen+1;
  }

      counter_high=0;
      digitalWrite(led, LOW);

  }

 
}
void sendMorseSymbol() {
  boolean est;
  int i, j;
  est=-1;
  if (morseSymbolLen < 1) {
    return;
  }
 

   Serial.print(morseSymbolLen);
   
      // Символ из таблицы кодов Морзе соответствует введенному символу.
      //  Отправим символ на компьютер.
    String str1;
    String strm;
    str1="";
    for (i=0;i<morseSymbolLen;i++){  
     str1=str1+morseSymbol[i];
    }
   //  Serial.print(code[6]);
     
      
     
      
    for (i=0;i<56;i++){
      String str2(code[i]);
          
      if (str1.compareTo(str2)==0){
        //Serial.print(str1);
       // Serial.print("est");
       Serial.print(str2);
       est=true;
      break;
      }
       }
      if (est!=-1){
      Serial.print(layoutLatin[i]);
     
    }
      morseSymbolLen = 0;
      return;
   
}
