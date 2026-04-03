#include "UnEncryptAES.h"

bit128 AESDecrypt(bit128 state, std::vector<bit128>& keys) {
	stateXor(state, keys[10]);	
	state = invSubBytes(invShiftRows(state));
	for (int j = 9; j > 0; --j) {
		stateXor(state, keys[j]);
		state = invMixColumns(state);
		state = invShiftRows(state);
		state = invSubBytes(state);
	}
	stateXor(state, keys[0]);
	return state;
}

bit128 invSubBytes(bit128 state) {
	bit128 result;
	for (int i = 0; i < 16; ++i) {
		unsigned char byte = (unsigned char)((state >> (120 - (i * 8))) & bit128(0xFF)).to_ulong();
		unsigned char substituted = inv_box[byte];
		result |= (bit128(substituted) << (120 - (i * 8)));
	}
	return result;
}

bit128 invShiftRows(bit128 state) {
	std::vector<bit32> cols = split128to32(state);
	std::vector<std::vector<bit8>> matrix(4);
	for (int i = 0; i < 4; ++i) matrix[i] = split32to8(cols[i]);

	bit8 temp1 = matrix[1][3];
	matrix[1][3] = matrix[1][2];
	matrix[1][2] = matrix[1][1];
	matrix[1][1] = matrix[1][0];
	matrix[1][0] = temp1;

	std::swap(matrix[2][0], matrix[2][2]);
	std::swap(matrix[2][1], matrix[2][3]);

	bit8 temp3 = matrix[3][0];
	matrix[3][0] = matrix[3][1];
	matrix[3][1] = matrix[3][2];
	matrix[3][2] = matrix[3][3];
	matrix[3][3] = temp3;

	std::vector<bit32> newCols(4);
	for (int i = 0; i < 4; ++i) newCols[i] = join8to32(matrix[i]);

	return join32to128(newCols);
}

bit128 invMixColumns(bit128 state) {
	auto cols = split128to32(state);
	std::vector<bit32> resultCols;

	for (int i = 0; i < 4; i++) {
		auto bytes = split32to8(cols[i]);

		unsigned char s[4];
		for (int j = 0; j < 4; ++j)
			s[j] = (unsigned char)bytes[j].to_ulong();

		unsigned char r[4];

		r[0] = mul(0x0e, s[0]) ^ mul(0x0b, s[1]) ^ mul(0x0d, s[2]) ^ mul(0x09, s[3]);
		r[1] = mul(0x09, s[0]) ^ mul(0x0e, s[1]) ^ mul(0x0b, s[2]) ^ mul(0x0d, s[3]);
		r[2] = mul(0x0d, s[0]) ^ mul(0x09, s[1]) ^ mul(0x0e, s[2]) ^ mul(0x0b, s[3]);
		r[3] = mul(0x0b, s[0]) ^ mul(0x0d, s[1]) ^ mul(0x09, s[2]) ^ mul(0x0e, s[3]);

		std::vector<bit8> mixedBytes = {
			bit8(r[0]), bit8(r[1]), bit8(r[2]), bit8(r[3])
		};

		resultCols.push_back(join8to32(mixedBytes));
	}

	return join32to128(resultCols);
}

unsigned char mul(unsigned char a, unsigned char b) {
	unsigned char result = 0;

	while (b) {
		if (b & 1)
			result ^= a;   

		a = xtime(a);      
		b >>= 1;           
	}

	return result;
}

std::string bit128tostring(const bit128& bits) {
	std::string result;

	for (int i = 0; i < 16; ++i) {
		unsigned char byte = 0;


		for (int j = 0; j < 8; ++j) {
			if (bits[(15 - i) * 8 + j]) {
				byte |= (1 << j);
			}
		}

		//end char check "\0
		if (byte == 0) break;

		result += static_cast<char>(byte);
	}

	return result;
}