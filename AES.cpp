#include "AES.h"


std::vector<bit32> split128to32(bit128 k) {
	std::vector<bit32> w;


	for (int j = 0; j < 4; ++j) {
		bit32 ans = 0;
		for (int i = 0; i < 32; ++i) {
			k.test(j * 32 + i) ? ans.set(i) : ans.reset(i);
		}
		w.push_back(ans);
	}
	return w;
}

std::vector<bit32> rotWord(std::vector<bit32> w) {
	return { w[1], w[2], w[3], w[0] };
}

std::vector<bit128> keySchedule(bit128 k) {

	std::vector<bit32> w = split128to32(k);

	for (int i = 3; i < 43; ++i) {
		bit32 temp = w[i - 1];

		if (i % 4 != 0) {

			bit32 newKey = w[i - 4] ^ temp;
			w.push_back(newKey);
		}
		else {
			
			bit32 newKey = subWord(rotWord(temp)) ^ Rcon(i / 4);
		}
	}

}