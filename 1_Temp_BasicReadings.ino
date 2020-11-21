/******************************************************************************
  Example1_BasicReadings.ino
  Example for the TMP117 I2C Temperature Sensor
  Madison Chodikov @ SparkFun Electronics
  May 29 2019
  ~
  This sketch configures the TMP117 temperature sensor and prints the
  temperature in degrees celsius and fahrenheit with a 500ms delay for
  easier readings. 
  Resources:
  Wire.h (included with Arduino IDE)
  SparkFunTMP117.h (included in the src folder) http://librarymanager/All#SparkFun_TMP117
  Development environment specifics:
  Arduino 1.8.9+
  Hardware Version 1.0.0
  This code is beerware; if you see me (or any other SparkFun employee) at
  the local, and you've found our code helpful, please buy us a round!
  Distributed as-is; no warranty is given.
******************************************************************************/

/*
  NOTE: For the most accurate readings:
  - Avoid heavy bypass traffic on the I2C bus
  - Use the highest available communication speeds
  - Use the minimal supply voltage acceptable for the system
  - Place device horizontally and out of any airflow when storing
  For more information on reaching the most accurate readings from the sensor,
  reference the "Precise Temperature Measurements with TMP116" datasheet that is
  linked on Page 35 of the TMP117's datasheet
*/

#include <Wire.h>            // Used to establish serial communication on the I2C bus
#include <SparkFun_TMP117.h> // Used to send and recieve specific information from our sensor
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

// The default address of the device is 0x48 = (GND)
TMP117 sensor; // Initalize sensor
static uint8_t payload[5];

void setup()
{
  SerialUSB.begin(115200);    // Start serial communication at 115200 baud
  while(!SerialUSB); 
  Wire.begin();
  Wire.setClock(400000);   // Set clock speed to be the fastest for better communication (fast mode)
  SerialUSB.println("TMP117 Example 1: Basic Readings");
  if (sensor.begin() == true) // Function to check if the sensor will correctly self-identify with the proper Device ID/Address
  {
    SerialUSB.println("Begin");
  }
  else
  {
    SerialUSB.println("Device failed to setup- Freezing code.");
    while (1); // Runs forever
  }
}

void loop()
{
  // Data Ready is a flag for the conversion modes - in continous conversion the dataReady flag should always be high
  if (sensor.dataReady() == true) // Function to make sure that there is data ready to be printed, only prints temperature values when data is ready
  {
    float tempC = sensor.readTempC();
    // Print temperature in °C
    SerialUSB.println(); // Create a white space for easier viewing
    SerialUSB.print("Temperature in Celsius: ");
    SerialUSB.println(tempC);
    // adjust for the f2sflt16 range (-1 to 1)
    // tempC = tempC / 100;
    // float -> int
    // note: this uses the sflt16 datum (https://github.com/mcci-catena/arduino-lmic#sflt16)
    //uint16_t payloadTemp = LMIC_f2sflt16(tempC);
    int16_t payloadTemp = round(tempC * 100);
    // int -> bytes
    byte tempHigh = highByte(payloadTemp);
    byte tempLow = lowByte(payloadTemp);
    // place the bytes into the payload
    payload[0] = tempHigh;
    payload[1] = tempLow;
    SerialUSB.println(payload[0]);
    SerialUSB.println(payload[1]);
    delay(500); // Delay added for easier readings
  }
}
