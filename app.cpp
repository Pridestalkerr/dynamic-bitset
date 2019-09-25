#include <iostream>
#include "dynamic_bitset.hpp"
#include <bitset>

int main()
{
	bit::Bitset b(130);
	b.set(0);
	std::cout << b;
	return 0;
}