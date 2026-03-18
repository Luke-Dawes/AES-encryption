#include "AES.h"


std::vector<bit32> split128to32(bit128 k) {
	std::vector<bit32> w;

	bit128 mask(0xFFFFFFFF);

	for (int i = 0; i < 4; ++i) {
		unsigned long chunk = ((k >> (i * 32)) & mask).to_ulong();
		w.push_back(chunk);
	}
	return w;
}



bit32 rotWord(bit32 w) { //wrong

	std::string result = "";

	std::string temp = w.to_string();

	for (int i = 1; i < 4; ++i) {
		result += temp.substr(i, 8);
	}
	result += temp.substr(0, 8);

	return bit32(result);

}

bit32 subWord(bit32 w) {

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
			
			bit32 newKey = rotWord(subWord(temp)) ^ Rcon(i / 4);
		}
	}

}