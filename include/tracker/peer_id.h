#pragma once

#include "parsing/buffer.h"
#include <random>
#include <string>

static const int PEER_ID_LEN = 20;
static std::random_device rd;

std::string generate_peer_id();
