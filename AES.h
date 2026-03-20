#pragma once
#include "KeySchedule.h"

bit128 AESEncrypt(bit128 plaintext, std::vector<bit128>& keys); 

inline void stateXor(bit128& state, bit128& key);

bit128 subBytes(bit128 state);
bit128 shiftRows(bit128 state);
bit128 mixColumns(bit128 state);

bit32 join8to32(std::vector<bit8> k);
bit128 join32to128(std::vector<bit32> k);
std::vector<bit8> split32to8(bit32 k);

unsigned char xtime(unsigned char x); //ai

bit128 stringtobit128(const std::string& plaintext); //ai


const int fieldMatrix[4][4] =
{ {2,3,1,1},
{ 1,2,3,1},
{1,1,2,3},
{3,1,1,2}
};