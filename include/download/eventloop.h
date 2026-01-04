#pragma once

#include <sys/event.h>
#include <vector>

class eventloop {
  public:
    eventloop();
    std::vector<int> poll_events();

    void register_fd(int fd);
    void remove_fd(int fd);

  private:
    int kq;
    struct kevent events[50];
};
