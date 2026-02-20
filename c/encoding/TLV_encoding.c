#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { UINT8_T, UINT16_T } Datatype;

typedef struct {
  uint8_t id;
  size_t size;
  Datatype type;
} Descrittore;
typedef struct {
  uint8_t id;
  uint16_t value;

} mydata;

int serializzazione(uint8_t *buffer, mydata *data) {
  int offset = 0;
  buffer[offset++] = 0x01;
  buffer[offset++] = 0x00;
  buffer[offset++] = 0x01;
  buffer[offset++] = data->id;
  buffer[offset++] = 0x02;
  buffer[offset++] = 0x00;
  buffer[offset++] = 0x02;
  uint16_t temp = htons(data->value);
  memcpy(&buffer[offset], &temp, 2);
  offset += 2;
  return offset;
}
void deserializzazione(uint8_t *buffer, int buffer_len) {
  int i = 0;
  while (i < buffer_len) {
    uint8_t tag = buffer[i++];
    uint16_t len = (buffer[i] << 8) | buffer[i + 1];
    i += 2;
    printf("trovato tag 0x%02X(lungi %d byte)", tag, len);
    if (tag == 0x01) {
      printf("ID = %d\n", buffer[i]);

    } else if (tag == 0x02) {
      uint16_t val;
      memcpy(&val, &buffer[i], 2);
      printf("temp = %d c\n", ntohs(val));
    } else {
      printf("tag sconosciuto\n");
    }
    i += len;
  }
}

int main(int argc, char *argv[]) {
  uint8_t pacchetto[100];
  mydata mystruct = {34, 3};
  int dimensione = serializzazione(pacchetto, &mystruct);
  printf("Byte spediti: ");
  for (int i = 0; i < dimensione; i++)
    printf("%02X ", pacchetto[i]);
  printf("\n\n");

  deserializzazione(pacchetto, dimensione);
  return EXIT_SUCCESS;
}
