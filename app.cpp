#include <iostream>
#include "dynamic_bitset.hpp"
#include <bitset>
#include <vector>

int main()
{
	bit::Bitset <std::vector <int>> b(130);
	b.set();
	std::cout << b << std::endl;
	std::cout << (b << 129) << std::endl;
	return 0;
}