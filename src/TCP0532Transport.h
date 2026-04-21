// src/TCP0532Transport.h v1
#pragma once

#include <stddef.h>
#include <stdint.h>

#if defined(ARDUINO)
#include <TCP1819.h>

inline bool tcp0532TransportBeginBus(BBI2C& bus) {
  (void)bus;
  return true;
}

inline bool tcp0532TransportWakeDevice(BBI2C& bus, uint8_t address) {
  (void)bus;
  (void)address;
  return true;
}

inline bool tcp0532TransportWrite(BBI2C& bus,
                                  uint8_t address,
                                  const uint8_t* data,
                                  size_t len) {
  (void)bus;
  (void)address;
  (void)data;
  (void)len;
  return true;
}

inline bool tcp0532TransportRead(BBI2C& bus, uint8_t address, uint8_t* data, size_t len) {
  (void)bus;
  (void)address;
  (void)data;
  (void)len;
  return true;
}

#else

struct BBI2C;

bool tcp0532TransportBeginBus(BBI2C& bus);
bool tcp0532TransportWakeDevice(BBI2C& bus, uint8_t address);
bool tcp0532TransportWrite(BBI2C& bus, uint8_t address, const uint8_t* data, size_t len);
bool tcp0532TransportRead(BBI2C& bus, uint8_t address, uint8_t* data, size_t len);

#endif
// src/TCP0532Transport.h v1