#include "AES.h"


bit128 AESEncrypt(bit128 state, std::vector<bit128>& keys) 
{
	stateXor(state, keys[0]);
	for (int j = 1; j < 10; ++j) {
		state = mixColumns(shiftRows(subBytes(state)));
		stateXor(state, keys[j]);
	}

	state = shiftRows(subBytes(state));
	stateXor(state, keys[11]);
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

	auto bit32Vector = split128to32(state); //splits into 4 32bit chunks
	std::vector<bit8> temp;
	int index;
	std::vector<bit32> result;
	result.push_back(bit32Vector[0]);
	for (int i = 1; i < 4; ++i) {
		temp.clear();

		//splits into 4 8bit chunks
		auto bit8Vector = split32to8(bit32Vector[i]); //splits into 4 8 bit chunks

		index = i;

		for (int j = 0; j < 4; ++j) {

			temp.push_back(bit8Vector[index]); 
			index = (i + 1) % 4;
		}
		result.push_back(join8to32(temp));  //joins 4 8bit chunks into one 32
	}
	return join32to128(result); //join 4 32bit chunks into one 128 chunk
}

bit128 mixColumns(bit128 state) {
	std::vector<std::vector<bit8> > BrokenDown;

	for (int i = 0; i < 4; i++) {

		auto temp = split128to32(state);

		BrokenDown.push_back(split32to8(temp[i]));
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