#include <VirtualWire.h>
/*
ver ::cl 20120520
Configuracion basica para modulo transmisor RT 11
Utiliza libreria VirtualWire.h
pin 01 entrada desde Arduino pin digital 2
pin 02 Tierra
pin 07 tierra
pin 08 antena externa
pin 09 tierra
pin 10 5v
*/

/*
* Función CRC
*/
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


uint8_t origen[2] = {B00000000,B00000010};  // Grupo origen
uint8_t destino[2] = {B00000000,B00000010};  // Grupo destino
// uint8_t destino[2] = {0b00000000,0b00000000};  // Grupo destino
uint8_t secuencia = B00000001;  // Numero de paquete actual enviado
uint8_t total = B00000101; // Cantidad total de veces a enviar el paquete (5)
uint8_t msg_1[8] = {B00000000, //" "
                B00000000, //" "
                B00000000, //" "
                B00000000, //" "
                B01000111, //"G"
                B00100000, //" "
                B00110000, //"0"
                B00110010 //"2"
};
uint8_t crc = B00010011;

uint8_t mensaje_completo[16];



const char *msg = "G 02";



void setup(){
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_set_tx_pin(2);    
  Serial.begin(9600);
  /*
  *Origen
  */
  mensaje_completo[0] = origen[0];
  mensaje_completo[1] = origen[1];
  /*
  *Destino
  */
  mensaje_completo[2] = destino[0];
  mensaje_completo[3] = destino[1];

  /*
  * CRC
  */

  mensaje_completo[4] = crc;
  mensaje_completo[5] = crc;

  /*
  * Secuencia
  */

  mensaje_completo[6] = secuencia;

  /*
  * Total de paquetes
  */

  mensaje_completo[7] = total;

  /*
  * Mensaje
  */
  mensaje_completo[8] = msg_1[0];
  mensaje_completo[9] = msg_1[1];
  mensaje_completo[10] = msg_1[2];
  mensaje_completo[11] = msg_1[3];
  mensaje_completo[12] = msg_1[4];
  mensaje_completo[13] = msg_1[5];
  mensaje_completo[14] = msg_1[6];
  mensaje_completo[15] = msg_1[7];


  Serial.println("configurando envio");
}





void loop(){

   const char* Data = "1101000101000111";
   char* Result = MakeCRC(Data);                         // Calculate CRC
   
   Serial.print("CRC of [");
   Serial.print(Data);
   Serial.print("] is [");
   Serial.print(Result);
   Serial.println("] with P=[100101]");

   Serial.println("Enviamos tooooodo el binario: ");
  for(int i = 0; i<16; i++){
    Serial.print(mensaje_completo[i],BIN);
  }

  /*
  * Envío 
  */
  vw_send(mensaje_completo, 16);

  digitalWrite(13, false);
  // vw_send((byte *)origen, strlen(origen));
  vw_wait_tx();
  delay(1000);
  Serial.println("mensaje enviado");
   
}