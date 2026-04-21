// src/TCP0532Parse.h v1
#pragma once

#include <stddef.h>
#include <stdint.h>

bool tcp0532IsAckFrame(const uint8_t* frame, size_t frameLen);

bool tcp0532ParseResponseFrame(const uint8_t* frame,
                               size_t frameLen,
                               uint8_t* payload,
                               size_t payloadCapacity,
                               size_t* payloadLen);
// src/TCP0532Parse.h v1