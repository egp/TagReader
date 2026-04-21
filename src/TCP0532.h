// src/TCP0532.h v6
#pragma once

#if defined(ARDUINO)
#include <Arduino.h>
#include <TCP1819.h>
#else
#include <stdint.h>
struct BBI2C;
#endif

static constexpr uint8_t TCP0532_DEFAULT_I2C_ADDRESS = 0x24;

class TCP0532 {
 public:
  TCP0532();

  bool begin(BBI2C& bus, uint8_t address = TCP0532_DEFAULT_I2C_ADDRESS);
  bool configureI2C(uint8_t address = TCP0532_DEFAULT_I2C_ADDRESS);
  bool wake();
  bool getFirmwareVersion(uint32_t& version);

  bool isReady() const;
  const char* lastError() const;
  uint8_t i2cAddress() const;

 private:
  void setError(const char* message);

  BBI2C* bus_;
  uint8_t i2cAddress_;
  const char* lastError_;
  bool ready_;
};
// src/TCP0532.h v6