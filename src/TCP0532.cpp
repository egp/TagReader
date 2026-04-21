// src/TCP0532.cpp v11
#include "TCP0532.h"

#include "TCP0532Frame.h"
#include "TCP0532Parse.h"
#include "TCP0532Transport.h"

TCP0532::TCP0532()
    : bus_(nullptr),
      i2cAddress_(TCP0532_DEFAULT_I2C_ADDRESS),
      lastError_(nullptr),
      ready_(false) {}

bool TCP0532::begin(BBI2C& bus, uint8_t address) {
  bus_ = &bus;
  i2cAddress_ = address;
  ready_ = false;

  const bool transportReady = tcp0532TransportBeginBus(bus);
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

bool TCP0532::wake() {
  if (bus_ == nullptr) {
    setError("not begun");
    return false;
  }

  const bool wakeOk = tcp0532TransportWakeDevice(*bus_, i2cAddress_);
  if (!wakeOk) {
    setError("wake failed");
    return false;
  }

  lastError_ = nullptr;
  ready_ = true;
  return true;
}

bool TCP0532::getFirmwareVersion(uint32_t& version) {
  version = 0;

  if (bus_ == nullptr) {
    setError("not begun");
    return false;
  }

  const uint8_t commandPayload[] = {0x02};
  uint8_t commandFrame[9] = {};
  size_t commandFrameLen = 0;
  if (!tcp0532BuildHostFrame(commandPayload, sizeof(commandPayload), commandFrame,
                             sizeof(commandFrame), &commandFrameLen)) {
    setError("frame build failed");
    return false;
  }

  if (!tcp0532TransportWrite(*bus_, i2cAddress_, commandFrame, commandFrameLen)) {
    setError("write failed");
    return false;
  }

  uint8_t ackFrame[6] = {};
  if (!tcp0532TransportRead(*bus_, i2cAddress_, ackFrame, sizeof(ackFrame)) ||
      !tcp0532IsAckFrame(ackFrame, sizeof(ackFrame))) {
    setError("ack failed");
    return false;
  }

  uint8_t responseFrame[13] = {};
  if (!tcp0532TransportRead(*bus_, i2cAddress_, responseFrame, sizeof(responseFrame))) {
    setError("response failed");
    return false;
  }

  uint8_t payload[6] = {};
  size_t payloadLen = 0;
  if (!tcp0532ParseResponseFrame(responseFrame, sizeof(responseFrame), payload, sizeof(payload),
                                 &payloadLen)) {
    setError("response failed");
    return false;
  }

  if (payloadLen != 6 || payload[0] != 0xD5 || payload[1] != 0x03) {
    setError("response failed");
    return false;
  }

  version = (static_cast<uint32_t>(payload[2]) << 24) |
            (static_cast<uint32_t>(payload[3]) << 16) |
            (static_cast<uint32_t>(payload[4]) << 8) |
            static_cast<uint32_t>(payload[5]);

  lastError_ = nullptr;
  ready_ = true;
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
// src/TCP0532.cpp v11