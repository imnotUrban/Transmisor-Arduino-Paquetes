/*
ver ::cl 20120520
Configuracion basica para modulo receptor  RR 10
Utiliza libreria VirtualWire.h
pin 01 5v
pin 02 Tierra
pin 03 antena externa
pin 07 tierra
pin 10 5v
pin 11 tierra
pin 12 5v
pin 14 Arduino pin digital 2
pin 15 5v
*/
#include <VirtualWire.h>

char* MakeCRC(const char* BitString)
{
   static char Res[6];                                 // CRC Result
   char CRC[5];
   int i;
   char DoInvert;
   
   for (i = 0; i < 5; ++i) CRC[i] = 0;                 // Init before calculation
   
   for (i = 0; i < strlen(BitString); ++i)
   {
      DoInvert = ('1' == BitString[i]) ^ CRC[4];       // XOR required?

      CRC[4] = CRC[3];
      CRC[3] = CRC[2];
      CRC[2] = CRC[1] ^ DoInvert;
      CRC[1] = CRC[0];
      CRC[0] = DoInvert;
   }
      
   for (i = 0; i < 5; ++i) Res[4 - i] = CRC[i] ? '1' : '0'; // Convert binary to ASCII
   Res[5] = '\0';                                      // Set string terminator

   return Res;
}


// Cambiar destino[1] para comunicarse con otro grupo en unicast
uint8_t destino[2] = {B00000000, B00000010};

void setup(){
    Serial.begin(9600);
    vw_set_ptt_inverted(true); 
    vw_setup(2000);
    vw_set_rx_pin(2);
    vw_rx_start();
}

void loop(){
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)){
    if((destino[1] == buf[3]) || (buf[3] == 0)){
      Serial.println("Numero grupo\n");
      Serial.print(buf[3]);
      Serial.print("\n");
    };
  }
}