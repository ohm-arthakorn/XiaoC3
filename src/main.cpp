#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>

unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

HardwareSerial pmsSerial(0); // Use UART0 for PMS5300 communication

void setup()
{
  // Begin Serial Monitor
  Serial.begin(115200);

  // Initialize PMS5300 serial communication
  pmsSerial.begin(9600, SERIAL_8N1, 20, 21); // PMS5300 typically communicates at 9600 baud
                                             // GPIO4 as RX, GPIO5 as TX
}

void loop()
{
  int index = 0;
  char value;
  char previousValue;

  while (pmsSerial.available())
  {
    value = pmsSerial.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d))
    {
      Serial.println("Cannot find the data header.");
      break;
    }

    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14)
    {
      previousValue = value;
    }
    else if (index == 5)
    {
      pm1 = 256 * previousValue + value;
      Serial.print("{ ");
      Serial.print("\"pm1\": ");
      Serial.print(pm1);
      Serial.print(" ug/m3");
      Serial.print(", ");
    }
    else if (index == 7)
    {
      pm2_5 = 256 * previousValue + value;
      Serial.print("\"pm2_5\": ");
      Serial.print(pm2_5);
      Serial.print(" ug/m3");
      Serial.print(", ");
    }
    else if (index == 9)
    {
      pm10 = 256 * previousValue + value;
      Serial.print("\"pm10\": ");
      Serial.print(pm10);
      Serial.print(" ug/m3");
    }
    else if (index > 15)
    {
      break;
    }
    index++;
  }

  // if (pmsSerial.available()) {
  //   byte buffer[32];  // Adjust buffer size based on data sheet requirements

  //   // Read available bytes from PMS5300
  //   int length = pmsSerial.readBytes(buffer, 32);

  //   // Process the data (simplified example)
  //   Serial.print("Received data: ");
  //   for (int i = 0; i < length; i++) {
  //     Serial.print(buffer[i], HEX);
  //     Serial.print(" ");
  //   }
  //   Serial.println();

  //   delay(1000);  // Adjust delay as needed
  // }
}
