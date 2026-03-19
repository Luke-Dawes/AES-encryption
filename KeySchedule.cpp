#include "KeySchedule.h"

std::vector<bit32> split128to32(bit128 k) {
	std::vector<bit32> w;

	bit128 mask(0xFFFFFFFF);

	for (int i = 0; i < 4; ++i) {
		unsigned long chunk = (k >> (32 * (3 - i))).to_ulong();
		w.push_back(bit32(chunk));
	}
	return w;
}

std::vector<bit128> join32to128(std::vector<bit32> w) {
	std::vector<bit128> keys;

	for (size_t i = 0; i < 44; i += 4) {
		bit128 key;
		key |= (bit128(w[i].to_ulong()) << 96);
		key |= (bit128(w[i + 1].to_ulong()) << 64);
		key |= (bit128(w[i + 2].to_ulong()) << 32);
		key |= bit128(w[i + 3].to_ulong());
		keys.push_back(key);
	}
	return keys;
}


bit32 rotWord(bit32 w) {
	return (w << 8) | (w >> 24);
}

bit32 subWord(bit32 w) {
	bit32 result(0);

	for (int i = 0; i < 4; ++i) {
		unsigned char byte = (w >> (i * 8)).to_ulong() & 0xFF;

		unsigned char substituted = sbox[byte];

		result |= (bit32(substituted) << (i * 8));
	}
	return result;
}

std::vector<bit128> keySchedule(bit128 k) {

	std::vector<bit32> w = split128to32(k);

	for (int i = 4; i < 44; ++i) {
		bit32 temp = w[i - 1];

		if (i % 4 == 0) {

			temp = subWord(rotWord(temp)) ^ Rcon[i / 4];
		
		}

		w.push_back(w[i - 4] ^ temp);
	}

	return join32to128(w);
	
}