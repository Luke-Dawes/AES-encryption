#include "AES.h"


bit128 AESEncrypt(bit128 state, std::vector<bit128>& keys) 
{
	stateXor(state, keys[0]);
	for (int j = 1; j < 10; ++j) {
		state = mixColumns(shiftRows(subBytes(state)));
		stateXor(state, keys[j]);
	}

	state = shiftRows(subBytes(state));
	stateXor(state, keys[10]);
	return state;
}

inline void stateXor(bit128& state, bit128& key) {
	state ^= key;
}

unsigned char xtime(unsigned char x) { //ai for maths
	return ((x << 1) ^ (((x >> 7) & 1) * 0x1B)) & 0xFF;
}

bit128 stringtobit128(const std::string& plaintext) { //ai
	bit128 bits;
	// Iterate through up to 16 characters (128 bits)
	for (size_t i = 0; i < plaintext.size() && i < 16; ++i) {
		unsigned char c = plaintext[i];
		for (int j = 0; j < 8; ++j) {
			// Set bit based on character's binary value
			// (15 - i) * 8 places characters in big-endian order
			if ((c >> j) & 1) {
				bits.set((15 - i) * 8 + j);
			}
		}
	}
	return bits;
}



bit128 subBytes(bit128 state) {

	bit128 result;

	for (int i = 0; i < 16; ++i) {
		unsigned char byte = (unsigned char)((state >> (120 - (i * 8))) & bit128(0xFF)).to_ulong();
		unsigned char substituted = sbox[byte];
		result |= (bit128(substituted) << (120 - (i * 8)));
	}
	return result;
}

bit128 shiftRows(bit128 state) {

	std::vector<bit32> cols = split128to32(state);

	std::vector<std::vector<bit8> > matrix(4);

	for (int i = 0; i < 4; ++i) {
		matrix[i] = split32to8(cols[i]);
	}

	bit8 temp1 = matrix[0][1];
	matrix[0][1] = matrix[1][1];
	matrix[1][1] = matrix[2][1];
	matrix[2][1] = matrix[3][1];
	matrix[3][1] = temp1;

	std::swap(matrix[0][2], matrix[2][2]);
	std::swap(matrix[1][2], matrix[3][2]);

	bit8 temp3 = matrix[3][3];
	matrix[3][3] = matrix[2][3];
	matrix[2][3] = matrix[1][3];
	matrix[1][3] = matrix[0][3];
	matrix[0][3] = temp3;

	std::vector<bit32> newCols(4);

	for (int i = 0; i < 4; ++i) {
		newCols[i] = join8to32(matrix[i]);
	}
	return join32to128(newCols);
}

bit128 mixColumns(bit128 state) { //ai for maths
	auto cols = split128to32(state);
	std::vector<bit32> resultCols;

	for (int i = 0; i < 4; i++) {
		auto bytes = split32to8(cols[i]);

		unsigned char s[4];
		for (int j = 0; j < 4; ++j) s[j] = (unsigned char)bytes[j].to_ulong();

		unsigned char r[4];
		r[0] = xtime(s[0]) ^ (xtime(s[1]) ^ s[1]) ^ s[2] ^ s[3];
		r[1] = s[0] ^ xtime(s[1]) ^ (xtime(s[2]) ^ s[2]) ^ s[3];
		r[2] = s[0] ^ s[1] ^ xtime(s[2]) ^ (xtime(s[3]) ^ s[3]);
		r[3] = (xtime(s[0]) ^ s[0]) ^ s[1] ^ s[2] ^ xtime(s[3]);

		std::vector<bit8> mixedBytes = { bit8(r[0]), bit8(r[1]), bit8(r[2]), bit8(r[3]) };
		resultCols.push_back(join8to32(mixedBytes));
	}

	return join32to128(resultCols);


}

std::vector<bit8> split32to8(bit32 k) {
	std::vector<bit8> results;

	for (int i = 0; i < 4; ++i) {
		bit8 chunk;
		for (int j = 0; j < 8; ++j) {
			chunk[j] = k[(8 * (3 - i)) + j];
		}
		results.push_back(chunk);
	}

	return results;
}

bit32 join8to32(std::vector<bit8> k) {
	bit32 result;
	result |= (bit32(k[0].to_ulong()) << 24);
	result |= (bit32(k[1].to_ulong()) << 16);
	result |= (bit32(k[2].to_ulong()) << 8);
	result |= bit32(k[3].to_ulong());
	return result;
}

bit128 join32to128(std::vector<bit32> k) {
	bit128 result;
	result |= (bit128(k[0].to_ulong()) << 96);
	result |= (bit128(k[1].to_ulong()) << 64);
	result |= (bit128(k[2].to_ulong()) << 32);
	result |= bit128(k[3].to_ulong());
	return result;
}