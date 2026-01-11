#pragma once

#include "parsing/buffer.h"
#include <cstdint>
#include <queue>

const uint32_t MAX_JOBS_IN_QUEUE = 5;

struct job_t {
    job_t();
    job_t(uint32_t idx, uint32_t block_len, uint32_t piece_len);
    void append_data(buffer_t &buf);
    bool is_finished();

    bool working;

    uint32_t idx;
    uint32_t begin;
    uint32_t block_len;
    uint32_t piece_len;

    std::queue<uint32_t> pending;
    buffer_t data;
};
