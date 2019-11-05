#include <iostream>
#include "dynamic_bitset.hpp"
#include <bitset>

int main()
{
	bit::Bitset <unsigned char> b(130);
	b.set();
	std::cout << b << std::endl;
	std::cout << (b << 129);
	return 0;
}