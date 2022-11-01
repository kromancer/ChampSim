#ifndef PREFETCHADDRESSGENERATOR_H
#define PREFETCHADDRESSGENERATOR_H

#include <coroutine>
#include <cstdint>

struct PrefetchAddressGenerator {
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type {
    uint64_t value_;

    PrefetchAddressGenerator get_return_object() {
      return PrefetchAddressGenerator(handle_type::from_promise(*this));
    }

    void unhandled_exception() {}

    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }

    std::suspend_always yield_value(uint64_t value) {
      value_ = value;
      return {};
    }

    void return_void() {}
  };

  handle_type h_;

  PrefetchAddressGenerator(handle_type h) : h_(h) {}
  ~PrefetchAddressGenerator() { h_.destroy(); }

  bool is_done() {
    return h_.done();
  }

  uint64_t operator()() {
    h_();
    return h_.promise().value_;
  }
};

#endif // PREFETCHADDRESSGENERATOR_H
