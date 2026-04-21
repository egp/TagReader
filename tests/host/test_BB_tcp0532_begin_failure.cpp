// tests/host/test_BB_tcp0532_begin_failure.cpp v1
#include "TCP0532.h"
#include "test_support/FakeTcp1819.h"
#include "test_support/TestSupport.h"

int main() {
  TCP0532 dut;
  BBI2C bus{};

  fakeTcp1819Reset();
  fakeTcp1819SetBeginResult(false);

  EXPECT_FALSE(dut.begin(bus, 0x28));
  EXPECT_FALSE(dut.isReady());
  EXPECT_STREQ("begin failed", dut.lastError());
  EXPECT_EQ(static_cast<uint8_t>(0x28), dut.i2cAddress());
  EXPECT_EQ(1, fakeTcp1819BeginCallCount());

  return 0;
}
// tests/host/test_BB_tcp0532_begin_failure.cpp v1