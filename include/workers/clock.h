#pragma once

#include <chrono>

using ll = long long;

class clock {
  public:
    clock();
    double get_upload_rate(ll byte_uploaded);

  private:
    ll byte_window;
    std::chrono::time_point<std::chrono::high_resolution_clock> curr;
};
