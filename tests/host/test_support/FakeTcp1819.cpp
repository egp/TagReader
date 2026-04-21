// tests/host/test_support/FakeTcp1819.cpp v1
#include "FakeTcp1819.h"

namespace {
bool gBeginResult = true;
int gBeginCallCount = 0;
}

void fakeTcp1819Reset() {
  gBeginResult = true;
  gBeginCallCount = 0;
}

void fakeTcp1819SetBeginResult(bool result) {
  gBeginResult = result;
}

int fakeTcp1819BeginCallCount() {
  return gBeginCallCount;
}

bool tcp0532HostBeginBus(BBI2C& bus) {
  (void)bus;
  ++gBeginCallCount;
  return gBeginResult;
}
// tests/host/test_support/FakeTcp1819.cpp v1