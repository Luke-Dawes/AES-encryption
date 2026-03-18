#pragma once
#include <cstdint>
#include <bitset>
#include <vector>


const int KEYSIZE = 128;
const int BLOCKSIZE = 128;

typedef std::bitset<128> bit128;
typedef std::bitset<32> bit32;

std::vector<bit128> keySchedule(bit128 key);
std::vector<bit32> split128to32(bit128 key);