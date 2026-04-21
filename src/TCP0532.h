// src/TCP0532.h v3
#pragma once

#if defined(ARDUINO)
#include <Arduino.h>
#include <TCP1819.h>
#else
#include <stdint.h>
struct BBI2C;
bool tcp0532HostBeginBus(BBI2C& bus);
#endif

static constexpr uint8_t TCP0532_DEFAULT_I2C_ADDRESS = 0x24;

class TCP0532 {
 public:
  TCP0532();

  bool begin(BBI2C& bus, uint8_t address = TCP0532_DEFAULT_I2C_ADDRESS);
  bool configureI2C(uint8_t address = TCP0532_DEFAULT_I2C_ADDRESS);

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
// src/TCP0532.h v3