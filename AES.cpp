#include "AES.h"


std::vector<bit32> split128to32(bit128 k) {
	std::vector<bit32> w;

	bit128 mask(0xFFFFFFFF);

	for (int i = 0; i < 4; ++i) {
		unsigned long chunk = (k >> (32 * (3 - i))).to_ulong();
		w.push_back(bit32(chunk));
	}
	return w;
}



bit32 rotWord(bit32 w) {
	return (w << 8) | (w >> 24);
}

bit32 subWord(bit32 w) {

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

}