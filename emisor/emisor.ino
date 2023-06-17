#include <VirtualWire.h>

uint8_t ceros = B00000000;
uint8_t origen[2] = {B00000000,B00000010};  // Grupo origen
uint8_t destino[2] = {B00000000,B00000010};  // Grupo destino
uint8_t secuencia = B00000000;  // Numero de paquete actual enviado
uint8_t total = B00000101; // Cantidad total de veces a enviar el paquete (5)

const char *msg = "G 02";
uint8_t mensaje[8] = {0};
uint8_t mensaje_completo[16] = {0};

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


void setup(){
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_set_tx_pin(2);    
  Serial.begin(9600);

  int length = strlen(msg);
  int startIndex = 8 - length; // Para guardar en el arreglo de derecha a izquierda

  // Recorrer la cadena y convertir los caracteres en bytes
  for (int i = 0; i < length; i++) {
    int index = startIndex + i; // Obtener el índice en el arreglo de destino
    mensaje[index] = byte(msg[i]); // Convertir el caracter en su representación binaria
  }

  String bitString;

  for (int i = 0; i < 8; ++i) {
    for (int j = 7; j >= 0; --j) {
      bitString += ((mensaje[i] >> j) & 1) ? '1' : '0';
    }
  }

  const char *Data = bitString.c_str();
  char* Result = MakeCRC(Data);
  uint8_t crc = strtol(Result, NULL, 2);

  /*Origen*/
  mensaje_completo[0] = origen[0];
  mensaje_completo[1] = origen[1];
  
  /*Destino*/
  mensaje_completo[2] = destino[0];
  mensaje_completo[3] = destino[1];

  /*CRC*/
  mensaje_completo[4] = ceros;
  mensaje_completo[5] = crc;

  /*Secuencia*/
  mensaje_completo[6] = secuencia;

  /*Total de paquetes*/
  mensaje_completo[7] = total;

  /*Mensaje*/
  mensaje_completo[8] = mensaje[0];
  mensaje_completo[9] = mensaje[1];
  mensaje_completo[10] = mensaje[2];
  mensaje_completo[11] = mensaje[3];
  mensaje_completo[12] = mensaje[4];
  mensaje_completo[13] = mensaje[5];
  mensaje_completo[14] = mensaje[6];
  mensaje_completo[15] = mensaje[7];

}

void loop(){
  secuencia = B00000000;
  for(secuencia; secuencia < int(total); secuencia++){

    vw_send(mensaje_completo, 16);
    vw_wait_tx();
    mensaje_completo[6] = secuencia;
    Serial.print(int(secuencia));
  }
  Serial.print("\n");
  delay(1000);
  // Serial.println("mensaje enviado");
}