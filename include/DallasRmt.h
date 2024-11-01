#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "OnewireRmt.h"

// Model IDs
#define DS18S20MODEL 0x10  // also DS1820
#define DS18B20MODEL 0x28
#define DS1822MODEL 0x22
#define DS1825MODEL 0x3B
#define DS28EA00MODEL 0x42

// Error Codes
#define DEVICE_DISCONNECTED_C -127
#define DEVICE_DISCONNECTED_F -196.6
#define DEVICE_DISCONNECTED_RAW -7040

typedef uint8_t DeviceAddress[8];

class DallasRmt {
 public:
  DallasRmt();
  DallasRmt(OnewireRmt *ow);
  DallasRmt(uint8_t pin, uint8_t rmt_rx, uint8_t rmt_tx);

  ~DallasRmt();

  void setOneWire(OnewireRmt *ow);

  /*
   * Initialises the bus
   */
  void begin(void);

  /*
   *  Returns the number of devices found on the bus
   */
  uint8_t getDeviceCount(void) {
    return _devices;
  }

  /*
   *  Returns true if address is valid
   */
  bool validAddress(const uint8_t *);

  /*
   * Returns true if address is of the family of sensors the lib supports.
   */
  // bool validFamily(const uint8_t *deviceAddress);

  /*
   * Finds an address at a given index on the bus
   */
  bool getAddress(uint8_t *deviceAddress, uint8_t index);

  /*
   * Attempts to determine if the device at the given address is connected to
   * the bus
   */
  bool isConnected(const uint8_t *);

  /*
   * Attempts to determine if the device at the given address is connected to
   * the bus.
   * Also allows for updating the read scratchpad
   */
  bool isConnected(const uint8_t *, uint8_t *);

  /*
   * Reads device's scratchpad
   */
  bool readScratchPad(const uint8_t *, uint8_t *);

  /*
   * Writes device's scratchpad
   */
  void writeScratchPad(const uint8_t *, const uint8_t *);

  /*
   * Reads device's power requirements
   */
  bool readPowerSupply(const uint8_t *deviceAddress);

  /*
   * Gets the global resolution
   */
  uint8_t getResolution() {
    return _bitResolution;
  }
  /*
   * Returns the device's resolution: 9, 10, 11, or 12 bits
   */
  uint8_t getResolution(const uint8_t *deviceAddress);

  /*
   * Sets the global resolution to 9, 10, 11, or 12 bits
   */
  void setResolution(uint8_t);

  /*
   * Sets the resolution of a device to 9, 10, 11, or 12 bits
   */
  bool setResolution(const uint8_t *, uint8_t,
                     bool skipGlobalBitResolutionCalculation = false);

  /*
   * Sets/gets the value of the waitForConversion flag
   * true : function requestTemperature() etc returns when conversion is ready
   * false: function requestTemperature() etc returns immediately (USE WITH
   * CARE!!)
   *        (1) programmer has to check if the needed delay has passed
   *        (2) but the application can do meaningful things in that time
   */
  void setWaitForConversion(bool value) {
    _waitForConversion = value;
  }

  bool getWaitForConversion(void) {
    return _waitForConversion;
  }

  /*
   * Sets/gets the checkForConversion flag
   * sets the value of the checkForConversion flag
   * true : function requestTemperature() etc will 'listen' to an IC to
   * determine whether a conversion is complete
   * false: function requestTemperature() etc will wait a set time (worst case
   * scenario) for a conversion to complete
   */
  void setCheckForConversion(bool value) {
    _checkForConversion = value;
  }

  bool getCheckForConversion(void) {
    return _checkForConversion;
  }

  /*
   * Sends command for all devices on the bus to perform a temperature
   * conversion
   */
  void requestTemperatures(void);
  // sends command for one device to perform a temperature conversion by address
  bool requestTemperaturesByAddress(const uint8_t *);

  /*
   * Sends command for one device to perform a temperature conversion by index
   */
  bool requestTemperaturesByIndex(uint8_t);

  /*
   * Returns temperature raw value (12 bit integer of 1/128 degrees C)
   */
  int16_t getTemp(const uint8_t *);

  /*
   * Returns temperature in degrees C
   */
  float getTempC(const uint8_t *);

  /*
   * Returns temperature in degrees F
   */
  float getTempF(const uint8_t *);

  /*
   * Get temperature for device index (slow)
   */
  float getTempCByIndex(uint8_t);

  /*
   * Get temperature for device index (slow)
   */
  float getTempFByIndex(uint8_t);

  /*
   * Returns true if the bus requires parasite power
   */
  bool isParasitePowerMode(void) {
    return _parasite;
  }

  /*
   * Is a conversion complete on the wire?
   */
  bool isConversionComplete(void);

  int16_t millisToWaitForConversion(uint8_t);

  /*
   * Convert from Celsius to Fahrenheit
   */
  static float toFahrenheit(float);

  /*
   * Convert from Fahrenheit to Celsius
   */
  static float toCelsius(float);

  /*
   * Convert from raw to Celsius
   */
  static float rawToCelsius(int16_t);

  /*
   * Convert from raw to Fahrenheit
   */
  static float rawToFahrenheit(int16_t);

 private:
  /*
   * The OneWire object
   */
  OnewireRmt *_ow;

  /*
   * count of devices on the bus
   */
  uint8_t _devices;
  typedef uint8_t ScratchPad[9];

  /*
   * Parasite power on or off
   */
  bool _parasite;

  /*
   * Used to determine the delay amount needed to allow for the
   * temperature conversion to take place
   */
  uint8_t _bitResolution;

  bool _waitForConversion;
  bool _checkForConversion;

  /*
   * Set to true if we created _ow
   */
  bool _ownOnewire;

  /*
   * Reads scratchpad and returns the raw temperature
   */
  int16_t calculateTemperature(const uint8_t *, uint8_t *);

  void blockTillConversionComplete(uint8_t);
};
