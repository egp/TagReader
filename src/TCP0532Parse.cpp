// src/TCP0532Parse.cpp v2
#include "TCP0532Parse.h"

namespace {

bool hasStandardPreamble(const uint8_t* frame, size_t frameLen) {
  return frame != nullptr && frameLen >= 6 && frame[0] == 0x00 && frame[1] == 0x00 &&
         frame[2] == 0xFF;
}

}  // namespace

bool tcp0532IsAckFrame(const uint8_t* frame, size_t frameLen) {
  if (frame == nullptr || frameLen != 6) {
    return false;
  }

  return frame[0] == 0x00 && frame[1] == 0x00 && frame[2] == 0xFF && frame[3] == 0x00 &&
         frame[4] == 0xFF && frame[5] == 0x00;
}

bool tcp0532ParseResponseFrame(const uint8_t* frame,
                               size_t frameLen,
                               uint8_t* payload,
                               size_t payloadCapacity,
                               size_t* payloadLen) {
  if (payloadLen == nullptr) {
    return false;
  }
  *payloadLen = 0;

  if (!hasStandardPreamble(frame, frameLen)) {
    return false;
  }

  const uint8_t len = frame[3];
  const uint8_t lcs = frame[4];
  if (static_cast<uint8_t>(len + lcs) != 0x00) {
    return false;
  }

  const size_t totalFrameLen = static_cast<size_t>(len) + 7;
  if (frameLen != totalFrameLen) {
    return false;
  }

  if (frame[frameLen - 1] != 0x00) {
    return false;
  }

  if (len > 0 && payload == nullptr) {
    return false;
  }
  if (payloadCapacity < len) {
    return false;
  }

  uint8_t sum = 0;
  for (size_t i = 0; i < len; ++i) {
    const uint8_t value = frame[5 + i];
    payload[i] = value;
    sum = static_cast<uint8_t>(sum + value);
  }

  const uint8_t dcs = frame[5 + len];
  if (static_cast<uint8_t>(sum + dcs) != 0x00) {
    return false;
  }

  *payloadLen = len;
  return true;
}
// src/TCP0532Parse.cpp v2