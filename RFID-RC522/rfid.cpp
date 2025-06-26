//Test of cheap 13.56 Mhz RFID-RC522 module from eBay
//This code is based on Martin Olejar's MFRC522 library. Minimal changes
//Adapted for Nucleo STM32 F401RE. Should work on other Nucleos too

//Connect as follows:
//RFID pins        ->  Nucleo header CN5 (Arduino-compatible header)
//----------------------------------------
//RFID IRQ=pin5    ->   Not used. Leave open
//RFID MISO=pin4   ->   Nucleo SPI_MISO=PA_6=D12
//RFID MOSI=pin3   ->   Nucleo SPI_MOSI=PA_7=D11
//RFID SCK=pin2    ->   Nucleo SPI_SCK =PA_5=D13
//RFID SDA=pin1    ->   Nucleo SPI_CS  =PB_6=D10
//RFID RST=pin7    ->   Nucleo         =PA_9=D8
//3.3V and Gnd to the respective pins                              
                              
#include "mbed.h"
#include "MFRC522.h"
#include "rfid.h"
#include "my_mqtt.h"
#include "relay.h"

// Nucleo Pin for MFRC522 reset (pick another D pin if you need D8)
#define MF_RESET    D8

DigitalOut LedGreen(LED1);

//Serial connection to PC for output
Serial pc(SERIAL_TX, SERIAL_RX);


MFRC522    RfChip   (SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS, D9);

void process_rfid() {
  ThisThread::sleep_for(200);
  pc.printf("starting...\n");
  pc.baud(9600);
  // Init. RC522 Chip
  RfChip.PCD_Init();

  while (true) {
    LedGreen = 1;

    // Look for new cards
    if ( ! RfChip.PICC_IsNewCardPresent())
    {
      ThisThread::sleep_for(500);
      continue;
    }

    // Select one of the cards
    if ( ! RfChip.PICC_ReadCardSerial())
    {
      ThisThread::sleep_for(500);
      continue;
    }

    LedGreen = 0;

   // Print UID
        pc.printf("Card UID: ");
        char card_uid[256];
        int card_p = 0;

        for (uint8_t i = 0; i < RfChip.uid.size; i++) {
            card_p += sprintf(card_uid + card_p, "%X", RfChip.uid.uidByte[i]);
        }
        pc.printf("%s", card_uid);
        pc.printf("\r\n");
        mqtt_send(card_uid, strlen(card_uid), "rfid");
        toggle_relay();

        // Print PICC type
        uint8_t piccType = RfChip.PICC_GetType(RfChip.uid.sak);
        const char* typeName = RfChip.PICC_GetTypeName(piccType);
        pc.printf("PICC Type: %s\r\n", typeName);
    ThisThread::sleep_for(1000);
  }
}