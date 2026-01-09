#include "download/eventloop.h"
#include "spdlog/spdlog.h"
#include <sys/event.h>
#include <vector>

eventloop::eventloop() : kq(-1) {
    kq = kqueue();
    if (kq == -1) {
        spdlog::error("Failed to create a kqueue");
        exit(1);
    }
}

std::vector<int> eventloop::poll_events() {
    struct kevent events[MAX_EVENT_POLL];
    struct timespec timeout = {5, 0}; // Wait 5 seconds max

    // Call kevent to query 50 events
    int new_events_count = kevent(kq, NULL, 0, events, MAX_EVENT_POLL, &timeout);

    if (new_events_count == -1) {
        spdlog::warn("failed to poll event or no events within 5 seconds");
        return {};
    }

    std::vector<int> ready_fds;
    ready_fds.reserve(new_events_count);

    for (int i = 0; i < new_events_count; i++) {
        int fd = (int)events[i].ident;

        if (events[i].flags & EV_ERROR)
            spdlog::debug("Error on socket {}", fd);

        ready_fds.push_back(fd);
    }

    return ready_fds;
}

void eventloop::register_fd(int fd) {
    struct kevent change;

    // EVFILT_READ (wait for data)
    // EV_ADD (add to kqueue) | EV_ENABLE (start watching)
    EV_SET(&change, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
        spdlog::warn("Failed to register fd {} to kqueue", fd);
    }
}
void eventloop::remove_fd(int fd) {
    struct kevent change;

    // EV_DELETE (remove from kqueue)
    EV_SET(&change, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);

    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
        spdlog::warn("Failed to delete fd {} from kqueue", fd);
    }
}
