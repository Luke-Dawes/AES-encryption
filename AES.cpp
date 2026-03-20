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

bit128 subBytes(bit128 state) {

	bit128 result;

	for (int i = 0; i < 16; ++i) {
		unsigned char byte = (state >> (static_cast<size_t>(i) * 8)).to_ulong() & 0xFF;

		unsigned char substituted = sbox[byte];

		result |= (bit128(substituted) << (static_cast<size_t>(i) * 8));
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

bit128 mixColumns(bit128 state) {
	std::vector<std::vector<bit8> > BrokenDown;

	for (int i = 0; i < 4; i++) {

		auto temp = split128to32(state);

		BrokenDown.push_back(split32to8(temp[i]));
	}

	std::vector<bit8> tempCol;
	for (int i = 0; i < 4; i++) {
		tempCol.clear();
		for (int j = 0; j < 4; j++) {
			tempCol.push_back(BrokenDown[j][i]);
		}

		//logic has to go here about mixing the columns. 


	}
	

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