#include "KeySchedule.h"
#include <iostream>

int main() {

	unsigned long long high = 0xC72F0CFE30822CD1ULL;
	unsigned long long low = 0xFF4AF33E5FAD9100ULL;

	bit128 key = (std::bitset<128>(high) << 64) | std::bitset<128>(low);


	//bit128 key(std::string("11000111001011110000110011111110001100001000001000101100110100011111111101001010111100110011111001011111101011011001000100000000"));
	std::vector<bit128> roundKeys = keySchedule(key);
	return 0;
}
