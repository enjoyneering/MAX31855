/***************************************************************************************************/
/*
   This is an Arduino library for 14-bit MAX31855 K-Thermocouple to Digital Converter
   with 12-bit Cold Junction Compensation. Can work with hardware & bitbang 5Mhz SPI & sampling
   rate ~9..10Hz.

   - K-type thermocouples have an absolute accuracy of around ±2°C..±6°C.
   - Measurement tempereture range -200°C..+700°C ±2°C or -270°C..+1372°C ±6°C
     with 0.25°C resolution/increment.
   - Cold junction compensation range -40°C..+125° ±3°C with 0.062°C resolution/increment.
     Optimal performance of cold junction compensation happends when the thermocouple cold junction
     & the MAX31855 are at the same temperature. Avoid placing heat-generating devices or components
     near the converter because this may produce an errors.
   - It is strongly recommended to add a 10nF/0.01mF ceramic surface-mount capacitor, placed across
     the T+ and T- pins, to filter noise on the thermocouple lines.
     
   written by : enjoyneering79
   sourse code: https://github.com/enjoyneering/MAX31855

   This sensor can work with hardware SPI bus, specials pins are required to interface

   Connect chip to pins:    SCLK        MISO        don't use SS for CS   don't use MOSI for CS
   Uno, Mini, Pro:          13          12          10                    11
   Mega2560, Due:           52          50          53                    51
   Leonardo, ProMicro:      15          14          x                     16
   NodeMCU 1.0:             GPIO14/D5   GPIO12/D6   GPIO15/D8*            GPIO13/D7
   WeMos D1 Mini:           GPIO14/D5   GPIO12/D6   GPIO15/D8*            GPIO13/D7

   *ESP8266, for CS use GPIO2/D4 or GPIO0/D3 & apply an external 25kOhm pullup/down resistor.

   GNU GPL license, all text above must be included in any redistribution, see link below for details:
   - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/

#include <MAX31855.h>


/**************************************************************************/
/*
    MAX31855()

    Constructor for software read only SPI serial interface

    NOTE:
    cs  - chip select, set CS low to enable the serial interface
    so  - serial data output
    sck - serial clock input
*/
/**************************************************************************/
MAX31855::MAX31855(uint8_t cs, uint8_t so, uint8_t sck)
{
  _useHardwareSPI = false; //false -> sw spi

  _cs  = cs;               //sw ss
  _so  = so;               //sw miso
  _sck = sck;              //sw sclk
}

/**************************************************************************/
/*
    MAX31855()

    Constructor for hardware read only SPI serial interface

    NOTE:
    - chip select "cs", set CS low to enable the serial interface
*/
/**************************************************************************/
MAX31855::MAX31855(uint8_t cs)
{
  _useHardwareSPI = true; //true -> hw spi

  _cs  = cs;              //hw ss
  _so  = 0;               //sw miso
  _sck = 0;               //sw sclk
}

/**************************************************************************/
/*
    begin()

    Initializes & configures I2C
*/
/**************************************************************************/
void MAX31855::begin(void)
{
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);                  //disables the spi interface for MAX31855, but it will initiate measurement/conversion

  switch (_useHardwareSPI)                  //true -> hw spi, false ->sw spi
  {
    case true:
      SPI.begin();                          //setting hardware SCK, MOSI, SS to output, pull SCK, MOSI low & SS high      
      break;

    case false:
      pinMode(_so, INPUT);
      pinMode(_sck, OUTPUT);
      digitalWrite(_sck, LOW);
      break;
  }

  delay(MAX31855_CONVERSION_POWER_UP_TIME);
}

/**************************************************************************/
/*
    detectThermocouple()

    Checks if thermocouple is open, shorted to GND, shorted to VCC

    Return:
    - 0 OK
    - 1 short to VCC
    - 2 short to GND
    - 3 not connected

    NOTE:
    - bit D16 is normally low & goes high if thermocouple is open, shorted to GND or VCC
    - bit D2  is normally low & goes high to indicate a hermocouple short to VCC
    - bit D1  is normally low & goes high to indicate a thermocouple short to GND
    - bit D0  is normally low & goes high to indicate a thermocouple open circuit
*/
/**************************************************************************/
uint8_t MAX31855::detectThermocouple(int32_t rawValue)
{
  if (rawValue == MAX31855_FORCE_READ_DATA) rawValue = readRawData();

  if (bitRead(rawValue, 16) == 1)
  {
    if      (bitRead(rawValue, 2) == 1) return MAX31855_THERMOCOUPLE_SHORT_TO_VCC;
    else if (bitRead(rawValue, 1) == 1) return MAX31855_THERMOCOUPLE_SHORT_TO_GND;
    else if (bitRead(rawValue, 0) == 1) return MAX31855_THERMOCOUPLE_NOT_CONNECTED;
    else                                return MAX31855_THERMOCOUPLE_UNKNOWN;
  }
  return MAX31855_THERMOCOUPLE_OK;
}

/**************************************************************************/
/*
    getChipID()

    Checks chip ID

    NOTE:
    - bit D17, D3 always return zero & can be used as device ID
*/
/**************************************************************************/
uint16_t MAX31855::getChipID(int32_t rawValue)
{
  if (rawValue == MAX31855_FORCE_READ_DATA) rawValue = readRawData();

  if (bitRead(rawValue, 17) == 0 && bitRead(rawValue, 3) == 0) return MAX31855_ID;

  return 0;
}

