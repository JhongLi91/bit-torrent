#include "download/job.h"
#include "parsing/buffer.h"
#include <cassert>
#include <cstdint>

job_t::job_t() {}

job_t::job_t(uint32_t idx, uint32_t block_len, uint32_t piece_len)
    : working(false), idx(idx), begin(0), block_len(block_len), piece_len(piece_len),
      data(buffer_t(piece_len)) {}

bool job_t::is_finished() { return begin + block_len == piece_len && pending.empty(); }

void job_t::schedule_next() {
    if (pending.size() >= MAX_JOBS_IN_QUEUE || is_finished())
        return;
    pending.push(begin);
    begin += block_len;
}

void job_t::append_data(buffer_t &buf) {
    assert(pending.size() > 0);
    setNB(data, buf.data(), block_len, pending.front());
    pending.pop();
}
