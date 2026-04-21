// tests/host/test_support/FakeTcp1819.h v2
#pragma once

struct BBI2C {};

void fakeTcp1819Reset();
void fakeTcp1819SetBeginResult(bool result);
int fakeTcp1819BeginCallCount();
// tests/host/test_support/FakeTcp1819.h v2