/**************************************************************************/
/*
    getTemperature()

    Reads Temperature, C

    NOTE:
    - range -200°C..+700°C ±2°C or -270°C..+1372°C ±6°C with 0.25°C
      resolution/increment
    - thermocouple temperature data is 14-bit long
    - bit D31 is the thermocouple temperature sign bit "+" is high & "-" is low,
      if T+ and T- are unconnected it goes low
    - bits D30..D18 contain the converted temperature in the order of MSB to LSB,
      if T+ and T- are unconnected they go high
    - it is strongly recommended to add a 10nF/0.01mF ceramic surface-mount
      capacitor, placed across the T+ and T- pins, to filter noise on the
      thermocouple lines
*/
/**************************************************************************/
float MAX31855::getTemperature(int32_t rawValue)
{
  if (rawValue == MAX31855_FORCE_READ_DATA) rawValue = readRawData();

  if (detectThermocouple(rawValue) != MAX31855_THERMOCOUPLE_OK || getChipID(rawValue) != MAX31855_ID) return MAX31855_ERROR;

  rawValue = rawValue >> 18; //clear D17..D0 bits

  return (float)rawValue * MAX31855_THERMOCOUPLE_RESOLUTION;
}

/**************************************************************************/
/*
    getColdJunctionTemperature()

    Reads Temperature, C

    NOTE:
    - range -40°C..+125° ±3°C with 0.062°C resolution/increment
    - chip internal temperature data is 12-bit long
    - bit D15 is cold-junction temperature sign bit "+" is high & "-" is low
    - bits D14..D4 contain cold-junction temperature in the order of MSB to LSB
*/
/**************************************************************************/
float MAX31855::getColdJunctionTemperature(int32_t rawValue)
{
  if (rawValue == MAX31855_FORCE_READ_DATA) rawValue = readRawData();

  if (getChipID(rawValue) != MAX31855_ID) return MAX31855_ERROR;

  rawValue = rawValue & 0x0000FFFF;                                   //clear D31..D16 bits
  rawValue = rawValue >> 4;                                           //clear D3...D0  bits

  return (float)rawValue * MAX31855_COLD_JUNCTION_RESOLUTION;
}

/**************************************************************************/
/*
    readRawData()

    Reads raw data from MAX31855, bit-banging implementation

    NOTE:
    - read of the cold-junction compensated thermocouple temperature requires
      14 clock cycles
    - read of the cold-junction compensated thermocouple temperature & reference
      junction temperatures requires 32 clock cycles
    - forcing CS low immediately stops any conversion process, force CS high
      to initiate a new measurement process
    - set CS low to enable the serial interface & force to output the first bit on the SO pin,
      apply 14/32 clock signals at SCK to read the results at SO on the falling edge of the SCK
    - bit D31 is the thermocouple temperature sign bit "+" is high & "-" is low,
      if T+ & T- pins are unconnected it goes low
    - bits D30..D18 contain the converted temperature in the order of MSB to LSB,
      if T+ & T- pins are unconnected they go high
    - bit D17 is low to provide a device ID for the MAX31855
    - bit D16 is normally low & goes high if thermocouple is open, shorted to GND or VCC
    - bit D15 is cold-junction temperature sign bit "+" is high & "-" is low
    - bits D14..D4 contain cold-junction temperature in the order of MSB to LSB
    - bit D3 is is low to provide a device ID for the MAX31855
    - bit D2 is normally low & goes high to indicate a hermocouple short to VCC
    - bit D1 is normally low & goes high to indicate a thermocouple short to GND
    - bit D0 is normally low & goes high to indicate a thermocouple open circuit

    - max SPI master clock speed is equal with board speed
      (16000000UL for 5V 16MHz/ProMini), but MAX31855 max speed is only 5MHz
    - SPI_MODE0 -> capture data on clock's falling edge
*/
/**************************************************************************/
int32_t MAX31855::readRawData(void)
{
  int32_t rawData = 0;

  digitalWrite(_cs, LOW);                                                //stop  measurement/conversion
  delay(1);                                                              //4MHz  is 0.25usec, do we need it???
  digitalWrite(_cs, HIGH);                                               //start measurement/conversion
  delay(MAX31855_CONVERSION_TIME);

  digitalWrite(_cs, LOW);                                                //set CS low to enable spi interface for MAX31855

  switch (_useHardwareSPI)                                               //true -> hw spi, false -> sw spi
  {
    case true:
      SPI.beginTransaction(SPISettings(5000000UL, MSBFIRST, SPI_MODE0)); //speed ~5MHz, read msb first, spi mode 0, see note
      for (uint8_t i = 0; i < 2; i++)                                    //read 32-bits via hardware spi, in order MSB->LSB (D31..D0 bit)
      {
        rawData = (rawData << 16) | SPI.transfer16(0x0000);              //chip has read only spi & mosi not connected, doesn't metter what to send
      }
      break;

    case false:
      for (uint8_t i = 0; i < 32; i++)                                   //read 32-bits via software spi, in order MSB->LSB (D31..D0 bit)
      {
        digitalWrite(_sck, HIGH);
        rawData = (rawData << 1) | digitalRead(_so);
        digitalWrite(_sck, LOW);
      }
      break;
  }  

  digitalWrite(_cs, HIGH);                                               //disables spi interface, but it will initiate measurement/conversion

  if (_useHardwareSPI == true) SPI.endTransaction();                     //de-asserting hw chip select & free hw spi for other slaves

  return rawData;
}
