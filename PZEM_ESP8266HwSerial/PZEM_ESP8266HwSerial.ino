#include <PZEM004T.h>

/*
   An example on how to use ESP8266 hardware serial with PZEM004T

   Note: ESP8266 UART0 pins GPIO1 (TX) and GPIO3 (RX) are usually connected
   to an onboard USB2serial converter, it can affect PZEM communication.
   It's better not to share USB2serial pins for PZEM communication

   Default UART0 pins could be swapped to gpio13(rx),gpio15(tx)
   Note: gpio15 pin must be pulled lOW on esp start, otherwise it won't boot from eeprom.
   PZEM004T always pulls it's RX pin HIGH via optocoupler's led, so make sure to workaround it anyhow while esp boots up
*/


/*
 * PDAControl
 * Documentation PDAControl English:
 * http://pdacontrolen.com/meter-pzem-004t-with-arduino-esp32-esp8266-python-raspberry-pi/
 * Documentacion PDAControl Español:
 * http://pdacontroles.com/medidor-pzem-004t-con-arduino-esp32-esp8266-python-raspberry-pi/
 * Video Tutorial : https://youtu.be/qt32YT_1oH8
 * 
 */

HardwareSerial hwserial(UART0);     // Use hwserial UART0 at pins GPIO1 (TX) and GPIO3 (RX)
PZEM004T pzem(&hwserial);           // Attach PZEM to hwserial
IPAddress ip(192,168,1,1);  

bool pzemrdy = false;   

void setup() {
   hwserial.swap();           // (optionally) swap hw_serial pins to gpio13(rx) (D7) , gpio15(tx)(D8)
   Serial1.begin(115200);     /* Since UART0 is occupied with PZEM004T, we use UART1 to output data to serial monitor
                                 UART1 uses hwserial at pin GPIO2 (D4)
                              */

  
   while (!pzemrdy) {
      Serial1.println("Connecting to PZEM...");
      pzemrdy = pzem.setAddress(ip);
      delay(1000);
   }
} 

void loop() {

  float v = pzem.voltage(ip);
  if (v < 0.0) v = 0.0;
   Serial1.print(v);Serial1.print("V; ");

  float i = pzem.current(ip);
   if(i >= 0.0){ Serial1.print(i);Serial1.print("A; ");   }

  float p = pzem.power(ip);
   if(p >= 0.0){ Serial1.print(p);Serial1.print("W; "); }

  float e = pzem.energy(ip);
   if(e >= 0.0){ Serial1.print(e);Serial1.print("Wh; "); }

  Serial1.println();

  delay(3000);
}
