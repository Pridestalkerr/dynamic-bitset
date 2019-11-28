#include <iostream>
#include "dynamic_bitset.hpp"
#include <bitset>
#include <vector>
#include <type_traits>

int main()
{
	bit::Bitset <unsigned char> b(2);
	b.set(0);
	std::cout << b << std::endl;
	/*std::cout << (b << 63) << std::endl;
	b[63] = false;
	b[8] = false;
	b[16] = false;
	b[32] = false;
	b[33] = false;
	b[61] = false;
	std::cout << b << std::endl;
	b = b << 0;
	std::cout << b << std::endl;
	b.set();
	std::cout << b << std::endl;
	b <<= 32;
	std::cout << b << std::endl;
	b = b >> 32;
	std::cout << b << std::endl;
	b >>= 16;
	std::cout << b << std::endl;
	b.test(333);*/
	return 0;
}