// tests/host/test_WB_tcp0532_frame_decode.cpp v1
#include "TCP0532Parse.h"
#include "test_support/TestSupport.h"

#include <stddef.h>
#include <stdint.h>

int main() {
  {
    const uint8_t ack[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
    EXPECT_TRUE(tcp0532IsAckFrame(ack, sizeof(ack)));
  }

  {
    const uint8_t truncatedAck[] = {0x00, 0x00, 0xFF, 0x00, 0xFF};
    EXPECT_FALSE(tcp0532IsAckFrame(truncatedAck, sizeof(truncatedAck)));
  }

  {
    const uint8_t malformedAck[] = {0x00, 0x00, 0xFF, 0x00, 0xFE, 0x00};
    EXPECT_FALSE(tcp0532IsAckFrame(malformedAck, sizeof(malformedAck)));
  }

  {
    const uint8_t response[] = {
        0x00, 0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03, 0x32, 0x01, 0x06, 0x07, 0xE8, 0x00};
    uint8_t payload[6] = {};
    size_t payloadLen = 0;

    EXPECT_TRUE(
        tcp0532ParseResponseFrame(response, sizeof(response), payload, sizeof(payload), &payloadLen));
    EXPECT_EQ(static_cast<size_t>(6), payloadLen);

    const uint8_t expectedPayload[] = {0xD5, 0x03, 0x32, 0x01, 0x06, 0x07};
    for (size_t i = 0; i < payloadLen; ++i) {
      EXPECT_EQ(expectedPayload[i], payload[i]);
    }
  }

  {
    const uint8_t badChecksum[] = {
        0x00, 0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03, 0x32, 0x01, 0x06, 0x07, 0xE7, 0x00};
    uint8_t payload[6] = {};
    size_t payloadLen = 123;

    EXPECT_FALSE(
        tcp0532ParseResponseFrame(badChecksum, sizeof(badChecksum), payload, sizeof(payload), &payloadLen));
    EXPECT_EQ(static_cast<size_t>(0), payloadLen);
  }

  {
    const uint8_t response[] = {
        0x00, 0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03, 0x32, 0x01, 0x06, 0x07, 0xE8, 0x00};
    uint8_t payload[5] = {};
    size_t payloadLen = 999;

    EXPECT_FALSE(
        tcp0532ParseResponseFrame(response, sizeof(response), payload, sizeof(payload), &payloadLen));
    EXPECT_EQ(static_cast<size_t>(0), payloadLen);
  }

  return 0;
}
// tests/host/test_WB_tcp0532_frame_decode.cpp v1