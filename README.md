# Bit Torrent
This is a re-creation of BitTorrent v1.0, a project intended for personal learning.
The goal of this project is to help me learn more about distributed systems and 
and other infrastructure programming in C++ like networking and concurrency. 

I followed this [wiki](https://wiki.theory.org/BitTorrentSpecification) as project spec.

Handshake Protocol Format
![handshake img](assets/handshake_protocol.png "Handshake Protocol")

# Scope
The version 1.0 of the BitTorrent protocol specification includes:
- Bencoding
- Torrent file structure
- Peer wire protocol
- Tracker HTTP/HTTPS protocol
- Kqueue for event polling (not part of BitTorrent)

# Project Requirements
- MacOs (for kqueue)
- C++20
- CMake

# How to Build and Run
- mkdir build
- cd build
- cmake ..
- make -j 8
- ./bin/bit_torrent <path to .torrent_file>

# Testing
- ctest

# Demo
- TODO
