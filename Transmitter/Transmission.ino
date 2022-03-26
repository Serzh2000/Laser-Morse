// Author Erik Linder
// Released 2011 under GNU GPLv3
//
// Usage: morse( <pin number>, <speed WPM>, <1=beep, 0=PTT> )
//        sendmsg( "<text-to-send>" )
//

#include <Morse.h>

// Use pin 13 (built-in LED of Arduino 2009)
Morse morse(13, 24, 1);

void setup()
{

}

void loop()
{
  morse.sendmsg("ETU_LETI");
  delay (2000);
  morse.sendmsg("_1886_");
  delay (2000);
}
