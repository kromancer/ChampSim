#include "PrefetchAddressGenerator.h"

PrefetchAddressGenerator prefetch() {
  uint64_t base_address = 0;

  for (; base_address < 128; base_address += 64) {
    co_yield base_address;
  }

  co_return base_address;
}
