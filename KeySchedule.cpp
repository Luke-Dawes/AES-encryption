#include "KeySchedule.h"
#include <iostream>
#include <iomanip>

std::vector<bit32> split128to32(bit128 k) {
	std::vector<bit32> w;

	for (int i = 0; i < 4; ++i) {
		bit32 chunk;
		for (int j = 0; j < 32; ++j) {
			// Pull bit from k and set it in chunk
			chunk[j] = k[(32 * (3 - i)) + j];
		}
		w.push_back(chunk);
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
			temp = subWord(rotWord(temp)) ^ bit32(Rcon[(i / 4) - 1]);
		}

		w.push_back(w[i - 4] ^ temp);
	}
	std::vector<bit128> keys;
	for (int i = 0; i < 44; i += 4) {

		bit128 fullKey(0);
		for (int j = 0; j < 4; ++j) {
			unsigned int wordVal = (unsigned int)w[i + j].to_ulong();
			std::cout << std::hex << std::setw(8) << std::setfill('0') << wordVal << " ";

			fullKey |= (bit128(wordVal) << (32 * (3 - j)));
		}
		std::cout << "\n";
		keys.push_back(fullKey);
	}

	return keys;
}
