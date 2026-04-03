#include "KeySchedule.h"
#include "AES.h"
#include "UnEncryptAES.h"
#include <iostream>
#include <iomanip>

int main() {

	unsigned long long high = 0xC72F0CFE30822CD1ULL;
	unsigned long long low = 0xFF4AF33E5FAD9100ULL;

	bit128 key = (std::bitset<128>(high) << 64) | std::bitset<128>(low);


	//bit128 key(std::string("11000111001011110000110011111110001100001000001000101100110100011111111101001010111100110011111001011111101011011001000100000000"));
	std::vector<bit128> roundKeys = keySchedule(key);

	std::string plaintext = "Hello, World!123"; //16 bytes = 128 bits

	auto temp = AESEncrypt(stringtobit128(plaintext), roundKeys);

	auto printing = split128to32(temp);

	std::cout << "\n\n";

	for (int j = 0; j < 4; ++j) {
		unsigned int wordVal = (unsigned int)printing[j].to_ulong();
		std::cout << std::hex << std::setw(8) << std::setfill('0') << wordVal << " ";
	}
	std::cout << "\n\n";

	auto decrypted = AESDecrypt(temp, roundKeys);

	std::cout << "Decrypted: ", bit128tostring(decrypted);

	std::cout << "\n\n";

	//sub bytes works
	std::cout << bit128tostring(invSubBytes(subBytes(stringtobit128(plaintext)))) << "\n";

	//shift rows works
	std::cout << bit128tostring(invShiftRows(shiftRows(stringtobit128(plaintext)))) << "\n";

	//mix columns works
	std::cout << bit128tostring(invMixColumns(mixColumns(stringtobit128(plaintext))));


}
