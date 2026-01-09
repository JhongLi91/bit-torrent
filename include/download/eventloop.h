#pragma once

#include <cstdint>
#include <sys/event.h>
#include <vector>

const uint32_t MAX_EVENT_POLL = 50;

class eventloop {
  public:
    eventloop();
    std::vector<int> poll_events();

    void register_fd(int fd);
    void remove_fd(int fd);

  private:
    int kq;
};
