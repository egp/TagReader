<!-- README.md v2 -->
# TCP0532

TCP0532 is an Arduino library for experimenting with PN532-based NFC readers over the
TCP1819 bit-banged I2C library.

## Status

This library is currently aimed at a single target board:

- Arduino UNO R4 WiFi

The library now has a host-tested foundation for:

- begin(BBI2C&, uint8_t)
- configureI2C(uint8_t)
- wake()
- getFirmwareVersion(uint32_t&)
- PN532 host-frame encoding
- PN532 ACK / response parsing

Hardware validation with the actual PN532 module is still the next major milestone.

## Dependency

TCP0532 depends on:

- TCP1819

## Build and test

Host tests:

make clean test

Arduino compile check:

make clean compile-all

CI compiles the example for:

- arduino:renesas_uno:unor4wifi

and also runs the host tests.

## Example sketch layout

examples/ReaderAlive/ReaderAlive.ino intentionally uses:

#include "src/TCP0532.h"

This is deliberate.

The example is intended to compile while living in the examples/ReaderAlive/
directory with a sketch-local symlink:

examples/ReaderAlive/src -> ../../src

That keeps development simple before the first release and may remain the permanent
workflow for the example sketch.

## Current example

ReaderAlive is still a minimal bring-up sketch. It is intentionally conservative and
has not yet been rewritten into a hardware-focused demo.

## Repository layout

- src/ — library sources
- examples/ReaderAlive/ — example sketch
- tests/host/ — host-side BB/WB tests
- .github/workflows/ci.yml — CI

## Notes on testing

The current host tests use a local fake transport in tests/host/test_support/.

A likely future improvement is to move the reusable scripted bus mock into the TCP1819
repository so multiple TCP* libraries can share the same host-side bus test machinery.
<!-- README.md v2 -->