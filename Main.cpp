#include "Main.h"

int main() {
	
}

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


std::vector<bit128> keySchedule(bit128 k) {

	std::vector<bit32> w = split128to32(k);



}