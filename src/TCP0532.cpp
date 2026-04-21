// src/TCP0532.cpp v6
#include "TCP0532.h"

TCP0532::TCP0532()
    : bus_(nullptr),
      i2cAddress_(TCP0532_DEFAULT_I2C_ADDRESS),
      lastError_(nullptr),
      ready_(false) {}

bool TCP0532::begin(BBI2C& bus, uint8_t address) {
  bus_ = &bus;
  i2cAddress_ = address;
  ready_ = false;

#if defined(ARDUINO)
  const bool transportReady = true;
#else
  const bool transportReady = tcp0532HostBeginBus(bus);
#endif

  if (!transportReady) {
    setError("begin failed");
    return false;
  }

  lastError_ = nullptr;
  ready_ = true;
  return true;
}

bool TCP0532::configureI2C(uint8_t address) {
  ready_ = false;
  if (address == 0x00) {
    setError("invalid i2c address");
    return false;
  }

  i2cAddress_ = address;
  lastError_ = nullptr;
  return true;
}

bool TCP0532::isReady() const {
  return ready_;
}

const char* TCP0532::lastError() const {
  return (lastError_ != nullptr) ? lastError_ : "";
}

uint8_t TCP0532::i2cAddress() const {
  return i2cAddress_;
}

void TCP0532::setError(const char* message) {
  lastError_ = message;
  ready_ = false;
}
// src/TCP0532.cpp v6