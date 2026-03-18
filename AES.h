#pragma once
#include <cstdint>
#include <bitset>
#include <vector>


const int KEYSIZE = 128;
const int BLOCKSIZE = 128;

typedef std::bitset<128> bit128;
typedef std::bitset<32> bit32;

//calculate KeySchedule
std::vector<bit128> keySchedule(bit128 key);
std::vector<bit32> split128to32(bit128 key);
std::vector<bit128> join32to128(std::vector<bit32> w);
std::vector<bit32> rotWord(std::vector<bit32> w);
bit32 subWord(bit32 w);
bit32 SBox(bit32 w);

//calculate AES
//unknown return and parameters so ive just make it void and no parameters
void subBytes();
void shiftRows();
void MixColumms();
void addRoundKey();
