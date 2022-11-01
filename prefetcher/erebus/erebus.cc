#include "erebus.h"

#include <cassert>
#include <cstdint>
#include <iostream>

#include "PrefetchAddressGenerator.h"
#include "cache.h"

class Erebus
{
private:
  PrefetchAddressGenerator& gen_;

public:
  Erebus(PrefetchAddressGenerator& gen) : gen_{gen} {}
  bool is_done() { return gen_.is_done(); }
  uint64_t get_next_addr() { return gen_(); }
};

static Erebus* erebus = nullptr;

void initErebus(PrefetchAddressGenerator& gen)
{
  assert(erebus == nullptr);
  erebus = new Erebus{gen};
}

void CACHE::prefetcher_initialize()
{
  assert(erebus != nullptr);
}

/** @brief Called as the last step of `CACHE::operate()`
 */
void CACHE::prefetcher_cycle_operate()
{
  if (!erebus->is_done()) {
    uint64_t addr = erebus->get_next_addr();
    prefetch_line(addr, true, 0);
  }
}

/** @brief When is this called?
 */
uint32_t CACHE::prefetcher_cache_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type, uint32_t metadata_in)
{
  return metadata_in;
}

/** @brief When is this called?
 */
uint32_t CACHE::prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr, uint32_t metadata_in)
{
  return metadata_in;
}

void CACHE::prefetcher_final_stats()
{
  cout << "Erebus: requested: " << pf_requested << " issued: " << pf_issued << " useful: " << pf_useful << " useless: " << pf_useless << endl;
}
