#pragma once

#include <sys/event.h>
#include <vector>

class event_loop {
  public:
    std::vector<int> poll_events();

    void register_fd(int fd);
    void remove_fd(int fd);

  private:
    int kq;
    // TODO: there are at most 50 peers, but this may cause error
    struct kevent events[50];
};
