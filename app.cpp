#include <iostream>
#include "dynamic_bitset.hpp"
#include <bitset>

int main()
{
	/*bit::Bitset b(130);
	b.set(0);
	b.set(1);
	b.set(2);
	std::cout << b << std::endl;
	b.reset(0);
	std::cout << b << std::endl;
	b.set();
	std::cout << b << std::endl;
	b[0] = false;
	std::cout << b << std::endl;
	b.flip(0);
	std::cout << b << std::endl;*/

	bit::Bitset <> b(130);
	b.set();
	std::cout << b;
	return 0;
}