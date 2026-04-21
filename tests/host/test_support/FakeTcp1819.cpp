// tests/host/test_support/FakeTcp1819.cpp v5
#include "FakeTcp1819.h"

#include "TCP0532Transport.h"

#include <stddef.h>

namespace {
bool gBeginResult = true;
bool gWakeResult = true;
bool gWriteResult = true;
bool gReadResult = true;

int gBeginCallCount = 0;
int gWakeCallCount = 0;
int gWriteCallCount = 0;
int gReadCallCount = 0;

uint8_t gLastWakeAddress = 0x00;
uint8_t gLastWriteAddress = 0x00;
size_t gLastWriteLength = 0;

uint8_t gAckFrame[32] = {};
size_t gAckFrameLen = 0;

uint8_t gResponseFrame[64] = {};
size_t gResponseFrameLen = 0;
}  // namespace

void fakeTcp1819Reset() {
  gBeginResult = true;
  gWakeResult = true;
  gWriteResult = true;
  gReadResult = true;

  gBeginCallCount = 0;
  gWakeCallCount = 0;
  gWriteCallCount = 0;
  gReadCallCount = 0;

  gLastWakeAddress = 0x00;
  gLastWriteAddress = 0x00;
  gLastWriteLength = 0;

  gAckFrameLen = 0;
  gResponseFrameLen = 0;
}

void fakeTcp1819SetBeginResult(bool result) {
  gBeginResult = result;
}

void fakeTcp1819SetWakeResult(bool result) {
  gWakeResult = result;
}

void fakeTcp1819SetWriteResult(bool result) {
  gWriteResult = result;
}

void fakeTcp1819SetReadResult(bool result) {
  gReadResult = result;
}

void fakeTcp1819SetAckFrame(const uint8_t* data, size_t len) {
  gAckFrameLen = len;
  for (size_t i = 0; i < len; ++i) {
    gAckFrame[i] = data[i];
  }
}

void fakeTcp1819SetResponseFrame(const uint8_t* data, size_t len) {
  gResponseFrameLen = len;
  for (size_t i = 0; i < len; ++i) {
    gResponseFrame[i] = data[i];
  }
}

int fakeTcp1819BeginCallCount() {
  return gBeginCallCount;
}

int fakeTcp1819WakeCallCount() {
  return gWakeCallCount;
}

int fakeTcp1819WriteCallCount() {
  return gWriteCallCount;
}

int fakeTcp1819ReadCallCount() {
  return gReadCallCount;
}

uint8_t fakeTcp1819LastWakeAddress() {
  return gLastWakeAddress;
}

uint8_t fakeTcp1819LastWriteAddress() {
  return gLastWriteAddress;
}

size_t fakeTcp1819LastWriteLength() {
  return gLastWriteLength;
}

bool tcp0532TransportBeginBus(BBI2C& bus) {
  (void)bus;
  ++gBeginCallCount;
  return gBeginResult;
}

bool tcp0532TransportWakeDevice(BBI2C& bus, uint8_t address) {
  (void)bus;
  ++gWakeCallCount;
  gLastWakeAddress = address;
  return gWakeResult;
}

bool tcp0532TransportWrite(BBI2C& bus, uint8_t address, const uint8_t* data, size_t len) {
  (void)bus;
  (void)data;
  ++gWriteCallCount;
  gLastWriteAddress = address;
  gLastWriteLength = len;
  return gWriteResult;
}

bool tcp0532TransportRead(BBI2C& bus, uint8_t address, uint8_t* data, size_t len) {
  (void)bus;
  (void)address;
  ++gReadCallCount;

  if (!gReadResult || data == nullptr) {
    return false;
  }

  const uint8_t* source = nullptr;
  size_t sourceLen = 0;

  if (gReadCallCount == 1) {
    source = gAckFrame;
    sourceLen = gAckFrameLen;
  } else if (gReadCallCount == 2) {
    source = gResponseFrame;
    sourceLen = gResponseFrameLen;
  } else {
    return false;
  }

  if (sourceLen != len) {
    return false;
  }

  for (size_t i = 0; i < len; ++i) {
    data[i] = source[i];
  }
  return true;
}
// tests/host/test_support/FakeTcp1819.cpp v5